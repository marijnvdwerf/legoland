"""frame.py -- stack-frame analysis of one function in the original and in our build.

Recursive-descent disassembly with stack-depth propagation along every edge (stdcall callees pop their
own arguments; __chkstk allocates eax). Every [esp+X] operand becomes an entry-relative slot
(X - depth; 0 = return address, +4 = first parameter, negative = locals).
"""
import sys, re, json, collections
sys.path.insert(0, "tools")
import capstone
import permuter as P
from reccmp.formats import detect_image

MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
ESP = re.compile(r"\[esp(?: \+ (0x[0-9a-f]+|\d+))?\]")
SIZE = {"byte": 1, "word": 2, "dword": 4, "qword": 8, "tbyte": 10}


def analyse(read, imports, names, start, size, stdcall):
    code = read(start, size)
    insns, depth, conflicts, calls = {}, {}, [], []
    work = [(start, 0)]
    pending_eax = None
    while work:
        addr, d = work.pop()
        while start <= addr < start + size:
            if addr in depth:
                if depth[addr] != d:
                    conflicts.append((hex(addr), depth[addr], d))
                break
            ins = next(MD.disasm(code[addr - start: addr - start + 16], addr), None)
            if ins is None:
                break
            insns[addr], depth[addr] = ins, d
            m, op = ins.mnemonic, ins.op_str
            nd = d
            if m == "push":
                nd = d + 4
            elif m == "pop":
                nd = d - 4
            elif m in ("sub", "add") and op.startswith("esp, ") and re.fullmatch(r"esp, (0x[0-9a-f]+|\d+)", op):
                n = int(op[5:], 0)
                nd = d + n if m == "sub" else d - n
            elif m == "mov" and re.fullmatch(r"eax, (0x[0-9a-f]+|\d+)", op) and addr == start:
                pending_eax = int(op[5:], 0)
            elif m == "call":
                tgt = None
                mm = re.fullmatch(r"0x([0-9a-f]+)", op)
                if mm:
                    tgt = names.get(int(mm.group(1), 16), hex(int(mm.group(1), 16)))
                mm = re.fullmatch(r"dword ptr \[0x([0-9a-f]+)\]", op)
                if mm:
                    tgt = imports.get(int(mm.group(1), 16), "iat_" + mm.group(1))
                if tgt is None:
                    tgt = "indirect:" + op
                calls.append((addr, tgt))
                if pending_eax is not None and len(insns) == 2:
                    nd, pending_eax = d + pending_eax, None
                else:
                    nd = d - stdcall.get(tgt.split("::")[-1], 0)
            if m == "ret":
                break
            if m == "jmp":
                mm = re.fullmatch(r"0x([0-9a-f]+)", op)
                if mm:
                    addr, d = int(mm.group(1), 16), nd
                    continue
                mm = re.fullmatch(r"dword ptr \[(\w+)\*4 \+ 0x([0-9a-f]+)\]", op)
                if mm:
                    t = int(mm.group(2), 16)
                    while True:
                        v = int.from_bytes(read(t, 4), "little")
                        if not (start <= v < start + size):
                            break
                        work.append((v, nd))
                        t += 4
                break
            if m.startswith("j") or m.startswith("loop"):
                mm = re.fullmatch(r"0x([0-9a-f]+)", op)
                if mm:
                    work.append((int(mm.group(1), 16), nd))
            addr, d = addr + ins.size, nd
    return insns, depth, conflicts, calls


def slots(insns, depth):
    """entry-relative slot -> list of (addr, size/lea, instruction text)"""
    out = collections.defaultdict(list)
    for a in sorted(insns):
        ins = insns[a]
        for mm in ESP.finditer(ins.op_str):
            x = int(mm.group(1), 0) if mm.group(1) else 0
            kind = "lea" if ins.mnemonic == "lea" else next((v for k, v in SIZE.items() if ins.op_str.startswith(k) or f", {k} ptr" in ins.op_str), "?")
            out[x - depth[a]].append((a, kind, f"{ins.mnemonic} {ins.op_str}"))
    return out


if __name__ == "__main__":
    mode = sys.argv[1]
    o = P.Original()
    syms = json.load(open("permuter_out/.orig_symbols.json"))
    onames = {int(k): v[0] for k, v in syms.items()}
    # the symbol cache's size is OUR function's size when the original's is unknown: decode up to the next
    # original symbol instead (recursive descent only follows reachable code anyway)
    osize = min(a for a in onames if a > 0x004453a0) - 0x004453a0
    print("orig size", hex(osize))
    stdcall = json.loads(sys.argv[2]) if len(sys.argv) > 2 else {}
    insns, depth, conflicts, calls = analyse(o.read, o.imports, onames, 0x004453a0, osize, stdcall)
    print("orig instructions", len(insns), "conflicts", conflicts[:10])
    if mode == "calls":
        c = collections.Counter(t for _, t in calls)
        for t, n in c.most_common():
            print(n, t)
def our_listing(asm_path, func):
    """name -> MSVC frame offset, from the /FAs listing; plus N-depth constant from the prologue."""
    lines = open(asm_path, encoding="latin-1").read().split("\n")
    p = next(i for i, l in enumerate(lines) if re.match(rf"_{func}\s+PROC", l))
    k = p - 1
    offs = {}
    while k > 0 and not lines[k].startswith("_TEXT"):
        mm = re.match(r"(_\w+\$)\s*=\s*(-?\d+)", lines[k])
        if mm:
            offs[mm.group(1)] = int(mm.group(2))
        k -= 1
    depth, c = 0, None
    for l in lines[p + 1:]:
        s = l.strip()
        mm = re.match(r"mov\s+eax,\s*(\d+)", s)
        if mm and depth == 0:
            depth = int(mm.group(1))
            continue
        if s.startswith("push"):
            depth += 4
            continue
        mm = re.search(r"(_\w+\$)\[esp\+(\d+)\]", s)
        if mm:
            c = int(mm.group(2)) - depth
            break
    return {n: v + c for n, v in offs.items()}, lines, p


if __name__ == "__main__" and mode == "frame":
    import subprocess
    # --- ours: built image, address from argv, names from the /FAs listing
    raddr = int(sys.argv[3], 16)
    rimg = detect_image("build/legoland.exe")
    rimports = {imp.addr: imp.name for imp in getattr(rimg, "imports", []) if imp.name}
    rins, rdep, rconf, _ = analyse(lambda a, n: bytes(rimg.read(a, n)), rimports, {}, raddr, 0x7000, stdcall)
    print("ours instructions", len(rins), "conflicts", rconf[:10])
    names, _, _ = our_listing("/tmp/lt/ra.asm", "RunAppraisal")
    rs, os_ = slots(rins, rdep), slots(insns, depth)
    byoff = {}
    for n, v in names.items():
        byoff.setdefault(v, []).append(n)
    print("\n== OUR stack variables (entry-relative; from the listing), with uses found by the disassembly")
    prev = None
    for v in sorted(byoff):
        ext = (v - prev) if prev is not None else 0
        print(f"{v:7d} {'/'.join(byoff[v]):22s} uses={len(rs.get(v, [])):4d}")
        prev = v
    lo_o = min(os_); lo_r = min(rs)
    print("\nlowest slot: orig", lo_o, "ours", lo_r)
    def show(tag, sl, a, b):
        print(f"\n== {tag} slots in [{a}, {b})")
        for v in sorted(k for k in sl if a <= k < b):
            u = sl[v]
            kinds = collections.Counter(k for _, k, _ in u)
            print(f"{v:7d} n={len(u):4d} {dict(kinds)} first={hex(u[0][0])} {u[0][2][:60]}")
    show("ORIG bottom", os_, lo_o, lo_o + 0x90)
    show("OURS bottom", rs, lo_r, lo_r + 0x90)
    show("ORIG top", os_, -0x400, 0x10)
    show("OURS top", rs, -0x400, 0x10)
IDX = re.compile(r"\[esp \+ (\w+)(?:\*(\d))? \+ (0x[0-9a-f]+|\d+)\]")


def indexed(insns, depth):
    out = collections.Counter()
    first = {}
    for a in sorted(insns):
        for mm in IDX.finditer(insns[a].op_str):
            v = int(mm.group(3), 0) - depth[a]
            out[v] += 1
            first.setdefault(v, (hex(a), f"{insns[a].mnemonic} {insns[a].op_str}"))
    return out, first


if __name__ == "__main__" and mode == "arrays":
    raddr = int(sys.argv[3], 16)
    rimg = detect_image("build/legoland.exe")
    rimports = {imp.addr: imp.name for imp in getattr(rimg, "imports", []) if imp.name}
    rins, rdep, _, _ = analyse(lambda a, n: bytes(rimg.read(a, n)), rimports, {}, raddr, 0x7000, stdcall)
    for tag, ii, dd in (("ORIG", insns, depth), ("OURS", rins, rdep)):
        cnt, first = indexed(ii, dd)
        print(f"\n== {tag} indexed [esp+reg*k+disp]: entry-relative disp, lowest 12 and the large-offset ones")
        for v in sorted(cnt)[:12]:
            print(f"{v:7d} n={cnt[v]:3d} {first[v][0]} {first[v][1][:70]}")
        print("   ...")
        for v in sorted(k for k in cnt if k > -1600):
            print(f"{v:7d} n={cnt[v]:3d} {first[v][0]} {first[v][1][:70]}")
        sl = slots(ii, dd)
        print(f"== {tag} constant slots above -1600:")
        for v in sorted(k for k in sl if k > -1600):
            print(f"{v:7d} n={len(sl[v]):3d} {sl[v][0][2][:70]}")

if __name__ == "__main__" and mode == "outs":
    raddr = int(sys.argv[3], 16)
    rimg = detect_image("build/legoland.exe")
    rimports = {imp.addr: imp.name for imp in getattr(rimg, "imports", []) if imp.name}
    rins, rdep, _, rcalls = analyse(lambda a, n: bytes(rimg.read(a, n)), rimports, {}, raddr, 0x7000, stdcall)
    # name our calls by matching call order with the original's (same call sequence by address order)
    ocalls = sorted(calls)
    for tag, ii, dd, cl, lo, hi in (("ORIG", insns, depth, ocalls, -9092, -9040), ("OURS", rins, rdep, sorted(rcalls), -9096, -9064)):
        sl = slots(ii, dd)
        callat = dict(cl)
        addrs = sorted(ii)
        print(f"\n== {tag} address-taken slots [{lo},{hi}): which call gets the address, where the value is read")
        for v in sorted(k for k in sl if lo <= k < hi):
            for a, kind, txt in sl[v]:
                if kind == "lea":
                    i = addrs.index(a)
                    nxt = next((addrs[j] for j in range(i, min(i + 30, len(addrs))) if ii[addrs[j]].mnemonic == "call"), None)
                    pos = sum(1 for j in range(i, addrs.index(nxt)) if ii[addrs[j]].mnemonic == "push") if nxt else -1
                    print(f"{v:7d} lea@{hex(a)} -> call@{hex(nxt) if nxt else None} {callat.get(nxt, '?')} (arg pushed {pos} pushes before the call)")
            reads = [hex(a) for a, kind, txt in sl[v] if kind != "lea"]
            print(f"        reads/writes: {reads}")

if __name__ == "__main__" and mode == "callsites":
    addrs = sorted(insns)
    for target in ("FUN_00444bf0", "FUN_00444c70", "FUN_00444cd0", "FUN_00444d20", "FUN_00444d70"):
        ca = next(a for a, t in calls if t == target)
        i = addrs.index(ca)
        print(f"== {target} @ {hex(ca)}")
        for j in range(max(0, i - 9), i + 1):
            a = addrs[j]
            ins = insns[a]
            extra = ""
            for mm in ESP.finditer(ins.op_str):
                x = int(mm.group(1), 0) if mm.group(1) else 0
                extra = f"   ; slot {x - depth[a]}"
            print(f"   {hex(a)} {ins.mnemonic} {ins.op_str}{extra}")

def norm_seq(ii, dd, callnames):
    """per instruction: (normalised text, [entry-relative constant esp slots])"""
    out = []
    for a in sorted(ii):
        ins = ii[a]
        sl = [(int(m.group(1), 0) if m.group(1) else 0) - dd[a] for m in ESP.finditer(ins.op_str)]
        t = ins.mnemonic + " " + ins.op_str
        if ins.mnemonic == "call":
            t = "call " + callnames.get(a, "?")
        t = re.sub(r"\[esp[^\]]*\]", "[S]", t)
        t = re.sub(r"\b(e?[abcd]x|[abcd][lh]|e?[sd]i|e?bp)\b", "R", t)
        t = re.sub(r"0x[0-9a-f]{5,}", "A", t)  # absolute addresses / branch targets
        out.append((t, sl, a))
    return out


if __name__ == "__main__" and mode == "map":
    import difflib
    raddr = int(sys.argv[3], 16)
    rimg = detect_image("build/legoland.exe")
    rimports = {imp.addr: imp.name for imp in getattr(rimg, "imports", []) if imp.name}
    rins, rdep, _, rcalls = analyse(lambda a, n: bytes(rimg.read(a, n)), rimports, {}, raddr, 0x7000, stdcall)
    # name our calls positionally by target: same callee address -> same name via the orig call list order of first appearance
    onm = dict(calls)
    # our targets are raw addresses; map raw target -> orig name through the order in which distinct targets first appear
    seen_o, seen_r = [], []
    for a, t in sorted(calls):
        if t not in seen_o: seen_o.append(t)
    for a, t in sorted(rcalls):
        if t not in seen_r: seen_r.append(t)
    rmap = dict(zip(seen_r, seen_o))
    rnm = {a: rmap.get(t, t) for a, t in rcalls}
    O, R = norm_seq(insns, depth, onm), norm_seq(rins, rdep, rnm)
    sm = difflib.SequenceMatcher(None, [x[0] for x in O], [x[0] for x in R], autojunk=False)
    pairs = collections.Counter()
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag not in ("equal", "replace") or (tag == "replace" and i2 - i1 != j2 - j1):
            continue
        for k in range(i2 - i1):
            so, sr = O[i1 + k][1], R[j1 + k][1]
            if len(so) == 1 and len(sr) == 1:
                pairs[(so[0], sr[0])] += 1
    names, _, _ = our_listing("/tmp/lt/ra.asm", "RunAppraisal")
    byoff = collections.defaultdict(list)
    for n, v in names.items(): byoff[v].append(n.strip("_$"))
    print("aligned ratio", round(sm.ratio(), 3))
    ocount = collections.Counter(s for x in O for s in x[1])
    print("\n== original slot -> our slots it lines up with (count)  [orig uses]")
    for so in sorted(k for k in ocount if -9172 <= k < -9040):
        cands = sorted(((n, sr) for (o, sr), n in pairs.items() if o == so), reverse=True)[:3]
        txt = ", ".join(f"{sr}:{'/'.join(byoff.get(sr, ['?']))}x{n}" for n, sr in cands)
        print(f"{so:7d} [{ocount[so]:3d}]  {txt}")

if __name__ == "__main__" and mode == "head":
    raddr = int(sys.argv[3], 16)
    n = int(sys.argv[4]) if len(sys.argv) > 4 else 70
    rimg = detect_image("build/legoland.exe")
    rimports = {imp.addr: imp.name for imp in getattr(rimg, "imports", []) if imp.name}
    rins, rdep, _, rcalls = analyse(lambda a, n: bytes(rimg.read(a, n)), rimports, {}, raddr, 0x7000, stdcall)
    names, _, _ = our_listing("/tmp/lt/ra.asm", "RunAppraisal")
    byoff = collections.defaultdict(list)
    for nm, v in names.items(): byoff[v].append(nm.strip("_$"))
    for tag, ii, dd, cl, nm in (("ORIG", insns, depth, dict(calls), {}), ("OURS", rins, rdep, dict(rcalls), byoff)):
        print(f"== {tag}")
        for a in sorted(ii)[:n]:
            ins = ii[a]
            ann = []
            for mm in ESP.finditer(ins.op_str):
                v = (int(mm.group(1), 0) if mm.group(1) else 0) - dd[a]
                ann.append(f"{v}" + (f"={'/'.join(nm[v])}" if v in nm else ""))
            for mm in IDX.finditer(ins.op_str):
                ann.append(f"idx{int(mm.group(3), 0) - dd[a]}")
            c = cl.get(a, "")
            print(f"  {hex(a)} {ins.mnemonic:5s} {ins.op_str:38s} {' '.join(ann)} {c}")

if __name__ == "__main__" and mode == "writes":
    want = [int(x) for x in sys.argv[3].split(",")]
    sl = slots(insns, depth)
    for v in want:
        w = [(a, t) for a, k, t in sl[v] if re.match(r"(mov|add|sub|inc|dec|and|or|xor|shl|sar|shr|imul|neg|not|lea)\b", t) and (", " not in t or ESP.search(t.split(", ")[0])) and not t.startswith(("lea", "push"))]
        w = [(a, t) for a, t in w if ESP.search(t.split(",")[0])]
        reads = len(sl[v]) - len(w)
        print(f"== slot {v}: {len(w)} writes, {reads} reads")
        for a, t in w[:14]:
            print(f"   {hex(a)} {t}")

if __name__ == "__main__" and mode == "ctx":
    want = [int(x) for x in sys.argv[3].split(",")]
    k = int(sys.argv[4]) if len(sys.argv) > 4 else 8
    sl = slots(insns, depth)
    addrs = sorted(insns)
    for v in want:
        uses = [a for a, kind, t in sl[v]]
        step = max(1, len(uses) // k)
        print(f"== slot {v} ({len(uses)} uses), every {step}th use with the next 2 instructions")
        for a in uses[::step][:k]:
            i = addrs.index(a)
            seq = " ; ".join(f"{insns[addrs[j]].mnemonic} {insns[addrs[j]].op_str}" for j in range(i, min(i + 3, len(addrs))))
            print(f"   {hex(a)} {seq}")

if __name__ == "__main__" and mode == "window":
    lo, hi = int(sys.argv[3], 16), int(sys.argv[4], 16)
    cl = dict(calls)
    for a in sorted(x for x in insns if lo <= x < hi):
        ins = insns[a]
        ann = [f"{(int(m.group(1), 0) if m.group(1) else 0) - depth[a]}" for m in ESP.finditer(ins.op_str)]
        ann += [f"idx{int(m.group(3), 0) - depth[a]}" for m in IDX.finditer(ins.op_str)]
        print(f"  {hex(a)} {ins.mnemonic:5s} {ins.op_str:36s} {' '.join(ann)} {cl.get(a, '')}")

if __name__ == "__main__" and mode == "anchors":
    # GetString(id) calls in the original (id = the closest preceding push imm) and in our C source (line numbers)
    addrs = sorted(insns)
    orig_rows = []
    for a, t in sorted(calls):
        if t != "GetString":
            continue
        i = addrs.index(a)
        sid = None
        for j in range(i - 1, max(i - 12, 0), -1):
            mm = re.fullmatch(r"push\s+(0x[0-9a-f]+|\d+)", f"{insns[addrs[j]].mnemonic} {insns[addrs[j]].op_str}")
            if mm:
                sid = int(mm.group(1), 0)
                break
        orig_rows.append((a, sid))
    src = open("src/legoland/challenge.c", encoding="latin-1").read().split("\n")
    lo = next(i for i, l in enumerate(src) if l.startswith("// FUNCTION: LEGOLAND 0x004453a0"))
    hi = next(i for i in range(lo + 1, len(src)) if src[i] == "}")
    ours_rows = []
    for n in range(lo, hi):
        for mm in re.finditer(r"GetString\((0x[0-9a-f]+|\d+)\)", src[n]):
            ours_rows.append((n + 1, int(mm.group(1), 0)))
    used = set()
    print("orig_addr  string_id  our_line")
    for a, sid in orig_rows:
        k = next((k for k, (n, x) in enumerate(ours_rows) if x == sid and k not in used), None)
        if k is not None: used.add(k)
        print(f"{hex(a)}  {hex(sid) if sid is not None else '?':>8}  {ours_rows[k][0] if k is not None else '-'}")
    print("our GetString lines with no original match:", [n for k, (n, x) in enumerate(ours_rows) if k not in used])