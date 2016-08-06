#ifndef	INTERNAL_STRUCT37_H
#define	INTERNAL_STRUCT37_H	1

#include "gamefile/struct_common.h"

typedef struct s_VHEX {
	BYTE	x[VHEXCOUNT];		/* Victory hexes: x-coordinate					*/
	BYTE	y[VHEXCOUNT];		/* Victory hexes: y-coordinate					*/
	BYTE	amount[VHEXCOUNT];	/* Victory hexes: amount					*/
	BYTE	owner[VHEXCOUNT];	/* Victory hexes: owner	0=player1, 1=player2, 2=neutral		*/
} VHEX;

typedef struct s_GAME_DATA {
	char	__data00[3];
	BYTE	Mgame;				/* Game date: month			*/
	BYTE	Ygame;				/* Game date: year - 1900		*/
	BYTE	terrain;			/* Game terrain				*/
	BYTE	campaign;			/* Campaign mode flag??			*/
	char	__data01[1];
	BYTE	OOBc1;				/* Country1 OOB ID			*/
	BYTE	OOBc2;				/* Country2 OOB ID			*/
	BYTE	OOBc3;				/* Country3 OOB ID			*/
	BYTE	OOBc4;				/* Country4 OOB ID			*/
	BYTE	P1mission;			/* Player1 mission type			*/
	BYTE	P2mission;			/* Player2 mission type			*/
	BYTE	turn_max;			/* Game max turns			*/
	BYTE	turn;				/* Game current turn			*/
	VHEX	vhex;				/* Game victory hexes			*/
	char	__data02[2];
	BYTE	vis;				/* Game visibility			*/
	BYTE	wind;				/* Game wind direction (blows to) <0..5=E,SE,SW,W,NW,NE>	*/
	BYTE	weather;			/* Game weather type			*/
	BYTE	Dgame;				/* Game date: day?			*/
	BYTE	Hgame;				/* Game date: hour?			*/
	char	__data04[9];
	USHORT	battles;			/* Campaign battles fought		*/
	USHORT	majvic;				/* Campaign major victories		*/
	char	location[SPWAW_AZSLOCATION];	/* Game location/description (ASCIIZ)	*/
	USHORT	battles_max;			/* Campaign max battles			*/
	USHORT	Mstart;				/* Campaign start date: month		*/
	USHORT	Ystart;				/* Campaign start date: year - 1900	*/
	USHORT	Mend;				/* Campaign end date: month		*/
	USHORT	Yend;				/* Campaign end date: year - 1900	*/
	char	__data05[16];
	USHORT	P1TLmen;			/* P1 Total losses: men			*/
	USHORT	P2TLmen;			/* P2 Total losses: men			*/
	char	__data06[2];
	USHORT	P1TLafv;			/* P1 Total losses: AFV			*/
	USHORT	P1TLapc;			/* P1 Total losses: APC			*/
	USHORT	P1TLart;			/* P1 Total losses: ART			*/
	char	__data07[4];
	USHORT	P1TLsoft;			/* P1 Total losses: soft vehicles	*/
	USHORT	P2TLsoft;			/* P2 Total losses: soft vehicles	*/
	char	__data08[6];
	USHORT	P2TLafv;			/* P2 Total losses: AFV			*/
	USHORT	P2TLapc;			/* P2 Total losses: APC			*/
	USHORT	P2TLart;			/* P2 Total losses: ART			*/
	char	__data09[32];
	USHORT	P1BLmen;			/* P1 Battle losses: men		*/
	USHORT	P2BLmen;			/* P2 Battle losses: men		*/
	char	__data10[2];
	USHORT	P1BLafv;			/* P1 Battle losses: AFV		*/
	USHORT	P1BLapc;			/* P1 Battle losses: APC		*/
	USHORT	P1BLart;			/* P1 Battle losses: ART		*/
	char	__data11[4];
	USHORT	P1BLsoft;			/* P1 Battle losses: soft vehicles	*/
	USHORT	P2BLsoft;			/* P2 Battle losses: soft vehicles	*/
	char	__data12[6];
	USHORT	P2BLafv;			/* P2 Battle losses: AFV		*/
	USHORT	P2BLapc;			/* P2 Battle losses: APC		*/
	USHORT	P2BLart;			/* P2 Battle losses: ART		*/
	char	__data13[12];
	USHORT	busy;				/* Inverted battle busy flag		*/
	char	__data14[4];
	DWORD	P1score;			/* P1 battle score			*/
	DWORD	P2score;			/* P2 battle score			*/
	char	__data15[898];
} GAME_DATA;

typedef struct s_STRUCT37 {
	union {
		char	raw[SIZESEC37];
		struct {
			GAME_DATA	data;
		} d;
	} u;
} STRUCT37;

extern SPWAW_ERROR	sec37_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

#endif	/* INTERNAL_STRUCT37_H */
