/** \file
 * The SPWaW Library - SPWaW OOB handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	RAW_H
#define	RAW_H	1

#include <spwawlib_spwoob.h>

#pragma pack(push, r1, 1)

#define	OOBAZSNAME	15
typedef struct s_NAME {
	char	data[OOBAZSNAME];
} NAME;

#define	OOBAZSCMT	80 /*95*/
typedef struct s_CMT {
	char	data[OOBAZSCMT];
} CMT;

typedef struct s_RAWOOB {
	CMT	cmt;
        struct s_w {
		//NAME	name		[SPWOOB_WCNT-1];
		NAME	name		[SPWOOB_WCNT];
		//BYTE	wclass		[SPWOOB_WCNT];
		////BYTE	__data0		[SPWOOB_WCNT];		/* Sym.			*/
		//BYTE	size		[SPWOOB_WCNT];
		//BYTE	warhead		[SPWOOB_WCNT];
		//struct s_pen {
		//	BYTE	HE;
		//	BYTE	AP;
		//}	pen		[SPWOOB_WCNT];
		//USHORT	kill		[SPWOOB_WCNT];
		//BYTE	accuracy	[SPWOOB_WCNT];
		//BYTE	rng_max		[SPWOOB_WCNT];
		//BYTE	rng_APCR	[SPWOOB_WCNT];
		//BYTE	pen_APCR	[SPWOOB_WCNT];
		//BYTE	pen_HEAT	[SPWOOB_WCNT];
		//USHORT	sound		[SPWOOB_WCNT];
		BYTE __data00		[SPWOOB_WCNT];
		BYTE __data01		[SPWOOB_WCNT];
		BYTE __data02		[SPWOOB_WCNT];
		BYTE __data03		[SPWOOB_WCNT];
		BYTE __data04		[SPWOOB_WCNT];
		BYTE __data05		[SPWOOB_WCNT];
		BYTE __data06		[SPWOOB_WCNT];
		BYTE __data07		[SPWOOB_WCNT];
		BYTE __data08		[SPWOOB_WCNT];
		BYTE __data09		[SPWOOB_WCNT];
		BYTE __data10		[SPWOOB_WCNT];
		BYTE __data11		[SPWOOB_WCNT];
		BYTE __data12		[SPWOOB_WCNT];
		BYTE __data13		[SPWOOB_WCNT];
		//BYTE	__data2		[7*SPWOOB_WCNT];	/* x1 ... x7		*/
		BYTE	__datax		[11*SPWOOB_WCNT];	/* x1 ... x11		*/
	} w;
	//BYTE	__padding0		[15];
	struct s_u {
		//NAME	name		[SPWOOB_UCNT-1];
		NAME	name		[SPWOOB_UCNT];
		//BYTE	uclass		[SPWOOB_UCNT];
		//BYTE	arm_SK		[SPWOOB_UCNT];
		//BYTE	size		[SPWOOB_UCNT];
		//BYTE	crew		[SPWOOB_UCNT];
		//struct	s_wpn {
		//	BYTE	w1;
		//	BYTE	w2;
		//	BYTE	w3;
		//	BYTE	w4;

		//}	wpn		[SPWOOB_UCNT];
		//struct s_wpn_ammo {
		//	BYTE	HE1;
		//	BYTE	AP1;
		//	BYTE	HE2;
		//	BYTE	AP2;
		//	BYTE	HE3;
		//	BYTE	AP3;
		//	BYTE	HE4;
		//	BYTE	AP4;
		//}	wpn_ammo	[SPWOOB_UCNT];
		//BYTE	speed		[SPWOOB_UCNT];
		//struct s_arm {
		//	BYTE	FH;
		//	BYTE	SH;
		//	BYTE	RH;
		//	BYTE	FT;
		//	BYTE	ST;
		//	BYTE	RT;
		//	BYTE	TP;
		//	BYTE	__data0	[7];
		//}	arm		[SPWOOB_UCNT];
		//BYTE	__data1		[7*SPWOOB_UCNT];	/* rct1 ... rct7	*/
		//struct s_slp {
		//	BYTE	FH;
		//	BYTE	SH;
		//	BYTE	RH;
		//	BYTE	FT;
		//	BYTE	ST;
		//	BYTE	RT;
		//}	slp		[SPWOOB_UCNT];
		//BYTE	radio		[SPWOOB_UCNT];
		//BYTE	start_yr	[SPWOOB_UCNT];
		//BYTE	end_yr		[SPWOOB_UCNT];
		//BYTE	cost		[SPWOOB_UCNT];
		//BYTE	nation		[SPWOOB_UCNT];
		//USHORT	lbm		[SPWOOB_UCNT];
		//BYTE	irvis		[SPWOOB_UCNT];
		//BYTE	fc		[SPWOOB_UCNT];
		//BYTE	wpn1_APCRammo	[SPWOOB_UCNT];
		//BYTE	wpn1_HEATammo	[SPWOOB_UCNT];
		//BYTE	rof		[SPWOOB_UCNT];
		//BYTE	stab		[SPWOOB_UCNT];
		//BYTE	rf		[SPWOOB_UCNT];
		//BYTE	__data2		[SPWOOB_UCNT];		/* ew			*/
		//BYTE	load_cap	[SPWOOB_UCNT];
		//BYTE	survive		[SPWOOB_UCNT];
		//BYTE	load_cost	[SPWOOB_UCNT];
		//BYTE	icon		[SPWOOB_UCNT];
		//BYTE	movcl		[SPWOOB_UCNT];
		//BYTE	swim		[SPWOOB_UCNT];
		//BYTE	end_mo		[SPWOOB_UCNT];
		//BYTE	smkdev		[SPWOOB_UCNT];
		//BYTE	start_mo	[SPWOOB_UCNT];
		//USHORT	sound		[SPWOOB_UCNT];
		//BYTE	wpn1_snd	[SPWOOB_UCNT];
		//BYTE	wpn2_snd	[SPWOOB_UCNT];
		//BYTE	wpn3_snd	[SPWOOB_UCNT];
		//BYTE	wpn4_snd	[SPWOOB_UCNT];
		//BYTE	__data3		[2*SPWOOB_UCNT];	/* x1 ... x2		*/
		//USHORT	msound		[SPWOOB_UCNT];
		//BYTE	__data4		[28*SPWOOB_UCNT];	/* z1 ... z28		*/
		BYTE __data00		[SPWOOB_UCNT];
		BYTE __data01		[SPWOOB_UCNT];
		BYTE __data02		[SPWOOB_UCNT];
		BYTE __data03		[SPWOOB_UCNT];
		BYTE __data04		[SPWOOB_UCNT];
		BYTE __data05		[SPWOOB_UCNT];
		BYTE __data06		[SPWOOB_UCNT];
		BYTE __data07		[SPWOOB_UCNT];
		BYTE __data08		[SPWOOB_UCNT];
		BYTE __data09		[SPWOOB_UCNT];
		BYTE __data10		[SPWOOB_UCNT];
		BYTE __data11		[SPWOOB_UCNT];
		BYTE __data12		[SPWOOB_UCNT];
		BYTE __data13		[SPWOOB_UCNT];
		BYTE __data14		[SPWOOB_UCNT];
		BYTE __data15		[SPWOOB_UCNT];
		BYTE __data16		[SPWOOB_UCNT];
		BYTE __data17		[SPWOOB_UCNT];
		BYTE __data18		[SPWOOB_UCNT];
		BYTE __data19		[SPWOOB_UCNT];
		BYTE __data20		[SPWOOB_UCNT];
		BYTE __data21		[SPWOOB_UCNT];
		BYTE __data22		[SPWOOB_UCNT];
		BYTE __data23		[SPWOOB_UCNT];
		BYTE __data24		[SPWOOB_UCNT];
		BYTE __data25		[SPWOOB_UCNT];
		BYTE __data26		[SPWOOB_UCNT];
		BYTE __data27		[SPWOOB_UCNT];
		BYTE __data28		[SPWOOB_UCNT];
		BYTE __data29		[SPWOOB_UCNT];
		BYTE __data30		[SPWOOB_UCNT];
		BYTE __data31		[SPWOOB_UCNT];
		BYTE __data32		[SPWOOB_UCNT];
		BYTE __data33		[SPWOOB_UCNT];
		BYTE __data34		[SPWOOB_UCNT];
		BYTE __data35		[SPWOOB_UCNT];
		BYTE __data36		[SPWOOB_UCNT];
		BYTE __data37		[SPWOOB_UCNT];
		BYTE __data38		[SPWOOB_UCNT];
		BYTE __data39		[SPWOOB_UCNT];
		BYTE __data40		[SPWOOB_UCNT];
		BYTE __data41		[SPWOOB_UCNT];
		BYTE __data42		[SPWOOB_UCNT];
		BYTE __data43		[SPWOOB_UCNT];
		BYTE __data44		[SPWOOB_UCNT];
		BYTE __data45		[SPWOOB_UCNT];
		BYTE __data46		[SPWOOB_UCNT];
		BYTE __data47		[SPWOOB_UCNT];
		BYTE __data48		[SPWOOB_UCNT];
		BYTE __data49		[SPWOOB_UCNT];
		BYTE __data50		[SPWOOB_UCNT];
		BYTE __data51		[SPWOOB_UCNT];
		BYTE __data52		[SPWOOB_UCNT];
		BYTE __data53		[SPWOOB_UCNT];
		BYTE __data54		[SPWOOB_UCNT];
		BYTE __data55		[SPWOOB_UCNT];
		BYTE __data56		[SPWOOB_UCNT];
		BYTE __data57		[SPWOOB_UCNT];
		BYTE __data58		[SPWOOB_UCNT];
		BYTE __data59		[SPWOOB_UCNT];
		BYTE __data60		[SPWOOB_UCNT];
		BYTE __data61		[SPWOOB_UCNT];
		BYTE __data62		[SPWOOB_UCNT];
		BYTE __data63		[SPWOOB_UCNT];
		BYTE __data64		[SPWOOB_UCNT];
		BYTE __data65		[SPWOOB_UCNT];
		BYTE __data66		[SPWOOB_UCNT];
		BYTE __data67		[SPWOOB_UCNT];
		BYTE __data68		[SPWOOB_UCNT];
		BYTE __data69		[SPWOOB_UCNT];
		BYTE __data70		[SPWOOB_UCNT];
		BYTE __data71		[SPWOOB_UCNT];
		BYTE __data72		[SPWOOB_UCNT];
		BYTE __data73		[SPWOOB_UCNT];
		BYTE __data74		[SPWOOB_UCNT];
		BYTE __data75		[SPWOOB_UCNT];
		BYTE __data76		[SPWOOB_UCNT];
		BYTE __data77		[SPWOOB_UCNT];
		BYTE __data78		[SPWOOB_UCNT];
		BYTE __data79		[SPWOOB_UCNT];
		BYTE __data80		[SPWOOB_UCNT];
		BYTE __data81		[SPWOOB_UCNT];
		BYTE __data82		[SPWOOB_UCNT];
		BYTE __data83		[SPWOOB_UCNT];
		BYTE __data84		[SPWOOB_UCNT];
		BYTE __data85		[SPWOOB_UCNT];
		BYTE __data86		[SPWOOB_UCNT];
		BYTE __data87		[SPWOOB_UCNT];
		BYTE __data88		[SPWOOB_UCNT];
		BYTE __data89		[SPWOOB_UCNT];
		BYTE __data90		[SPWOOB_UCNT];
		BYTE __data91		[SPWOOB_UCNT];
		BYTE __data92		[SPWOOB_UCNT];
		BYTE __data93		[SPWOOB_UCNT];
		BYTE __data94		[SPWOOB_UCNT];
		BYTE __data95		[SPWOOB_UCNT];
		BYTE __data96		[SPWOOB_UCNT];
		BYTE __data97		[SPWOOB_UCNT];
		BYTE __data98		[SPWOOB_UCNT];
		BYTE __data99		[SPWOOB_UCNT];
		BYTE __data100		[SPWOOB_UCNT];
		BYTE __data101		[SPWOOB_UCNT];
		BYTE __data102		[SPWOOB_UCNT];
		BYTE __data103		[SPWOOB_UCNT];
		BYTE __data104		[SPWOOB_UCNT];
		BYTE __data105		[SPWOOB_UCNT];
	} u;
	//BYTE	__padding1		[15];
	struct s_f {
		//NAME	name		[SPWOOB_FCNT-1];
		NAME	name		[SPWOOB_FCNT];
		//BYTE	nation		[SPWOOB_FCNT];
		//BYTE	start_mo	[SPWOOB_FCNT];
		//BYTE	special		[SPWOOB_FCNT];
		//BYTE	__data0		[8*SPWOOB_FCNT];	/* x1 ... x8		*/
		//BYTE	start_yr	[SPWOOB_FCNT];
		//BYTE	end_yr		[SPWOOB_FCNT];
		//BYTE	purchscrn	[SPWOOB_FCNT];
		//BYTE	type		[SPWOOB_FCNT];
		BYTE __data00		[SPWOOB_FCNT];
		BYTE __data01		[SPWOOB_FCNT];
		BYTE __data02		[SPWOOB_FCNT];
		BYTE __data03		[SPWOOB_FCNT];
		BYTE __data04		[SPWOOB_FCNT];
		BYTE __data05		[SPWOOB_FCNT];
		BYTE __data06		[SPWOOB_FCNT];
		BYTE __data07		[SPWOOB_FCNT];
		BYTE __data08		[SPWOOB_FCNT];
		BYTE __data09		[SPWOOB_FCNT];
		BYTE __data10		[SPWOOB_FCNT];
		BYTE __data11		[SPWOOB_FCNT];
		BYTE __data12		[SPWOOB_FCNT];
		BYTE __data13		[SPWOOB_FCNT];
		BYTE __data14		[SPWOOB_FCNT];
		struct s_urid {
			USHORT	data[SPWOOB_FMUCNT];
		}	urid		[SPWOOB_FCNT];
		struct s_ucnt {
			BYTE	data[SPWOOB_FMUCNT];
		}	ucnt		[SPWOOB_FCNT];
	} f;
} RAWOOB;

#pragma pack(pop, r1)

extern void	spwoob_dump_raw_data	(RAWOOB *raw, BYTE id, char *base);

#endif	/* RAW_H */
