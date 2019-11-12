#!/bin/bash

AGE="$1"
YEAR="$2"

if [ -z "${AGE}" -o -z "${YEAR}" ]; then
	echo "Usage: $0 AGE YEAR"
	echo "Where: AGE    maximum age for files to be considered (uses yngfiles syntax)"
	echo "       YEAR   copyright year to check"
	echo
	exit 1
fi

LF="copyrights_to_check_${AGE}_${YEAR}.txt"


grep -i "Copyright (C) 2" $(yngfiles src ${AGE} | awk '{print $7}' | sort) 2>/dev/null | grep -v '^Binary file ' | grep -v ${YEAR} | sed -e 's/:.*$//' >"${LF}"

