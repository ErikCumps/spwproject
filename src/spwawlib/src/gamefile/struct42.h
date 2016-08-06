#ifndef	INTERNAL_STRUCT42_H
#define	INTERNAL_STRUCT42_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT42 {
	union {
		char	raw[SIZESEC42];
		struct {
			char	data[SIZESEC42];
		} d;
	} u;
} STRUCT42;

#endif	/* INTERNAL_STRUCT42_H */
