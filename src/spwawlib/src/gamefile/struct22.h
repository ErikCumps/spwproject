#ifndef	INTERNAL_STRUCT22_H
#define	INTERNAL_STRUCT22_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT22 {
	union {
		char	raw[SIZESEC22];
		struct {
			char	data[SIZESEC22];
		} d;
	} u;
} STRUCT22;

#endif	/* INTERNAL_STRUCT22_H */
