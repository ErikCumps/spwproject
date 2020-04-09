/** \file
 * The SPWaW war cabinet - strategic map - render data - height colorfield pixmap collection.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	STRATMAP_RENDERDATA_HPMC_H
#define	STRATMAP_RENDERDATA_HPMC_H	1

#include "smap_hexdata.h"
#include "smap_renderdata_pmc.h"

/*! Height map colorfields pixmap collection 
 *
 * Pixmaps collection organization: flattened matrix in row/col order:
 * One row for each colorfield (limit rows in total).
 * SMAP_HM_CNT columns in each row for hexmasked pixmaps.
 *
 * indexing: (row,col) -> row*SMAP_HM_CNT + col
 */
typedef struct s_SMAP_RENDERDATA_HPMC {
	SMAP_HH			limit;		/*!< The highest height supported by this collection		*/
	int			cfcnt;		/*!< The number of height colorfields in this collection	*/
	int			pmcnt;		/*!< Total number of pixmaps in this collection			*/
	QPixmap			*pixmaps;	/*!< Pointer to the pixmaps collection array			*/
} SMAP_RENDERDATA_HPMC;

/* forward declaration */
typedef struct s_SMAP_RENDERDATA SMAP_RENDERDATA;

/*! HPMC ID */
typedef enum e_SMAP_HPMC_ID {
	SMAP_HPMC_UNKNOWN = 0,
	SMAP_HPMC_ID_GREY,
	SMAP_HPMC_ID_TOPO,
	SMAP_HPMC_ID_NORMAL,
	SMAP_HPMC_ID_JUNGLE,
	SMAP_HPMC_ID_DESERT,
} SMAP_HPMC_ID;

extern bool	SMAP_RENDERDATA_HPMC_create	(SMAP_RENDERDATA_PMC &pmc, SMAP_RENDERDATA_HPMC &hpmc, SPWAW_GAME_TYPE gametype, SMAP_HPMC_ID id);
extern void	SMAP_RENDERDATA_HPMC_destroy	(SMAP_RENDERDATA_HPMC &hpmc);

/*! Returns the height map colorfields pixmap index for the specified height and mask index */
static inline int
SMAP_hthm2idx (SMAP_RENDERDATA_HPMC &hpmc, int h, int m)
{
	if (h<SMAP_HH_START) h = SMAP_HH_START;
	if (h>hpmc.limit) h = hpmc.limit;
	return (h*SMAP_HM_CNT+m);
}

#endif	/* STRATMAP_RENDERDATA_HPMC_H */
