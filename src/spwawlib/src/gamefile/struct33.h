#ifndef	INTERNAL_STRUCT33_H
#define	INTERNAL_STRUCT33_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT33 {
	union {
		char	raw[SIZESEC33];
		struct {
			char	data[SIZESEC33];
		} d;
	} u;
} STRUCT33;

#endif	/* INTERNAL_STRUCT33_H */
