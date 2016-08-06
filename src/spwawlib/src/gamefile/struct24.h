#ifndef	INTERNAL_STRUCT24_H
#define	INTERNAL_STRUCT24_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT24 {
	union {
		char	raw[SIZESEC24];
		struct {
			char	data[SIZESEC24];
		} d;
	} u;
} STRUCT24;

#endif	/* INTERNAL_STRUCT24_H */
