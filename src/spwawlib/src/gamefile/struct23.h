#ifndef	INTERNAL_STRUCT23_H
#define	INTERNAL_STRUCT23_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT23 {
	union {
		char	raw[SIZESEC23];
		struct {
			char	data[SIZESEC23];
		} d;
	} u;
} STRUCT23;

#endif	/* INTERNAL_STRUCT23_H */
