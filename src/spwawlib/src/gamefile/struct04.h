#ifndef	INTERNAL_STRUCT04_H
#define	INTERNAL_STRUCT04_H	1

#include "gamefile/struct_common.h"

typedef struct s_STRUCT04 {
	union {
		char	raw[SIZESEC04];
		struct {
			MAP_ICON	icon[MAPWIDTH][MAPHEIGHT];
		} d;
	} u;
} STRUCT04;

#endif	/* INTERNAL_STRUCT04_H */
