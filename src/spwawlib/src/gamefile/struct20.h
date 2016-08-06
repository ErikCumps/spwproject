#ifndef	INTERNAL_STRUCT20_H
#define	INTERNAL_STRUCT20_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT20 {
	union {
		char	raw[SIZESEC20];
		struct {
			char	data[SIZESEC20];
		} d;
	} u;
} STRUCT20;

#endif	/* INTERNAL_STRUCT20_H */
