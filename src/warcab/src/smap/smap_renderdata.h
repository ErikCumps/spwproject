/** \file
 * The SPWaW war cabinet - strategic map - render data.
 *
 * Copyright (C) 2012-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	STRATMAP_RENDERDATA_H
#define	STRATMAP_RENDERDATA_H	1

#include "smap_hexdata.h"
#include "smap_renderdata_pmc.h"
#include "smap_renderdata_hpmc.h"

/*! Render data descriptor */
typedef struct s_SMAP_RENDERDATA {
	const char		*desc;		/*!< render data description					*/
	int			width;		/*!< hex width, in pixels					*/
	int			height;		/*!< hex height, in pixels					*/
	int			side;		/*!< hex side, in pixels					*/
	SMAP_RENDERDATA_PMC	pmc;		/*!< Pixmap container						*/
	struct s_hpmc_spwaw {			/*!< height colorfield pixmap collections for SP:WaW		*/
		SMAP_RENDERDATA_HPMC	grey;		/*!< regular grey colorfield for SP:WaW			*/
		SMAP_RENDERDATA_HPMC	topo;		/*!< topographic colorfield for SP:WaW			*/
		SMAP_RENDERDATA_HPMC	tnor;		/*!< normal terrain colorfield for SP:WaW		*/
		SMAP_RENDERDATA_HPMC	tjun;		/*!< jungle terrain colorfield for SP:WaW		*/
		SMAP_RENDERDATA_HPMC	tdes;		/*!< desert terrain colorfield for SP:WaW		*/
	}			hpmc_spwaw;
	struct s_hpmc_winspww2 {		/*!< height colorfield pixmap collections for winSPWW2		*/
		SMAP_RENDERDATA_HPMC	grey;		/*!< regular grey colorfield for winSPWW2		*/
		SMAP_RENDERDATA_HPMC	topo;		/*!< topographic colorfield for winSPWW2		*/
		SMAP_RENDERDATA_HPMC	tnor;		/*!< normal terrain colorfield for winSPWW2		*/
		SMAP_RENDERDATA_HPMC	tjun;		/*!< jungle terrain colorfield for winSPWW2		*/
		SMAP_RENDERDATA_HPMC	tdes;		/*!< desert terrain colorfield for winSPWW2		*/
	}			hpmc_winspww2;
} SMAP_RENDERDATA;

/*! Height colorfield selector type */
typedef enum e_SMAP_HPMC_TYPE {
	SMAP_HPMC_GREY = 0,	/*!< Regular grey colorfield	*/
	SMAP_HPMC_TOPO,		/*!< Topographic colorfield	*/
	SMAP_HPMC_TERRAIN,	/*!< Terrain-based colorfield	*/
} SMAP_HPMC_TYPE;

/*! Create Renderdata for rendering hexes of the specified size in pixels */
extern bool			SMAP_RENDERDATA_create	(SMAP_RENDERDATA &rd, const char *desc, int size);

/*! Destroy Renderdata */
extern void			SMAP_RENDERDATA_destroy (SMAP_RENDERDATA &rd);

/* Return pointer to requested height colorfield pixmap collection */
extern SMAP_RENDERDATA_HPMC *	SMAP_RENDERDATA_hpmc	(SMAP_RENDERDATA &rd, SMAP_HPMC_TYPE type, SPWAW_GAME_TYPE gametype, SPWAW_TERRAIN terrain = SPWAW_TUNKNOWN);

#endif	/* STRATMAP_RENDERDATA_H */
