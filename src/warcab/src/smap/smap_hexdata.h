/** \file
 * The SPWaW war cabinet - strategic map - data definitions.
 *
 * Copyright (C) 2012-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	STRATMAP_HEXDATA_H
#define	STRATMAP_HEXDATA_H	1

 /*! Strategic Map: Hex Mask Type */
typedef enum e_SMAP_HM {
	SMAP_HM_C = 0,		/*!< Center hex						*/
	SMAP_HM_LE,		/*!< Left-edge-column, Even-numbered-row hex		*/
	SMAP_HM_RE,		/*!< Right-edge-column, Even-numbered-row hex		*/
	SMAP_HM_RO,		/*!< Right-edge-column, Odd-numbered-row hex		*/
	SMAP_HM_B,		/*!< Bottom-edge-row hex				*/
	SMAP_HM_BR,		/*!< Bottom-edge-row, Right-edge-column hex		*/
	SMAP_HM_START = SMAP_HM_C,	/*!< \internal */
	SMAP_HM_LAST  = SMAP_HM_BR	/*!< \internal */
} SMAP_HM;

/*! Strategic Map: total number of Hex Mask Types */
#define	SMAP_HM_CNT	(SMAP_HM_LAST-SMAP_HM_START+1)

/*! Strategic Map: Hex Height Type */
typedef enum e_SMAP_HH {
	SMAP_HH_NEG = 0,	/*!< Height is below 0 meters				*/
	SMAP_HH_000,		/*!< Height is between 0 and 5 meters (exclusive)	*/ 
	SMAP_HH_005,		/*!< Height is between 5 and 10 meters (exclusive)	*/
	SMAP_HH_010,		/*!< Height is between 10 and 15 meters (exclusive)	*/
	SMAP_HH_015,		/*!< Height is between 15 and 20 meters (exclusive)	*/
	SMAP_HH_020,		/*!< Height is between 20 and 25 meters (exclusive)	*/
	SMAP_HH_025,		/*!< Height is between 25 and 30 meters (exclusive)	*/
	SMAP_HH_030,		/*!< Height is between 30 and 35 meters (exclusive)	*/
	SMAP_HH_035,		/*!< Height is between 35 and 40 meters (exclusive)	*/
	SMAP_HH_040,		/*!< Height is between 40 and 45 meters (exclusive)	*/
	SMAP_HH_045,		/*!< Height is between 45 and 50 meters (exclusive)	*/
	SMAP_HH_050,		/*!< Height is between 50 and 50 meters (exclusive)	*/
	SMAP_HH_055,		/*!< Height is between 55 and 60 meters (exclusive)	*/
	SMAP_HH_060,		/*!< Height is between 60 and 65 meters (exclusive)	*/
	SMAP_HH_065,		/*!< Height is between 65 and 70 meters (exclusive)	*/
	SMAP_HH_070,		/*!< Height is between 70 and 75 meters (exclusive)	*/
	SMAP_HH_075,		/*!< Height is between 75 and 80 meters (exclusive)	*/
	SMAP_HH_080,		/*!< Height is between 80 and 85 meters (exclusive)	*/
	SMAP_HH_085,		/*!< Height is between 85 and 90 meters (exclusive)	*/
	SMAP_HH_090,		/*!< Height is between 90 and 95 meters (exclusive)	*/
	SMAP_HH_095,		/*!< Height is between 95 and 100 meters (exclusive)	*/
	SMAP_HH_100,		/*!< Height is between 100 and 105 meters (exclusive)	*/
	SMAP_HH_105,		/*!< Height is between 105 and 110 meters (exclusive)	*/
	SMAP_HH_110,		/*!< Height is between 110 and 115 meters (exclusive)	*/
	SMAP_HH_115,		/*!< Height is between 115 and 120 meters (exclusive)	*/
	SMAP_HH_120,		/*!< Height is between 120 and 125 meters (exclusive)	*/
	SMAP_HH_125,		/*!< Height is between 125 and 130 meters (exclusive)	*/
	SMAP_HH_130,		/*!< Height is between 130 and 135 meters (exclusive)	*/
	SMAP_HH_135,		/*!< Height is between 135 and 140 meters (exclusive)	*/
	SMAP_HH_140,		/*!< Height is between 140 and 145 meters (exclusive)	*/
	SMAP_HH_145,		/*!< Height is between 145 and 150 meters (exclusive)	*/
	SMAP_HH_150,		/*!< Height is 150 meters or higher			*/
	SMAP_HH_START = SMAP_HH_NEG,		/*!< \internal */
	SMAP_HH_LAST_SPWAW = SMAP_HH_035,	/*!< \internal - maximum height for SP:WaW	*/
	SMAP_HH_LAST_WINSPWW2 = SMAP_HH_150	/*!< \internal - maximum height for winSPWW2	*/
} SMAP_HH;

/*! Strategic Map: total number of Hex Height Types for SP:WaW */
#define	SMAP_HH_CNT_SPWAW	(SMAP_HH_LAST_SPWAW-SMAP_HH_START+1)

/*! Strategic Map: total number of Hex Height Types for winSPWW2 */
#define	SMAP_HH_CNT_WINSPWW2	(SMAP_HH_LAST_WINSPWW2-SMAP_HH_START+1)

/*! Strategic Map: return max height type for specified game type */
static inline SMAP_HH
SMAP_gametype2maxheight (SPWAW_GAME_TYPE gametype)
{
	switch (gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
		default:
			return (SMAP_HH_LAST_SPWAW);
		case SPWAW_GAME_TYPE_WINSPWW2:
			return (SMAP_HH_LAST_WINSPWW2);
	}

	/* We shouldn't get here */
	return (SMAP_HH_START);
}

/*! Strategic Map: Hex Influence Type */
typedef enum e_SMAP_HI {
	SMAP_HI_NONE = 0,	/*!< No influence registered				*/
	SMAP_HI_BLUE,		/*!< Blue player					*/
	SMAP_HI_RED,		/*!< Red player (Opponent)				*/
	SMAP_HI_START = SMAP_HI_NONE,	/*!< \internal */
	SMAP_HI_LAST  = SMAP_HI_RED	/*!< \internal */
} SMAP_HI;

/*! Strategic Map: total number of Hex Influence Types */
#define	SMAP_HI_CNT	(SMAP_HI_LAST-SMAP_HI_START+1)

/*! Strategic Map: Hex Road Direction Type */
typedef enum e_SMAP_HRD {
	SMAP_HRD_EE = 0,	/*!< East						*/
	SMAP_HRD_SE,		/*!< South East						*/
	SMAP_HRD_SW,		/*!< South West						*/
	SMAP_HRD_WW,		/*!< West						*/
	SMAP_HRD_NW,		/*!< North West						*/
	SMAP_HRD_NE,		/*!< North East						*/
	SMAP_HRD_START = SMAP_HRD_EE,	/*!< \internal */
	SMAP_HRD_LAST  = SMAP_HRD_NE	/*!< \internal */
} SMAP_HRD;

/*! Strategic Map: total number of Hex Road Direction Types */
#define	SMAP_HRD_CNT	(SMAP_HRD_LAST-SMAP_HRD_START+1)

/*! Strategic Map: Hex Road Direction flag bitmasks */
extern int	SMAP_HRD_mask[SMAP_HRD_CNT];

/*! Strategic Map: Hex Road Direction, 180 degrees flag bitmasks */
extern int	SMAP_HRD_180mask[SMAP_HRD_CNT];

/*! Strategic Map: convert height (in meters) to SMAP_HH */
static inline SMAP_HH
raw2ht (int h)
{
	SMAP_HH	ht;

	switch (h/5) {
		case 30:
			ht = SMAP_HH_150;
			break;
		case 29:
			ht = SMAP_HH_145;
			break;
		case 28:
			ht = SMAP_HH_140;
			break;
		case 27:
			ht = SMAP_HH_135;
			break;
		case 26:
			ht = SMAP_HH_130;
			break;
		case 25:
			ht = SMAP_HH_125;
			break;
		case 24:
			ht = SMAP_HH_120;
			break;
		case 23:
			ht = SMAP_HH_115;
			break;
		case 22:
			ht = SMAP_HH_110;
			break;
		case 21:
			ht = SMAP_HH_105;
			break;
		case 20:
			ht = SMAP_HH_100;
			break;
		case 19:
			ht = SMAP_HH_095;
			break;
		case 18:
			ht = SMAP_HH_090;
			break;
		case 17:
			ht = SMAP_HH_085;
			break;
		case 16:
			ht = SMAP_HH_080;
			break;
		case 15:
			ht = SMAP_HH_075;
			break;
		case 14:
			ht = SMAP_HH_070;
			break;
		case 13:
			ht = SMAP_HH_065;
			break;
		case 12:
			ht = SMAP_HH_060;
			break;
		case 11:
			ht = SMAP_HH_055;
			break;
		case 10:
			ht = SMAP_HH_050;
			break;
		case 9:
			ht = SMAP_HH_045;
			break;
		case 8:
			ht = SMAP_HH_040;
			break;
		case 7:
			ht = SMAP_HH_035;
			break;
		case 6:
			ht = SMAP_HH_030;
			break;
		case 5:
			ht = SMAP_HH_025;
			break;
		case 4:
			ht = SMAP_HH_020;
			break;
		case 3:
			ht = SMAP_HH_015;
			break;
		case 2:
			ht = SMAP_HH_010;
			break;
		case 1:
			ht = SMAP_HH_005;
			break;
		case 0:
			ht = SMAP_HH_000;
			break;
		default:
			if (h < 0)
				ht = SMAP_HH_NEG;
			else
				ht = SMAP_HH_150;
			break;

	}

	return (ht);
}

#endif	/* STRATMAP_HEXDATA_H */