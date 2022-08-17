#!/usr/bin/env zsh
directoryToSearch="$1"
includeFile="$2"

while read -r line
do
	file="$line"
	wasIncluded=false
	while read -r line
	do
		if [[ "$line" == *"$includeFile"* ]]; then
			wasIncluded=true
		else
			if [[ "$wasIncluded" == false ]]; then
				echo "$file"
			fi
		fi
	done < <(awk '$1 ~ /^#include/ {print $2}' "$file")
done < <(find "$directoryToSearch" -type f -name "*.cpp" -o -name "*.cxx" -o -name "*.c++" -o -name "*.c")
