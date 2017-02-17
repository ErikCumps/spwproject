#!/bin/bash

# Estimate project codesize.
#
# Copyright (C) 2001,2002,2003,2004,2005,2009,2017 Erik Cumps
#
# License: GPL v2

# Source directories
SDIRS="../.."

# Temp file
TFILE=./.codesize-$$

# DateTimeStamp
STAMP=$(date +%Y%m%d%H%M)

# Only list files
LSONLY=0
if [ ! -z "$1" -a "$1" = "-l" ]; then LSONLY=1; fi

# Find source and project files:
					>  ${TFILE}
# Search on filename
find ${SDIRS} -name '*.bat'		>> ${TFILE}
find ${SDIRS} -name '*.blst'		>> ${TFILE}
find ${SDIRS} -name '*.[ch]'		>> ${TFILE}
find ${SDIRS} -name '*.[ch]pp'		>> ${TFILE}
find ${SDIRS} -name '*.cfg'		>> ${TFILE}
find ${SDIRS} -name '*.pl'		>> ${TFILE}
find ${SDIRS} -name '*.pro'		>> ${TFILE}
find ${SDIRS} -name '*.sh'		>> ${TFILE}

# Early filter
cat ${TFILE}							|
	grep -v 'ad_api\/doc\/'					|
	grep -v 'DATA\/'					|
	grep -v 'QT\/'						|
	grep -v 'QWT\/'						|
	grep -v 'ZLIB\/'					|
	grep -v 'warcab\/moc\/'					|
	grep -v 'warcab\/resource\/resource\.'			|
	grep -v 'warcab/resource/work\/'			|
	grep -v 'stdafx\.h\.cpp'				|
	cat > ${TFILE}_new
rm -f ${TFILE}; mv ${TFILE}_new ${TFILE}

# Sort and discover unique files
md5sum -t $(cat ${TFILE}) | sort -n |
	awk 'BEGIN { c = 0; }
	{
	     	if ( c != $1 ) { print $2; c = $1; }
	}' > ${TFILE}.UNIQ
mv -f ${TFILE}.UNIQ ${TFILE}	&>/dev/null
sort ${TFILE} > "codelist_${STAMP}"

if [ ${LSONLY} -eq 0 ]; then
	# Report sizes
	wc `cat ${TFILE}` | sort -n -r > "codesize_${STAMP}"
fi

rm -f ${TFILE}

