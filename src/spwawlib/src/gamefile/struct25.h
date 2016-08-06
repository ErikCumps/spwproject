#ifndef	INTERNAL_STRUCT25_H
#define	INTERNAL_STRUCT25_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT25 {
	union {
		char	raw[SIZESEC25];
		struct {
			char	data[SIZESEC25];
		} d;
	} u;
} STRUCT25;

#endif	/* INTERNAL_STRUCT25_H */
