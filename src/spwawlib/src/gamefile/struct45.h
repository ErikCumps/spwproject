#ifndef	INTERNAL_STRUCT45_H
#define	INTERNAL_STRUCT45_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT45 {
	union {
		char	raw[SIZESEC45];
		struct {
			char	data[SIZESEC45];
		} d;
	} u;
} STRUCT45;

#endif	/* INTERNAL_STRUCT45_H */
