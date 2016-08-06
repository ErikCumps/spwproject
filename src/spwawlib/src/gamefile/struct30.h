#ifndef	INTERNAL_STRUCT30_H
#define	INTERNAL_STRUCT30_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT30 {
	union {
		char	raw[SIZESEC30];
		struct {
			char	data[SIZESEC30];
		} d;
	} u;
} STRUCT30;

#endif	/* INTERNAL_STRUCT30_H */
