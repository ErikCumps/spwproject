#ifndef	INTERNAL_STRUCT16_H
#define	INTERNAL_STRUCT16_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT16 {
	union {
		char	raw[SIZESEC16];
		struct {
			char	data[SIZESEC16];
		} d;
	} u;
} STRUCT16;

#endif	/* INTERNAL_STRUCT16_H */
