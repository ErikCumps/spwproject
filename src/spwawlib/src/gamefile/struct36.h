#ifndef	INTERNAL_STRUCT36_H
#define	INTERNAL_STRUCT36_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT36 {
	union {
		char	raw[SIZESEC36];
		struct {
			char	data[SIZESEC36];
		} d;
	} u;
} STRUCT36;

#endif	/* INTERNAL_STRUCT36_H */
