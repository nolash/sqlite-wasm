#!/bin/bash

f=${1--}
re="^[0-9]*  *([a-zA-Z]*)  *([0-9]*)  *([0-9]*)  *([0-9]*)  *(([0-9]*)  *)?([0-9][0-9])"

while IFS= read -r l; do
	[[ $l =~ $re ]] && echo "${BASH_REMATCH[1]},${BASH_REMATCH[2]},${BASH_REMATCH[3]},${BASH_REMATCH[4]},${BASH_REMATCH[6]},${BASH_REMATCH[7]}"
done
