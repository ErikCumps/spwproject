#ifndef	INTERNAL_STRUCT43_H
#define	INTERNAL_STRUCT43_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT43 {
	union {
		char	raw[SIZESEC43];
		struct {
			char	data[SIZESEC43];
		} d;
	} u;
} STRUCT43;

#endif	/* INTERNAL_STRUCT43_H */
