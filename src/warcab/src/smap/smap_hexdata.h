/** \file
 * The SPWaW war cabinet - strategic map - data definitions.
 *
 * Copyright (C) 2012-2016 Erik Cumps <erik.cumps@gmail.com>
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
	SMAP_HH_035,		/*!< Height is 35 meters or higher			*/
	SMAP_HH_START = SMAP_HH_NEG,	/*!< \internal */
	SMAP_HH_LAST  = SMAP_HH_035	/*!< \internal */
} SMAP_HH;

/*! Strategic Map: total number of Hex Height Types */
#define	SMAP_HH_CNT	(SMAP_HH_LAST-SMAP_HH_START+1)

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

	if (h >= 35)		ht = SMAP_HH_035;
	else if (h >= 30)	ht = SMAP_HH_030;
	else if (h >= 25)	ht = SMAP_HH_025;
	else if (h >= 20)	ht = SMAP_HH_020;
	else if (h >= 15)	ht = SMAP_HH_015;
	else if (h >= 10)	ht = SMAP_HH_010;
	else if (h >= 05)	ht = SMAP_HH_005;
	else if (h >= 00)	ht = SMAP_HH_000;
	else			ht = SMAP_HH_NEG;

	return (ht);
}

#endif	/* STRATMAP_HEXDATA_H */