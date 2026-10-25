#!/bin/sh
# usage: regress.sh baseline.txt  (run from worktree root after building)
# Lists functions whose match % went DOWN vs baseline, and functions newly at 100%.
uv run tools/agent/status.py 2>/dev/null > /tmp/regress.cur.$$
awk 'NR==FNR{b[$1]=$4;next} ($1 in b) && $4+0 < b[$1]+0 {print "REGRESSED", $0, "(was " b[$1] ")"} ($1 in b) && $4=="100.00" && b[$1]!="100.00" {print "NEWLY MATCHED", $0}' "$1" /tmp/regress.cur.$$
rm -f /tmp/regress.cur.$$
