#ifndef	INTERNAL_STRUCT51_H
#define	INTERNAL_STRUCT51_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT51 {
	union {
		char	raw[SIZESEC51];
		struct {
			char	data[SIZESEC51];
		} d;
	} u;
} STRUCT51;

#endif	/* INTERNAL_STRUCT51_H */
