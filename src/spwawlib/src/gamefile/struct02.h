#ifndef	INTERNAL_STRUCT02_H
#define	INTERNAL_STRUCT02_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT02 {
	union {
		char	raw[SIZESEC02];
		struct {
			MAP_ICON	icon[MAPWIDTH][MAPHEIGHT];
		} d;
	} u;
} STRUCT02;

#endif	/* INTERNAL_STRUCT02_H */
