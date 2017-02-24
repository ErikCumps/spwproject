/** \file
 * The SPWaW Library - savegame size definitions.
 *
 * Copyright (C) 2007-2017 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_SIZES_H
#define	INTERNAL_SIZES_H	1

#define	SIZESEC00		20
#define	SIZESEC01		206400
#define	SIZESEC02		48000
#define	SIZESEC03		48000
#define	SIZESEC04		48000
#define	SIZESEC05		48000
#define	SIZESEC06		48000
#define	SIZESEC07		48000
#define	SIZESEC08		480000
#define	SIZESEC09		192000
#define	SIZESEC10		4
#define	SIZESEC11		640000
#define	SIZESEC12		2
#define	SIZESEC13		4
#define	SIZESEC14		4
#define	SIZESEC15		2
#define	SIZESEC16		1
#define	SIZESEC17		108000
#define	SIZESEC18		16000
#define	SIZESEC19		400
#define	SIZESEC20		1
#define	SIZESEC21		1
#define	SIZESEC22		2
#define	SIZESEC23		5000
#define	SIZESEC24		6400
#define	SIZESEC25		1600
#define	SIZESEC26		4
#define	SIZESEC27		2
#define	SIZESEC28		336000
#define	SIZESEC29		24000
#define	SIZESEC30		100
#define	SIZESEC31		100
#define	SIZESEC32		892
#define	SIZESEC33		28
#define	SIZESEC34		25600
#define	SIZESEC35		18400
#define	SIZESEC36		1680
#define	SIZESEC37		1276
#define	SIZESEC38		4
#define	SIZESEC39		4
#define	SIZESEC40		120
#define	SIZESEC41		200
#define	SIZESEC42		2
#define	SIZESEC43		2
#define	SIZESEC44		9600
#define	SIZESEC45		2
#define	SIZESEC46		60
#define	SIZESEC47		6400
#define	SIZESEC48		80
#define	SIZESEC51		2

#define	MAPWIDTH		100
#define	MAPHEIGHT		240

#define	LEADERCOUNT		800
#define	LEADERP1START		0
#define	LEADERP1COUNT		400
#define	LEADERP1END		(LEADERP1START + LEADERP1COUNT - 1)
#define	LEADERP2START		400
#define	LEADERP2COUNT		400
#define	LEADERP2END		(LEADERP2START + LEADERP2COUNT - 1)

#define	UNITCOUNT		800
#define	UNITP1START		0
#define	UNITP1COUNT		400
#define	UNITP1END		(UNITP1START + UNITP1COUNT - 1)
#define	UNITP2START		400
#define	UNITP2COUNT		400
#define	UNITP2END		(UNITP2START + UNITP2COUNT - 1)

#define	UNITPOSCOUNT		5400
#define	UNITP1POSSTART		0
#define	UNITP1POSCOUNT		400
#define	UNITP1POSEND		(UNITP1POSSTART + UNITP1POSCOUNT - 1)
#define	UNITP2POSSTART		400
#define	UNITP2POSCOUNT		400
#define	UNITP2POSEND		(UNITP2POSSTART + UNITP2POSCOUNT - 1)
#define	VHEXPOSSTART		800
#define	PREVUNITP1POSSTART	2000
#define	PREVUNITP1POSCOUNT	400
#define	PREVUNITP1POSEND	(PREVUNITP1POSSTART + PREVUNITP1POSCOUNT - 1)
#define	PREVUNITP2POSSTART	2400
#define	PREVUNITP2POSCOUNT	400
#define	PREVUNITP2POSEND	(PREVUNITP2POSSTART + PREVUNITP2POSCOUNT - 1)

#define	FORMCOUNT		400
#define	FORMP1START		0
#define	FORMP1COUNT		100
#define	FORMP1END		(FORMP1START + FORMP1COUNT - 1)
#define	FORMP2START		100
#define	FORMP2COUNT		100
#define	FORMP2END		(FORMP2START + FORMP2COUNT - 1)

#define	PLAYER1			0
#define	PLAYER2			1

#define	VHEXCOUNT		21

#define	MAXMISSIONTYPE		3

#define	MAXTERRAINTYPE		5

#define	MAXWEATHERTYPE		6

/* Maximum supported number of units in a single formation.
 * Note: this is an arbitrary number - it could be anything... */
#define	MAXFORMATIONUNITS	16

/* Starting minor formation number for crews and special attached units */
#define	SPECIALUNITFSID		50

#endif	/* INTERNAL_SIZES_H */
