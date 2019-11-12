#!/bin/bash

if [ -z "${GIT_DIR}" ]; then
	echo "Please run this script from with a git bash shell!"
	echo
	exit 2
fi

FILE="$1"
YEAR="$2"

if [ -z "${FILE}" -o -z "${YEAR}" ]; then
	echo "Usage: $0 FILE YEAR"
	echo "Where: FILE   file with list of files to check"
	echo "       YEAR   copyright year to check"
	echo
	exit 1
fi

for f in $(<"${FILE}"); do
	git log -n 1 --format=%ad "${f}" | grep -q 2019
	if [ $? -eq 0 ]; then
		echo "${f}"
	fi
done

