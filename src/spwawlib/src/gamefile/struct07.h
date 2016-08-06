#ifndef	INTERNAL_STRUCT07_H
#define	INTERNAL_STRUCT07_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT07 {
	union {
		char	raw[SIZESEC07];
		struct {
			MAP_ICON	icon[MAPWIDTH][MAPHEIGHT];
		} d;
	} u;
} STRUCT07;

#endif	/* INTERNAL_STRUCT07_H */
