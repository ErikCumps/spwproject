#ifndef	INTERNAL_STRUCT10_H
#define	INTERNAL_STRUCT10_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT10 {
	union {
		char	raw[SIZESEC10];
		struct {
			char	data[SIZESEC10];
		} d;
	} u;
} STRUCT10;

#endif	/* INTERNAL_STRUCT10_H */
