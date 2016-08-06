#ifndef	INTERNAL_STRUCT31_H
#define	INTERNAL_STRUCT31_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT31 {
	union {
		char	raw[SIZESEC31];
		struct {
			char	data[SIZESEC31];
		} d;
	} u;
} STRUCT31;

#endif	/* INTERNAL_STRUCT31_H */
