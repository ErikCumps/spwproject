#ifndef	INTERNAL_STRUCT44_H
#define	INTERNAL_STRUCT44_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT44 {
	union {
		char	raw[SIZESEC44];
		struct {
			char	data[SIZESEC44];
		} d;
	} u;
} STRUCT44;

#endif	/* INTERNAL_STRUCT44_H */
