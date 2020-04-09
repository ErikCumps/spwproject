#!/bin/bash

SRC="$1"; DST="${SRC}.smap"; NAME="$2"
if [ -z "${SRC}" -o ! -e "${SRC}.c3g" ]; then
	echo "Usage: $0 SRC [NAME]"
	echo "Where: SRC    source file basename"
	echo "       NAME   optional NAME (default is SRC)"
	echo
	exit 1
fi
if [ -z "${NAME}" ]; then NAME="${SRC}"; fi

convert_spwaw()
{
	local	SRC="$1" DST="$2.spwaw" NAME="$3"

	echo "static COLOR spwaw_${NAME}_colors[] = {"				>"${DST}"

	cat "${SRC}.c3g"							|
	grep 'rgb'								|
	sed -e's/^.*rgb(//' -e 's/).*//'					|
	awk 'BEGIN{i=0}{if((i%14)==0){print "\t{ " $0 " },";}i=i+1;}'		>>"${DST}"
	echo "};"								>>"${DST}"
	echo "static COLORFIELDS spwaw_${NAME} = { ARRAYCOUNT(spwaw_${NAME}_colors), spwaw_${NAME}_colors, 100, 100 };"	>>"${DST}"
}

convert_winspww2()
{
	local	SRC="$1" DST="$2.winspww2" NAME="$3"

	echo "static COLOR winspww2_${NAME}_colors[] = {"			>"${DST}"

	cat "${SRC}.c3g"							|
	grep 'rgb'								|
	sed -e's/^.*rgb(//' -e 's/).*//'					|
	awk 'BEGIN{i=0}{if((i%4)==0){print "\t{ " $0 " },";}i=i+1;}'		>>"${DST}"
	echo "};"								>>"${DST}"
	echo "static COLORFIELDS winspww2_${NAME} = { ARRAYCOUNT(winspww2_${NAME}_colors), winspww2_${NAME}_colors, 100, 100 };"	>>"${DST}"
}

convert_spwaw    "${SRC}" "${DST}" "${NAME}"
convert_winspww2 "${SRC}" "${DST}" "${NAME}"

