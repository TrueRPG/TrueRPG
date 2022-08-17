#!/usr/bin/env zsh
set -e
files=$(zsh ./scripts/find-files-without-include.zsh ./src pch.h)
if ! [ -z $files ]; then
	echo "Files that don't include pch.h:"
	echo $files
	exit 1
fi
