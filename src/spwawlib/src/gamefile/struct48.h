#ifndef	INTERNAL_STRUCT48_H
#define	INTERNAL_STRUCT48_H	1

#include "gamefile/struct_common.h"

typedef struct s_RECHEAD {
	char	__data00[4];
	DWORD	size;		/* Data size in bytes	*/
	char	__data01[72];
} RECHEAD;

typedef struct s_STRUCT48 {
	union {
		char	raw[SIZESEC48];
		struct {
			RECHEAD	data;
		} d;
	} u;
} STRUCT48;

#endif	/* INTERNAL_STRUCT48_H */
