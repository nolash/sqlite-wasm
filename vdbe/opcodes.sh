#!/bin/bash

f=${1--}
re='^#define OP_([a-zA-Z0-9]*)  *([0-9]*) ?.*$'
i=0
echo "char opcodemap[][32] = {"
while IFS= read -r l; do
	[[ $l =~ $re ]] && echo "\"${BASH_REMATCH[1]}\","
	((i++))
done 
echo "};"
echo "int opcodecount = $i"
