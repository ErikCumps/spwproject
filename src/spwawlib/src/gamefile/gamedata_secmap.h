/** \file
 * The SPWaW Library - game data handling - section map.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAMEDATA_SECMAP_H
#define	GAMEDATA_SECMAP_H	1

#include <spwawlib_types.h>

typedef struct s_SECMAPEL {
	int		idx;		/* index					*/
	void		*ptr;		/* offset					*/
	DWORD		size;		/* size	(0 if section must be allocated)	*/
	bool		compress;	/* section likes to be compressed		*/
	bool		optional;	/* section is optional				*/
	bool		freeme;		/* section must be freed after use		*/

} SECMAPEL;

typedef struct s_SECMAP {
	int		cnt;		/* Number of SECMAP elements in the list	*/
	SECMAPEL	*list;		/* List of SECMAP elements			*/
} SECMAP;

extern SECMAP *		gamedata_SECMAP		(SPWAW_GAME_TYPE gametype);
extern SECMAPEL *	gamedata_section	(SECMAP *map, int idx);

extern bool		gamedata_secmap_create	(SECMAP *MAP, SECMAP *map);
extern bool		gamedata_secmap_destroy	(SECMAP *map);

#endif	/* GAMEDATA_SECMAP_H */
