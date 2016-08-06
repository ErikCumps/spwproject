#ifndef	INTERNAL_STRUCT11_H
#define	INTERNAL_STRUCT11_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT11 {
	union {
		char	raw[SIZESEC11];
		struct {
			char	data[SIZESEC11];
		} d;
	} u;
} STRUCT11;

#endif	/* INTERNAL_STRUCT11_H */
