#ifndef	INTERNAL_STRUCT_COMMON_H
#define	INTERNAL_STRUCT_COMMON_H	1

#include "common.h"
#include "common/sizes.h"
#include "strtab/strtab.h"

typedef struct s_MAP_ICON {
	BYTE	file;	/* File ID (TerIIIzJ.shp)	*/
	BYTE	icon;	/* Icon ID in SHP file		*/
} MAP_ICON;

typedef	struct s_UNIT_LIST {
	USHORT	cnt;
	USHORT	list[UNITCOUNT];
} UNIT_LIST;

typedef	struct s_FORMATION_LIST {
	USHORT	cnt;
	USHORT	list[FORMCOUNT];
} FORMATION_LIST;

#endif	/* INTERNAL_STRUCT_COMMON_H */
