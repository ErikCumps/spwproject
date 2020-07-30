/** \file
 * The SPWaW Library - winSPWW2 gamefile handling.
 *
 * Copyright (C) 2019-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION37_WINSPWW2_H
#define	SECTION37_WINSPWW2_H	1

#include "gamefile/winspww2/defines_winspww2.h"

typedef struct s_WINSPWW2_VHEX {
	BYTE		x[WINSPWW2_VHEXCOUNT];		/* Victory hexes: x-coordinate					*/
	BYTE		y[WINSPWW2_VHEXCOUNT];		/* Victory hexes: y-coordinate					*/
	BYTE		amount[WINSPWW2_VHEXCOUNT];	/* Victory hexes: amount					*/
	BYTE		owner[WINSPWW2_VHEXCOUNT];	/* Victory hexes: owner	0=player1, 1=player2, 2=neutral		*/
} WINSPWW2_VHEX;

typedef struct s_WINSPWW2_GAME_DATA {
	char		__data00[2];
	BYTE		location_number;		/* Battle location number					*/
	char		__data01[1];
	BYTE		Mgame;				/* Battle date: month						*/
	BYTE		Ygame;				/* Battle date: year - 1900					*/
	BYTE		terrain;			/* Battle terrain type						*/
	BYTE		campaign;			/* Campaign mode flag??						*/
	BYTE		postload;			/* Post-load action?						*/
	BYTE		OOBc1;				/* Country1 OOB ID						*/
	BYTE		OOBc2;				/* Country2 OOB ID						*/
	BYTE		OOBc3;				/* Country3 OOB ID						*/
	BYTE		OOBc4;				/* Country4 OOB ID						*/
	BYTE		P1mission;			/* Player1 mission type						*/
	BYTE		P2mission;			/* Player2 mission type						*/
	BYTE		turn_max;			/* Battle max turns						*/
	BYTE		turn;				/* Battle current turn						*/
	WINSPWW2_VHEX	vhex;				/* Battle victory hexes						*/
	char		__data02[2];
	BYTE		vis;				/* Battle visibility						*/
	BYTE		wind;				/* Battle wind direction (blows to) <0..5=E,SE,SW,W,NW,NE>	*/
	char		__data03[4];
	SHORT		P1result;			/* Player1 battle result					*/
	SHORT		P2result;			/* Player2 battle result					*/
	char		__data04[2];
	USHORT		battles;			/* Campaign battles fought					*/
	USHORT		DDcount;			/* Campaign decisive defeats					*/
	char		__data05[2];
	USHORT		MDcount;			/* Campaign marginal defeats					*/
	char		__data06[2];
	USHORT		Dcount;				/* Campaign draws						*/
	char		__data07[2];
	USHORT		MVcount;			/* Campaign marginal victories					*/
	char		__data08[2];
	USHORT		DVcount;			/* Campaign decisive victories					*/
	char		__data09[2];
	char		location[WINSPWW2_AZSLOCATION];	/* Battle location/description (ASCIIZ)				*/
	USHORT		battles_max;			/* Campaign max battles						*/
	USHORT		Mstart;				/* Campaign start date: month					*/
	USHORT		Ystart;				/* Campaign start date: year - 1900				*/
	USHORT		Mend;				/* Campaign end date: month					*/
	USHORT		Yend;				/* Campaign end date: year - 1900				*/
	char		__data10[12];
	USHORT		P1TLmen;			/* P1 Total losses: men						*/
	USHORT		P2TLmen;			/* P2 Total losses: men						*/
	char		__data11[4];
	DWORD		P1TLafv;			/* P1 Total losses: AFV						*/
	DWORD		P1TLapc;			/* P1 Total losses: APC						*/
	DWORD		P1TLart;			/* P1 Total losses: ART						*/
	char		__data12[8];
	DWORD		P1TLsoft;			/* P1 Total losses: soft vehicles				*/
	DWORD		P2TLsoft;			/* P2 Total losses: soft vehicles				*/
	DWORD		P1TLairtpt;			/* P1 Total losses: air transport				*/
	DWORD		P1TLaircraft;			/* P1 Total losses: aircraft					*/
	char		__data13[4];
	DWORD		P2TLafv;			/* P2 Total losses: AFV						*/
	DWORD		P2TLapc;			/* P2 Total losses: APC						*/
	DWORD		P2TLart;			/* P2 Total losses: ART						*/
	char		__data14[12];
	DWORD		P2TLairtpt;			/* P2 Total losses: air transport				*/	
	DWORD		P2TLaircraft;			/* P2 Total losses: aircraft					*/
	char		__data15[4];
	DWORD		P1BLmen;			/* P1 Battle losses: men					*/
	DWORD		P2BLmen;			/* P2 Battle losses: men					*/
	char		__data16[4];
	DWORD		P1BLafv;			/* P1 Battle losses: AFV					*/
	DWORD		P1BLapc;			/* P1 Battle losses: APC					*/
	DWORD		P1BLart;			/* P1 Battle losses: ART					*/
	char		__data17[8];
	DWORD		P1BLsoft;			/* P1 Battle losses: soft vehicles				*/
	DWORD		P2BLsoft;			/* P2 Battle losses: soft vehicles				*/
	DWORD		P1BLairtpt;			/* P1 Battle losses: air transport?				*/
	DWORD		P1BLaircraft;			/* P1 Battle losses: aircraft?					*/
	char		__data18[4];
	DWORD		P2BLafv;			/* P2 Battle losses: AFV					*/
	DWORD		P2BLapc;			/* P2 Battle losses: APC					*/
	DWORD		P2BLart;			/* P2 Battle losses: ART					*/
	char		__data19[12];
	DWORD		P2BLairtpt;			/* P2 Battle losses: air transport?				*/
	DWORD		P2BLaircraft;			/* P2 Battle losses: aircraft?					*/
	char		__data20[4];
	USHORT		busy;				/* Inverted battle busy flag					*/
	char		__data21[2];
	DWORD		P1score;			/* P1 battle score						*/
	DWORD		P2score;			/* P2 battle score						*/
	char		__data22[135];
} WINSPWW2_GAME_DATA;

typedef struct s_WINSPWW2_SECTION37 {
	union u_u {
		char	raw[WINSPWW2_SIZESEC37];
		struct s_d {
			WINSPWW2_GAME_DATA	data;
		} d;
	} u;
} WINSPWW2_SECTION37;

#endif	/* SECTION37_WINSPWW2_H */
