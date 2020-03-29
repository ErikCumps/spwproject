/** \file
 * The SPWaW war cabinet - strategic map - render data - pixmap container
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	STRATMAP_RENDERDATA_PMC_H
#define	STRATMAP_RENDERDATA_PMC_H	1

#include "smap_hexdata.h"
#include "smap_renderdata_xpm.h"

/*! Render data: pixmap container */
typedef struct s_SMAP_RENDERDATA_PMC {
	int	width;				/*!< hex width, in pixels					*/
	int	height;				/*!< hex height, in pixels					*/
	QPixmap	border;				/*!< Hex border							*/
	QPixmap	selection;			/*!< Hex selection indicator					*/
	QPixmap	hexmask[SMAP_HM_CNT];		/*!< Hex render masks						*/
	QPixmap	transparent;			/*!< Transparency mask						*/
	QPixmap	water[SMAP_HM_CNT];		/*!< Water feature colorfields					*/
	QPixmap	bridge[SMAP_HRD_CNT];		/*!< Bridge feature sections					*/
	QPixmap	road1[SMAP_HRD_CNT];		/*!< Primary road feature sections				*/
	QPixmap	road2[SMAP_HRD_CNT];		/*!< Secondary road feature sections				*/
	QPixmap	railr[SMAP_HRD_CNT];		/*!< Reilroad feature sections					*/
	QPixmap	traml[SMAP_HRD_CNT];		/*!< Tramline feature sections					*/
	QPixmap	inf_blue[SMAP_HM_CNT];		/*!< Blue influence colorfields					*/
	QPixmap	inf_red[SMAP_HM_CNT];		/*!< Red influence colorfields					*/
	QPixmap	inf_neutral[SMAP_HM_CNT];	/*!< Neutral influence colorfields				*/
	QPixmap	bluedot[16];			/*!< Blue unit indicators					*/
	QPixmap	reddot[16];			/*!< Red unit indicators					*/
	QPixmap	vh_blue;			/*!< Victory hex claimed by blue player				*/
	QPixmap vh_red;				/*!< Victory hex claimed by blue player				*/
	QPixmap vh_neutral;			/*!< Victory hex, not yet claimed				*/
	QPixmap	frontline[SMAP_HRD_CNT];	/*!< Frontline edges						*/
} SMAP_RENDERDATA_PMC;

/*! Create Renderdata for rendering hexes of the specified size in pixels */
extern bool	SMAP_RENDERDATA_PMC_create	(SMAP_RENDERDATA_PMC &pmc, int size);

/*! Destroy Renderdata */
extern void	SMAP_RENDERDATA_PMC_destroy	(SMAP_RENDERDATA_PMC &pmc);

#endif	/* STRATMAP_RENDERDATA_PMC_H */
