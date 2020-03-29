/** \file
 * The SPWaW war cabinet - strategic map - render data - XPM data.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	STRATMAP_RENDERDATA_XPM_H
#define	STRATMAP_RENDERDATA_XPM_H	1

/*! Render data: XPM image container */
typedef struct s_SMAP_RENDERDATA_XPMS {
	const char **border;		/*!< Hex border								*/
	const char **maskC;		/*!< Center hex render mask						*/
	const char **maskLE;		/*!< Left-edge-column, Even-numbered-row hex render mask		*/
	const char **maskRE;		/*!< Right-edge-column, Even-numbered-row hex render mask		*/
	const char **maskRO;		/*!< Right-edge-column, Odd-numbered-row hex render mask		*/
	const char **maskB;		/*!< Bottom-edge-row hex render mask					*/
	const char **maskBR;		/*!< Bottom-edge-row, Right-edge-column hex render mask			*/
	const char **sel;		/*!< Hex selection indicator						*/
	const char **selmask;		/*!< Hex selection indicator render mask				*/
	const char **water;		/*!< Color field for water						*/
	const char **bridgeEE;		/*!< Bridge section running to the east edge of the hex			*/
	const char **bridgeSE;		/*!< Bridge section running to the south east edge of the hex		*/
	const char **bridgeSW;		/*!< Bridge section running to the south west edge of the hex		*/
	const char **bridgeWW;		/*!< Bridge section running to the west edge of the hex			*/
	const char **bridgeNW;		/*!< Bridge section running to the north west edge of the hex		*/
	const char **bridgeNE;		/*!< Bridge section running to the north east edge of the hex		*/
	const char **road1EE;		/*!< Primary road section running to the east edge of the hex		*/
	const char **road1SE;		/*!< Primary road section running to the south east edge of the hex	*/
	const char **road1SW;		/*!< Primary road section running to the south west edge of the hex	*/
	const char **road1WW;		/*!< Primary road section running to the west edge of the hex		*/
	const char **road1NW;		/*!< Primary road section running to the north west edge of the hex	*/
	const char **road1NE;		/*!< Primary road section running to the north east edge of the hex	*/
	const char **road2EE;		/*!< Secondary road section running to the east edge of the hex		*/
	const char **road2SE;		/*!< Secondary road section running to the south east edge of the hex	*/
	const char **road2SW;		/*!< Secondary road section running to the south west edge of the hex	*/
	const char **road2WW;		/*!< Secondary road section running to the west edge of the hex		*/
	const char **road2NW;		/*!< Secondary road section running to the north west edge of the hex	*/
	const char **road2NE;		/*!< Secondary road section running to the north east edge of the hex	*/
	const char **railrEE;		/*!< Railroad section running to the east edge of the hex		*/
	const char **railrSE;		/*!< Railroad section running to the south east edge of the hex		*/
	const char **railrSW;		/*!< Railroad section running to the south west edge of the hex		*/
	const char **railrWW;		/*!< Railroad section running to the west edge of the hex		*/
	const char **railrNW;		/*!< Railroad section running to the north west edge of the hex		*/
	const char **railrNE;		/*!< Railroad section running to the north east edge of the hex		*/
	const char **tramlEE;		/*!< Tramline section running to the east edge of the hex		*/
	const char **tramlSE;		/*!< Tramline section running to the south east edge of the hex		*/
	const char **tramlSW;		/*!< Tramline section running to the south west edge of the hex		*/
	const char **tramlWW;		/*!< Tramline section running to the west edge of the hex		*/
	const char **tramlNW;		/*!< Tramline section running to the north west edge of the hex		*/
	const char **tramlNE;		/*!< Tramline section running to the north east edge of the hex		*/
	const char **blue;		/*!< Color field for blue influence					*/
	const char **red;		/*!< Color field for red influence					*/
	const char **neutral;		/*!< Color field for neutral influence					*/
	const char **mask;		/*!< Transparency mask							*/
	const char **bluedot1;		/*!< Blue unit indicator (1 unit)					*/
	const char **bluedot2;		/*!< Blue unit indicator (2 units)					*/
	const char **bluedot3;		/*!< Blue unit indicator (3 units)					*/
	const char **bluedot4;		/*!< Blue unit indicator (4 units)					*/
	const char **bluedot5;		/*!< Blue unit indicator (5 units)					*/
	const char **bluedot6;		/*!< Blue unit indicator (6 units)					*/
	const char **bluedot7;		/*!< Blue unit indicator (7 units)					*/
	const char **bluedot8;		/*!< Blue unit indicator (8 units)					*/
	const char **bluedot9;		/*!< Blue unit indicator (9 units)					*/
	const char **bluedot10;		/*!< Blue unit indicator (10 units)					*/
	const char **bluedot11;		/*!< Blue unit indicator (11 units)					*/
	const char **bluedot12;		/*!< Blue unit indicator (12 units)					*/
	const char **bluedot13;		/*!< Blue unit indicator (13 units)					*/
	const char **bluedot14;		/*!< Blue unit indicator (14 units)					*/
	const char **bluedot15;		/*!< Blue unit indicator (15 units)					*/
	const char **bluedot16;		/*!< Blue unit indicator (16 units)					*/
	const char **reddot1;		/*!< Red unit indicator (1 unit)					*/
	const char **reddot2;		/*!< Red unit indicator (2 units)					*/
	const char **reddot3;		/*!< Red unit indicator (3 units)					*/
	const char **reddot4;		/*!< Red unit indicator (4 units)					*/
	const char **reddot5;		/*!< Red unit indicator (5 units)					*/
	const char **reddot6;		/*!< Red unit indicator (6 units)					*/
	const char **reddot7;		/*!< Red unit indicator (7 units)					*/
	const char **reddot8;		/*!< Red unit indicator (8 units)					*/
	const char **reddot9;		/*!< Red unit indicator (9 units)					*/
	const char **reddot10;		/*!< Red unit indicator (10 units)					*/
	const char **reddot11;		/*!< Red unit indicator (11 units)					*/
	const char **reddot12;		/*!< Red unit indicator (12 units)					*/
	const char **reddot13;		/*!< Red unit indicator (13 units)					*/
	const char **reddot14;		/*!< Red unit indicator (14 units)					*/
	const char **reddot15;		/*!< Red unit indicator (15 units)					*/
	const char **reddot16;		/*!< Red unit indicator (16 units)					*/
	const char **vhexblue;		/*!< Victory hex claimed by blue player					*/
	const char **vhexred;		/*!< Victory hex claimed by red player					*/
	const char **vhexneutral;	/*!< Victory hex, not yet claimed					*/
	const char **frontlineEE;	/*!< Front line running on the east edge of the hex			*/
	const char **frontlineSE;	/*!< Front line running on the south east edge of the hex		*/
	const char **frontlineSW;	/*!< Front line running on the south west edge of the hex		*/
	const char **frontlineWW;	/*!< Front line running on the west edge of the hex			*/
	const char **frontlineNW;	/*!< Front line running on the north west edge of the hex		*/
	const char **frontlineNE;	/*!< Front line running on the north east edge of the hex		*/
} SMAP_RENDERDATA_XPMS;

/*! Render data: height colorfield XPM image container */
typedef struct s_SMAP_RENDERDATA_HCFXPMS {
	const char **hneg;		/*!< Color field for negative height					*/
	const char **h000;		/*!< Color field for height between 0 and 5 meters (exclusive)		*/
	const char **h005;		/*!< Color field for height between 5 and 10 meters (exclusive)		*/
	const char **h010;		/*!< Color field for height between 10 and 15 meters (exclusive)	*/
	const char **h015;		/*!< Color field for height between 15 and 20 meters (exclusive)	*/
	const char **h020;		/*!< Color field for height between 20 and 25 meters (exclusive)	*/
	const char **h025;		/*!< Color field for height between 25 and 30 meters (exclusive)	*/
	const char **h030;		/*!< Color field for height between 30 and 35 meters (exclusive)	*/
	const char **h035;		/*!< Color field for 35 meters height or higher				*/
} SMAP_RENDERDATA_HCFXPMS;

#endif	/* STRATMAP_RENDERDATA_XPM_H */
