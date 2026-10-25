#include "exceptlog.h"
#include <windows.h>
#include <stdarg.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

struct ExceptionEntry {
    unsigned int code;
    const char *message;
};

// FUNCTION: LEGOLAND 0x00453da0
int stackdump(void *exc_info, const char *filename) {
    char progname[260] = "Unknown";
    const char *modname = "Unknown";
    EXCEPTION_POINTERS *ep;
    EXCEPTION_RECORD *rec;
    const char *sep;
    CONTEXT *ctx;
    char path[260];
    char *bufend;
    HANDLE hFile;
    int count = 0;
    char buf[1000] = {EmptyString[0]};
    MEMORY_BASIC_INFORMATION mbi;
    char *out;
    int maxlen = 50;
    unsigned char *ip;
    char modpath[260];
    int i;
    char tmp[1000];
    DWORD *sp;
    DWORD *limit;
    DWORD *next;
    char *base;
    char *dot;
    const char *kind;

    if (ExceptionReportStarted == 0) {
        ExceptionReportStarted = 1;
        if (GetModuleFileNameA(NULL, path, 0x104) <= 0) {
            path[0] = 0;
        }
        base = GetFileNameFromPath(path);
        lstrcpyA(progname, base);
        dot = strrchr(progname, '.');
        if (dot != NULL) {
            *dot = 0;
        }
        // STRING: LEGOLAND 0x004b8c78
        lstrcpyA(base, "exceptlog.txt");
        hFile = CreateFileA(path, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,
            NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            // STRING: LEGOLAND 0x004b8c58
            OutputDebugStringA("Error creating exception report");
            return 0;
        }
        ep = (EXCEPTION_POINTERS *)exc_info;
        rec = ep->ExceptionRecord;
        ctx = ep->ContextRecord;
        if (VirtualQuery((LPCVOID)ctx->Eip, &mbi, sizeof(mbi))) {
            if (GetModuleFileNameA((HMODULE)mbi.AllocationBase, modpath, 0x104) > 0) {
                modname = GetFileNameFromPath(modpath);
            }
        }
        // STRING: LEGOLAND 0x004b8c2c
        WriteFileFormatted(hFile, "%s caused %s in module %s at %04x:%08x.\r\n", progname, GetExceptionDescription(rec->ExceptionCode),
            modname, ctx->SegCs, ctx->Eip);
        // STRING: LEGOLAND 0x004b8c08
        WriteFileFormatted(hFile, "Exception handler called in %s.\r\n", filename);
        WriteErrorTimeAndSystemInfo(hFile);
        if (rec->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && rec->NumberParameters >= 2) {
            // STRING: LEGOLAND 0x004b8bfc
            kind = "Read from";
            if (rec->ExceptionInformation[0]) {
                // STRING: LEGOLAND 0x004b8bf0
                kind = "Write to";
            }
            // STRING: LEGOLAND 0x004b8bc0
            wsprintfA(tmp, "%s location %08x caused an access violation.\r\n", kind, rec->ExceptionInformation[1]);
            WriteFileFormatted(hFile, (char *)PercentSFormat, tmp);
        }
        // STRING: LEGOLAND 0x004b8bb8
        WriteFileFormatted(hFile, "\r\n");
        // STRING: LEGOLAND 0x004b8ba8
        WriteFileFormatted(hFile, "Registers:\r\n");
        // STRING: LEGOLAND 0x004b8b80
        WriteFileFormatted(hFile, "EAX=%08x CS=%04x EIP=%08x EFLGS=%08x\r\n", ctx->Eax, ctx->SegCs, ctx->Eip, ctx->EFlags);
        // STRING: LEGOLAND 0x004b8b58
        WriteFileFormatted(hFile, "EBX=%08x SS=%04x ESP=%08x EBP=%08x\r\n", ctx->Ebx, ctx->SegSs, ctx->Esp, ctx->Ebp);
        // STRING: LEGOLAND 0x004b8b34
        WriteFileFormatted(hFile, "ECX=%08x DS=%04x ESI=%08x FS=%04x\r\n", ctx->Ecx, ctx->SegDs, ctx->Esi, ctx->SegFs);
        // STRING: LEGOLAND 0x004b8b10
        WriteFileFormatted(hFile, "EDX=%08x ES=%04x EDI=%08x GS=%04x\r\n", ctx->Edx, ctx->SegEs, ctx->Edi, ctx->SegGs);
        // STRING: LEGOLAND 0x004b8afc
        WriteFileFormatted(hFile, "Bytes at CS:EIP:\r\n");
        ip = (unsigned char *)ctx->Eip;
        for (i = 0; i < StackDumpCodeBytes; i++) {
            __try {
                // STRING: LEGOLAND 0x004b8af4
                WriteFileFormatted(hFile, "%02x ", ip[i]);
            } __except (1) {
                // STRING: LEGOLAND 0x004b8af0
                WriteFileFormatted(hFile, "?? ");
            }
        }
        // STRING: LEGOLAND 0x004b8ae0
        WriteFileFormatted(hFile, "\r\nStack dump:\r\n");
        __try {
            sp = (DWORD *)ctx->Esp;
            /* original reads the stack base from fs:[4] (inline asm); not expressible in pure C */
            limit = sp + StackDumpMaxWords;
            bufend = buf + sizeof(buf) - maxlen;
            out = buf;
            while ((next = sp + 1) <= limit) {
                if (count % StackDumpWordsPerLine == 0) {
                    // STRING: LEGOLAND 0x004b8ad8
                    out += wsprintfA(out, "%08x: ", sp);
                }
                // STRING: LEGOLAND 0x004b8ad4
                sep = " ";
                count++;
                if (count % StackDumpWordsPerLine == 0 || sp + 2 > limit) {
                    sep = "\r\n";
                }
                // STRING: LEGOLAND 0x004b8acc
                out += wsprintfA(out, "%08x%s", *sp, sep);
                sp = next;
                if (out > bufend) {
                    WriteFileFormatted(hFile, (char *)PercentSFormat, buf);
                    buf[0] = 0;
                    out = buf;
                }
            }
            WriteFileFormatted(hFile, (char *)PercentSFormat, buf);
        } __except (1) {
            // STRING: LEGOLAND 0x004b8aa0
            WriteFileFormatted(hFile, "Exception encountered during stack dump.\r\n");
        }
        WriteModuleList(hFile);
        CloseHandle(hFile);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00454290
void WriteFileFormatted(HANDLE file, const char *format, ...) {
    DWORD written;
    char buffer[2000];
    va_list args;

    va_start(args, format);
    wvsprintfA(buffer, format, args);
    WriteFile(file, buffer, lstrlenA(buffer), &written, NULL);
}

// FUNCTION: LEGOLAND 0x004542e0
void WriteModuleList(HANDLE file) {
    SYSTEM_INFO si;
    MEMORY_BASIC_INFORMATION mbi;
    DWORD pagesize;
    DWORD limit;
    DWORD i = 0;
    UINT_PTR prev = 0;

    // STRING: LEGOLAND 0x004b8c90
    WriteFileFormatted(file, "\r\n\tModule list: names, addresses, sizes, time stamps and file times:\r\n");
    GetSystemInfo(&si);
    pagesize = si.dwPageSize;
    limit = (0x40000000 / pagesize) << 2;
    while (i < limit) {
        if (VirtualQuery((LPCVOID)((UINT_PTR)pagesize * i), &mbi, sizeof(mbi)) && mbi.RegionSize > 0) {
            i += mbi.RegionSize / pagesize;
            if (mbi.State == MEM_COMMIT && (UINT_PTR)mbi.AllocationBase > prev) {
                prev = (UINT_PTR)mbi.AllocationBase;
                WriteModuleInfo(file, prev);
            }
        } else {
            i += 0x10000 / pagesize;
        }
    }
}

// FUNCTION: LEGOLAND 0x00454380
void WriteModuleInfo(HANDLE file, UINT_PTR base) {
    char path[MAX_PATH];

    __try {
        if (GetModuleFileNameA((HMODULE)base, path, MAX_PATH) > 0) {
            DWORD fsize;
            FILETIME ftime;
            HMODULE hinst = (HMODULE)base;
            char buf[100];
            HANDLE fh;
            IMAGE_NT_HEADERS *nthdr;

            buf[0] = EmptyString[0];
            memset(buf + 1, 0, sizeof(buf) - 1);
            fsize = 0;
            if (*(WORD *)hinst == IMAGE_DOS_SIGNATURE) {
                nthdr = (IMAGE_NT_HEADERS *)(base + ((IMAGE_DOS_HEADER *)hinst)->e_lfanew);
                if (nthdr->Signature == IMAGE_NT_SIGNATURE) {
                    fh = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL, NULL);
                    if (fh != INVALID_HANDLE_VALUE) {
                        fsize = GetFileSize(fh, NULL);
                        if (GetFileTime(fh, NULL, NULL, &ftime)) {
                            // STRING: LEGOLAND 0x004b8d04
                            wsprintfA(buf, " - file date is ");
                            FormatLocalFileTime(buf + lstrlenA(buf), ftime);
                        }
                        CloseHandle(fh);
                    }
                    // STRING: LEGOLAND 0x004b8cd8
                    WriteFileFormatted(file, "%s, loaded at 0x%08x - %d bytes - %08x%s\r\n", path, hinst, fsize,
                        nthdr->FileHeader.TimeDateStamp, buf);
                }
            }
        }
    } __except (1) {
    }
}

// FUNCTION: LEGOLAND 0x00454500
void FormatLocalFileTime(char *buffer, FILETIME ft) {
    WORD date;
    WORD time;

    if (FileTimeToLocalFileTime(&ft, &ft) && FileTimeToDosDateTime(&ft, &date, &time)) {
        // STRING: LEGOLAND 0x004b8d18
        wsprintfA(buffer, "%d/%d/%d %02d:%02d:%02d", (date >> 5) & 0xf, date & 0x1f, (date >> 9) + 1980,
            time >> 11, (time >> 5) & 0x3f, (time & 0x1f) * 2);
    } else {
        buffer[0] = 0;
    }
}

// FUNCTION: LEGOLAND 0x004545a0
void WriteErrorTimeAndSystemInfo(HANDLE file) {
    FILETIME ft;
    char timestr[100];
    char path[MAX_PATH];
    char user[200];
    char computer[200];
    SYSTEM_INFO si;
    MEMORYSTATUS ms;
    DWORD usize;
    DWORD csize;

    GetSystemTimeAsFileTime(&ft);
    FormatLocalFileTime(timestr, ft);
    // STRING: LEGOLAND 0x004b8d9c
    WriteFileFormatted(file, "Error occurred at %s.\r\n", timestr);
    if (GetModuleFileNameA(NULL, path, MAX_PATH) <= 0) {
        // STRING: LEGOLAND 0x004b8c88
        lstrcpyA(path, "Unknown");
    }
    usize = 200;
    if (!GetUserNameA(user, &usize)) {
        lstrcpyA(user, "Unknown");
    }
    csize = 200;
    if (!GetComputerNameA(computer, &csize)) {
        lstrcpyA(computer, "Unknown");
    }
    // STRING: LEGOLAND 0x004b8d88
    WriteFileFormatted(file, "%s (Version %s)\r\n", path, ProductVersionString);
    // STRING: LEGOLAND 0x004b8d6c
    WriteFileFormatted(file, "Run by %s on machine %s.\r\n", user, computer);
    GetSystemInfo(&si);
    // STRING: LEGOLAND 0x004b8d50
    WriteFileFormatted(file, "%d processor(s), type %d.\r\n", si.dwNumberOfProcessors, si.dwProcessorType);
    ms.dwLength = sizeof(ms);
    GlobalMemoryStatus(&ms);
    // STRING: LEGOLAND 0x004b8d30
    WriteFileFormatted(file, "%d MBytes physical memory.\r\n", (ms.dwTotalPhys + 0xfffff) >> 20);
}

// FUNCTION: LEGOLAND 0x00454700
const char *GetExceptionDescription(unsigned int code) {
    struct ExceptionEntry table[24] = {
        // STRING: LEGOLAND 0x004b8fd8
        {0x40010005, "a Control-C"},
        // STRING: LEGOLAND 0x004b8fc8
        {0x40010008, "a Control-Break"},
        // STRING: LEGOLAND 0x004b8fb0
        {0x80000002, "a Datatype Misalignment"},
        // STRING: LEGOLAND 0x004b8fa0
        {0x80000003, "a Breakpoint"},
        // STRING: LEGOLAND 0x004b8f8c
        {0xc0000005, "an Access Violation"},
        // STRING: LEGOLAND 0x004b8f78
        {0xc0000006, "an In Page Error"},
        // STRING: LEGOLAND 0x004b8f6c
        {0xc0000017, "a No Memory"},
        // STRING: LEGOLAND 0x004b8f54
        {0xc000001d, "an Illegal Instruction"},
        // STRING: LEGOLAND 0x004b8f38
        {0xc0000025, "a Noncontinuable Exception"},
        // STRING: LEGOLAND 0x004b8f20
        {0xc0000026, "an Invalid Disposition"},
        // STRING: LEGOLAND 0x004b8f08
        {0xc000008c, "a Array Bounds Exceeded"},
        // STRING: LEGOLAND 0x004b8eec
        {0xc000008d, "a Float Denormal Operand"},
        // STRING: LEGOLAND 0x004b8ed4
        {0xc000008e, "a Float Divide by Zero"},
        // STRING: LEGOLAND 0x004b8ebc
        {0xc000008f, "a Float Inexact Result"},
        // STRING: LEGOLAND 0x004b8ea0
        {0xc0000090, "a Float Invalid Operation"},
        // STRING: LEGOLAND 0x004b8e8c
        {0xc0000091, "a Float Overflow"},
        // STRING: LEGOLAND 0x004b8e78
        {0xc0000092, "a Float Stack Check"},
        // STRING: LEGOLAND 0x004b8e64
        {0xc0000093, "a Float Underflow"},
        // STRING: LEGOLAND 0x004b8e48
        {0xc0000094, "an Integer Divide by Zero"},
        // STRING: LEGOLAND 0x004b8e34
        {0xc0000095, "an Integer Overflow"},
        // STRING: LEGOLAND 0x004b8e18
        {0xc0000096, "a Privileged Instruction"},
        // STRING: LEGOLAND 0x004b8e04
        {0xc00000fd, "a Stack Overflow"},
        // STRING: LEGOLAND 0x004b8de8
        {0xc0000142, "a DLL Initialization Failed"},
        // STRING: LEGOLAND 0x004b8dcc
        {0xe06d7363, "a Microsoft C++ Exception"}};
    unsigned int i = 0;
    while (i < 24) {
        if (code == table[i].code) {
            return table[i].message;
        }
        i++;
    }
    // STRING: LEGOLAND 0x004b8db4
    return "Unknown exception type";
}

// FUNCTION: LEGOLAND 0x004548f0
char *GetFileNameFromPath(char *path) {
    char *last_backslash = strrchr(path, '\\');
    if (last_backslash != NULL) {
        return last_backslash + 1;
    }
    return path;
}
