/** \file
 * The SPWaW war cabinet - strategic map - render data - pixmap container
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "smap_renderdata.h"
#include "smap_renderdata_xpm.h"

/*! XPM data for rendering hexes of size 11x11 pixels */
extern SMAP_RENDERDATA_XPMS xpms_11x11;

/*! XPM data for rendering hexes of size 21x21 pixels */
extern SMAP_RENDERDATA_XPMS xpms_21x21;

bool
SMAP_RENDERDATA_PMC_create (SMAP_RENDERDATA_PMC &pmc, int size)
{
	SMAP_RENDERDATA_XPMS	*xpms;
	int			midx;

	memset (&pmc, 0, sizeof(pmc));
	pmc.width = pmc.height = size;

	switch (pmc.width) {
		case 11:
			xpms = &xpms_11x11;
			break;
		case 21:
			xpms = &xpms_21x21;
			break;
		default:
			return (false);
			break;
	}

	/* Border pixmap */
	pmc.border = xpms->border;

	/* Selection indicator pixmap */
	pmc.selection = xpms->sel;
	pmc.selection.setAlphaChannel (xpms->selmask);

	/* Hex mask pixmaps */
	pmc.hexmask[SMAP_HM_C]  = xpms->maskC;
	pmc.hexmask[SMAP_HM_LE] = xpms->maskLE;
	pmc.hexmask[SMAP_HM_RE] = xpms->maskRE;
	pmc.hexmask[SMAP_HM_RO] = xpms->maskRO;
	pmc.hexmask[SMAP_HM_B]  = xpms->maskB;
	pmc.hexmask[SMAP_HM_BR] = xpms->maskBR;

	/* Transparency pixmap */
	pmc.transparent = xpms->mask;

	/* Water pixmaps */
	for (midx=SMAP_HM_START; midx<=SMAP_HM_LAST; midx++) {
		pmc.water[midx] = xpms->water;
		pmc.water[midx].setAlphaChannel (pmc.hexmask[midx]);
	}

	/* Bridge pixmaps */
	pmc.bridge[0] = xpms->bridgeEE;
	pmc.bridge[1] = xpms->bridgeSE;
	pmc.bridge[2] = xpms->bridgeSW;
	pmc.bridge[3] = xpms->bridgeWW;
	pmc.bridge[4] = xpms->bridgeNW;
	pmc.bridge[5] = xpms->bridgeNE;

	/* Primary road pixmaps */
	pmc.road1[0] = xpms->road1EE;
	pmc.road1[1] = xpms->road1SE;
	pmc.road1[2] = xpms->road1SW;
	pmc.road1[3] = xpms->road1WW;
	pmc.road1[4] = xpms->road1NW;
	pmc.road1[5] = xpms->road1NE;

	/* Secondary road pixmaps */
	pmc.road2[0] = xpms->road2EE;
	pmc.road2[1] = xpms->road2SE;
	pmc.road2[2] = xpms->road2SW;
	pmc.road2[3] = xpms->road2WW;
	pmc.road2[4] = xpms->road2NW;
	pmc.road2[5] = xpms->road2NE;

	/* Railroad pixmaps */
	pmc.railr[0] = xpms->railrEE;
	pmc.railr[1] = xpms->railrSE;
	pmc.railr[2] = xpms->railrSW;
	pmc.railr[3] = xpms->railrWW;
	pmc.railr[4] = xpms->railrNW;
	pmc.railr[5] = xpms->railrNE;

	/* Tramline pixmaps */
	pmc.traml[0] = xpms->tramlEE;
	pmc.traml[1] = xpms->tramlSE;
	pmc.traml[2] = xpms->tramlSW;
	pmc.traml[3] = xpms->tramlWW;
	pmc.traml[4] = xpms->tramlNW;
	pmc.traml[5] = xpms->tramlNE;

	/* Influence pixmaps */
	for (midx=SMAP_HM_START; midx<=SMAP_HM_LAST; midx++) {
		pmc.inf_red[midx] = xpms->red;
		pmc.inf_red[midx].setAlphaChannel (pmc.hexmask[midx]);
		pmc.inf_red[midx].setAlphaChannel (pmc.transparent);
	}

	for (midx=SMAP_HM_START; midx<=SMAP_HM_LAST; midx++) {
		pmc.inf_blue[midx] = xpms->blue;
		pmc.inf_blue[midx].setAlphaChannel (pmc.hexmask[midx]);
		pmc.inf_blue[midx].setAlphaChannel (pmc.transparent);
	}

	for (midx=SMAP_HM_START; midx<=SMAP_HM_LAST; midx++) {
		pmc.inf_neutral[midx] = xpms->neutral;
		pmc.inf_neutral[midx].setAlphaChannel (pmc.hexmask[midx]);
		pmc.inf_neutral[midx].setAlphaChannel (pmc.transparent);
	}

	for (midx=SMAP_HM_START; midx<=SMAP_HM_LAST; midx++) {
		pmc.inf_contested[midx] = xpms->contested;
		pmc.inf_contested[midx].setAlphaChannel (pmc.hexmask[midx]);
		pmc.inf_contested[midx].setAlphaChannel (pmc.transparent);
	}

	/* Blue unit pixmaps */
	pmc.bluedot[ 0] = xpms->bluedot1;
	pmc.bluedot[ 1] = xpms->bluedot2;
	pmc.bluedot[ 2] = xpms->bluedot3;
	pmc.bluedot[ 3] = xpms->bluedot4;
	pmc.bluedot[ 4] = xpms->bluedot5;
	pmc.bluedot[ 5] = xpms->bluedot6;
	pmc.bluedot[ 6] = xpms->bluedot7;
	pmc.bluedot[ 7] = xpms->bluedot8;
	pmc.bluedot[ 8] = xpms->bluedot9;
	pmc.bluedot[ 9] = xpms->bluedot10;
	pmc.bluedot[10] = xpms->bluedot11;
	pmc.bluedot[11] = xpms->bluedot12;
	pmc.bluedot[12] = xpms->bluedot13;
	pmc.bluedot[13] = xpms->bluedot14;
	pmc.bluedot[14] = xpms->bluedot15;
	pmc.bluedot[15] = xpms->bluedot16;
	pmc.bluesplat   = xpms->bluesplat;

	/* Red unit pixmaps */
	pmc.reddot[ 0] = xpms->reddot1;
	pmc.reddot[ 1] = xpms->reddot2;
	pmc.reddot[ 2] = xpms->reddot3;
	pmc.reddot[ 3] = xpms->reddot4;
	pmc.reddot[ 4] = xpms->reddot5;
	pmc.reddot[ 5] = xpms->reddot6;
	pmc.reddot[ 6] = xpms->reddot7;
	pmc.reddot[ 7] = xpms->reddot8;
	pmc.reddot[ 8] = xpms->reddot9;
	pmc.reddot[ 9] = xpms->reddot10;
	pmc.reddot[10] = xpms->reddot11;
	pmc.reddot[11] = xpms->reddot12;
	pmc.reddot[12] = xpms->reddot13;
	pmc.reddot[13] = xpms->reddot14;
	pmc.reddot[14] = xpms->reddot15;
	pmc.reddot[15] = xpms->reddot16;
	pmc.redsplat   = xpms->redsplat;

	/* Victory hex pixmaps */
	pmc.vh_blue      = xpms->vhexblue;
	pmc.vh_red       = xpms->vhexred;
	pmc.vh_neutral   = xpms->vhexneutral;
	pmc.vh_contested = xpms->vhexcontested;

	/* Frontline pixmaps */
	pmc.frontline[0] = xpms->frontlineEE;
	pmc.frontline[1] = xpms->frontlineSE;
	pmc.frontline[2] = xpms->frontlineSW;
	pmc.frontline[3] = xpms->frontlineWW;
	pmc.frontline[4] = xpms->frontlineNW;
	pmc.frontline[5] = xpms->frontlineNE;

	/* Dotted frontline pixmaps */
	pmc.dottedfrontline[0] = xpms->dottedfrontlineEE;
	pmc.dottedfrontline[1] = xpms->dottedfrontlineSE;
	pmc.dottedfrontline[2] = xpms->dottedfrontlineSW;
	pmc.dottedfrontline[3] = xpms->dottedfrontlineWW;
	pmc.dottedfrontline[4] = xpms->dottedfrontlineNW;
	pmc.dottedfrontline[5] = xpms->dottedfrontlineNE;

	return (true);
}

void
SMAP_RENDERDATA_PMC_destroy (SMAP_RENDERDATA_PMC &pmc)
{
	memset (&pmc, 0, sizeof(pmc));
}