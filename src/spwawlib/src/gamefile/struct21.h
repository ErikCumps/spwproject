#ifndef	INTERNAL_STRUCT21_H
#define	INTERNAL_STRUCT21_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT21 {
	union {
		char	raw[SIZESEC21];
		struct {
			char	data[SIZESEC21];
		} d;
	} u;
} STRUCT21;

#endif	/* INTERNAL_STRUCT21_H */
