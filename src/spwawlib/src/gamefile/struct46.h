#ifndef	INTERNAL_STRUCT46_H
#define	INTERNAL_STRUCT46_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT46 {
	union {
		char	raw[SIZESEC46];
		struct {
			char	data[SIZESEC46];
		} d;
	} u;
} STRUCT46;

#endif	/* INTERNAL_STRUCT46_H */
