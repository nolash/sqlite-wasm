#!/bin/bash

1>_opcodemap.h
f=${1--}
re='^#define OP_([a-zA-Z0-9]*)  *([0-9]*) ?.*$'
i=0
cat <<EOF
char opcodemap[1024][32] = {
EOF

while IFS= read -r l; do
	[[ $l =~ $re ]] && echo "\"${BASH_REMATCH[1]}\","
	((i++))
done 
echo "};"
cat <<EOF
int opcodecount = $i;
EOF
