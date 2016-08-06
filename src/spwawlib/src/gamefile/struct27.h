#ifndef	INTERNAL_STRUCT27_H
#define	INTERNAL_STRUCT27_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT27 {
	union {
		char	raw[SIZESEC27];
		struct {
			char	data[SIZESEC27];
		} d;
	} u;
} STRUCT27;

#endif	/* INTERNAL_STRUCT27_H */
