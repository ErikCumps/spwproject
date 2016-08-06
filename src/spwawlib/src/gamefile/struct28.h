#ifndef	INTERNAL_STRUCT28_H
#define	INTERNAL_STRUCT28_H	1

#include "gamefile/struct_common.h"

typedef struct s_MAP_DATA1 {
	USHORT	count;		/* Tile count		*/
	BYTE	ID;		/* Tile ID + 0xD6 ???)	*/
	BYTE	file;		/* File ID - 1 ???	*/
	char	__data00[10];
} MAP_DATA1;

typedef struct s_STRUCT28 {
	union {
		char	raw[SIZESEC28];
		struct {
			MAP_DATA1	data[MAPWIDTH][MAPHEIGHT];
		} d;
	} u;
} STRUCT28;

#endif	/* INTERNAL_STRUCT28_H */
