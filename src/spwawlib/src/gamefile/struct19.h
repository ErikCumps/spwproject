#ifndef	INTERNAL_STRUCT19_H
#define	INTERNAL_STRUCT19_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT19 {
	union {
		char	raw[SIZESEC19];
		struct {
			char	data[SIZESEC19];
		} d;
	} u;
} STRUCT19;

#endif	/* INTERNAL_STRUCT19_H */
