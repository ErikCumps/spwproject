#ifndef	INTERNAL_STRUCT06_H
#define	INTERNAL_STRUCT06_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT06 {
	union {
		char	raw[SIZESEC06];
		struct {
			MAP_ICON	icon[MAPWIDTH][MAPHEIGHT];
		} d;
	} u;
} STRUCT06;

#endif	/* INTERNAL_STRUCT06_H */
