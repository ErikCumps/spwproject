/** \file
 * The SPWaW Library - API definitions.
 *
 * Copyright (C) 2007-2017 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 *
 * \addtogroup API
 * @{
 */

#ifndef	SPWAW_LIB_DEFINES_H
#define	SPWAW_LIB_DEFINES_H	1

#define	SPWAW_MINSPERTURN	5	/* 5 minutes */	//TODO: move somewhere else?
#define	SPWAW_HEXSIZE		50	/* 50 meters */	//TODO: move somewhere else?

#define	SPWAW_STARTYEAR		1900

#define	SPWAW_AZSCMTTITLE	200
#define	SPWAW_AZSCMTMAPSRC	50
#define	SPWAW_AZSLOCATION	112
#define	SPWAW_AZSNAME		16
#define	SPWAW_AZSOOBNAME	15
#define	SPWAW_AZSDNAME		256	// TODO: integrate with dossier code
#define	SPWAW_AZSDCMT		1024	// TODO: integrate with dossier code

#define	SPWAW_VHEXCNT		21	/* 21 victory hexes */

#define	SPWAW_HEX2M(h_)		(h_ * SPWAW_HEXSIZE)
#define	SPWAW_HEX2K(h_)		((double)(h_ * SPWAW_HEXSIZE) / 1000.0)
#define	SPWAW_HEX2KPH(h_)	(HEX2K(h_) * 60 / SPWAW_MINSPERTURN)

#define	SPWAW_HEX_EE		0x01	/* hex east side	*/
#define	SPWAW_HEX_SE		0x02	/* hex southeast side	*/
#define	SPWAW_HEX_SW		0x04	/* hex southwest side	*/
#define	SPWAW_HEX_WW		0x08	/* hex west side	*/
#define	SPWAW_HEX_NW		0x10	/* hex northwest side	*/
#define	SPWAW_HEX_NE		0x20	/* hex northeast side	*/

#define	SPWAW_SECTION_COUNT	50	/*!< total number of sections in a savegame	*/

#endif	/* SPWAW_LIB_DEFINES_H */

/** @} */
