/** \file
 * The SPWaW Library - utility code: data to/from text conversion.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	TEXTCONV_H
#define	TEXTCONV_H	1

typedef enum e_TC_DATA {
	TC_DATA_MARKER,
	TC_DATA_STRBUF,

	TC_DATA_BOOL,
	TC_DATA_BYTE,
	TC_DATA_USHORT,
	TC_DATA_ULONG,
	TC_DATA_DWORD,
	TC_DATA_ASCIIZ,
	TC_DATA_TIMESTAMP,

	TC_DATA_IDX_BOOL,
	TC_DATA_IDX_BYTE,
	TC_DATA_IDX_USHORT,
	TC_DATA_IDX_ULONG,
	TC_DATA_IDX_DWORD,
	TC_DATA_IDX_ASCIIZ,
	TC_DATA_IDX_TIMESTAMP,
} TC_DATA;

extern bool	TC_v2t	(FILE *f, char *name, TC_DATA type, void *data = NULL, unsigned int len = 0, int idx = 0);
extern bool	TC_t2v	(FILE *f, char *name, TC_DATA type, void *data = NULL, unsigned int len = NULL, int *idx = NULL);

#endif	/* TEXTCONV_H */
