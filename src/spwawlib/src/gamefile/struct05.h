#ifndef	INTERNAL_STRUCT05_H
#define	INTERNAL_STRUCT05_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT05 {
	union {
		char	raw[SIZESEC05];
		struct {
			MAP_ICON	icon[MAPWIDTH][MAPHEIGHT];
		} d;
	} u;
} STRUCT05;

#endif	/* INTERNAL_STRUCT05_H */
