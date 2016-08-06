#ifndef	INTERNAL_STRUCT15_H
#define	INTERNAL_STRUCT15_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT15 {
	union {
		char	raw[SIZESEC15];
		struct {
			char	data[SIZESEC15];
		} d;
	} u;
} STRUCT15;

#endif	/* INTERNAL_STRUCT15_H */
