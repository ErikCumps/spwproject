/** \file
 * The Steel Panthers World at War report tool.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL V2
 */

#ifndef	INTERNAL_UTILS_H
#define	INTERNAL_UTILS_H	1

#include "stdafx.h"

typedef struct s_MKSTR_SRC {
	BYTE		*ptr;
	DWORD		len;
	DWORD		addr;
} MKSTR_SRC;

typedef struct s_MKSTR_DST {
	char		*ptr;
	unsigned long	len;
} MKSTR_DST;

typedef enum e_MKSTR_TYPE {
	MKSTR_LIST = 0,
	MKSTR_ARRAY,
} MKSTR_TYPE;

typedef struct s_MKSTR_OPT {
	MKSTR_TYPE	type;
	int		width;
} MKSTR_OPT;

extern void	mkstr_bytedata	(MKSTR_DST *dst, MKSTR_SRC *src, MKSTR_OPT *opt);

extern void	UD_report	(SPWAW_UD *ud, char *title, FILE *file);
extern void	UD_block_report	(SPWAW_UD *ud, int width, FILE *file, char *prefix);
extern void	UD_block_report	(SPWAW_UD *ud, char *title, int width, FILE *file, char *prefix);
extern void	UD_block_report	(SPWAW_UD *ud, int width, FILE *file);
extern void	UD_block_report	(SPWAW_UD *ud, char *title, int width, FILE *file);

extern void	error		(char *fmt, ...);

extern void	smart_title	(FILE *rf, char ulc, char *fmt, ...);

extern void	report_mmas	(FILE *rf, char *msg, SPWAW_IMMAS *ptr, SPWAW_SNAP_OOB_FORCE *fp, bool unit);
extern void	report_mmas	(FILE *rf, char *msg, SPWAW_FMMAS *ptr, SPWAW_SNAP_OOB_FORCE *fp, bool unit);

/*! Simple macro to calculate the length of a (statically initialized) array */
#define	ARRAYCOUNT(arr_) (sizeof(arr_)/sizeof(arr_[0]))

#endif	/* INTERNAL_UTILS_H */
