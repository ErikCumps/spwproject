#ifndef	INTERNAL_STRUCT03_H
#define	INTERNAL_STRUCT03_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT03 {
	union {
		char	raw[SIZESEC03];
		struct {
			MAP_ICON	icon[MAPWIDTH][MAPHEIGHT];
		} d;
	} u;
} STRUCT03;

#endif	/* INTERNAL_STRUCT03_H */
