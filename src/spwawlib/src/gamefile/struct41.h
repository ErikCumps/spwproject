#ifndef	INTERNAL_STRUCT41_H
#define	INTERNAL_STRUCT41_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT41 {
	union {
		char	raw[SIZESEC41];
		struct {
			char	data[SIZESEC41];
		} d;
	} u;
} STRUCT41;

#endif	/* INTERNAL_STRUCT41_H */
