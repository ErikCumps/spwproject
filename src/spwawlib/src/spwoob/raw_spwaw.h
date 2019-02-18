/** \file
 * The SPWaW Library - SPWaW OOB handling - raw SPWaW data handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	RAW_SPWAW_H
#define	RAW_SPWAW_H	1

#include "spwoob/raw_spwaw_api.h"

#define	SPWOOB_WCNT	250
#define	SPWOOB_UCNT	250
#define	SPWOOB_FCNT	200
#define	SPWOOB_FMECNT	10
#define	SPWOOB_EFSTART	1000

#pragma pack(push, r1, 1)

#define	OOBAZSNAME	15
typedef struct s_NAME {
	char	data[OOBAZSNAME];
} NAME;

typedef struct s_RAWOOB {
        struct s_w {
		NAME	name		[SPWOOB_WCNT];
		BYTE	wclass		[SPWOOB_WCNT];
		BYTE	__data0		[SPWOOB_WCNT];		/* Sym.			*/
		BYTE	size		[SPWOOB_WCNT];
		BYTE	warhead		[SPWOOB_WCNT];
		struct s_pen {
			BYTE	HE;
			BYTE	AP;
		}	pen		[SPWOOB_WCNT];
		USHORT	kill		[SPWOOB_WCNT];
		BYTE	accuracy	[SPWOOB_WCNT];
		BYTE	rng_max		[SPWOOB_WCNT];
		BYTE	rng_APCR	[SPWOOB_WCNT];
		BYTE	pen_APCR	[SPWOOB_WCNT];
		BYTE	pen_HEAT	[SPWOOB_WCNT];
		USHORT	sound		[SPWOOB_WCNT];
		BYTE	__data2		[7*SPWOOB_WCNT];	/* x1 ... x7		*/
	} w;
	struct s_u {
		NAME	name		[SPWOOB_UCNT];
		BYTE	uclass		[SPWOOB_UCNT];
		BYTE	arm_SK		[SPWOOB_UCNT];
		BYTE	size		[SPWOOB_UCNT];
		BYTE	crew		[SPWOOB_UCNT];
		struct	s_wpn {
			BYTE	w1;
			BYTE	w2;
			BYTE	w3;
			BYTE	w4;

		}	wpn		[SPWOOB_UCNT];
		struct s_wpn_ammo {
			BYTE	HE1;
			BYTE	AP1;
			BYTE	HE2;
			BYTE	AP2;
			BYTE	HE3;
			BYTE	AP3;
			BYTE	HE4;
			BYTE	AP4;
		}	wpn_ammo	[SPWOOB_UCNT];
		BYTE	speed		[SPWOOB_UCNT];
		struct s_arm {
			BYTE	FH;
			BYTE	SH;
			BYTE	RH;
			BYTE	FT;
			BYTE	ST;
			BYTE	RT;
			BYTE	TP;
			BYTE	__data0	[7];
		}	arm		[SPWOOB_UCNT];
		BYTE	__data1		[7*SPWOOB_UCNT];	/* rct1 ... rct7	*/
		struct s_slp {
			BYTE	FH;
			BYTE	SH;
			BYTE	RH;
			BYTE	FT;
			BYTE	ST;
			BYTE	RT;
		}	slp		[SPWOOB_UCNT];
		BYTE	radio		[SPWOOB_UCNT];
		BYTE	start_yr	[SPWOOB_UCNT];
		BYTE	end_yr		[SPWOOB_UCNT];
		BYTE	cost		[SPWOOB_UCNT];
		BYTE	nation		[SPWOOB_UCNT];
		USHORT	lbm		[SPWOOB_UCNT];
		BYTE	irvis		[SPWOOB_UCNT];
		BYTE	fc		[SPWOOB_UCNT];
		BYTE	wpn1_APCRammo	[SPWOOB_UCNT];
		BYTE	wpn1_HEATammo	[SPWOOB_UCNT];
		BYTE	rof		[SPWOOB_UCNT];
		BYTE	stab		[SPWOOB_UCNT];
		BYTE	rf		[SPWOOB_UCNT];
		BYTE	__data2		[SPWOOB_UCNT];		/* ew			*/
		BYTE	load_cap	[SPWOOB_UCNT];
		BYTE	survive		[SPWOOB_UCNT];
		BYTE	load_cost	[SPWOOB_UCNT];
		BYTE	icon		[SPWOOB_UCNT];
		BYTE	movcl		[SPWOOB_UCNT];
		BYTE	swim		[SPWOOB_UCNT];
		BYTE	end_mo		[SPWOOB_UCNT];
		BYTE	smkdev		[SPWOOB_UCNT];
		BYTE	start_mo	[SPWOOB_UCNT];
		USHORT	sound		[SPWOOB_UCNT];
		BYTE	wpn1_snd	[SPWOOB_UCNT];
		BYTE	wpn2_snd	[SPWOOB_UCNT];
		BYTE	wpn3_snd	[SPWOOB_UCNT];
		BYTE	wpn4_snd	[SPWOOB_UCNT];
		BYTE	__data3		[2*SPWOOB_UCNT];	/* x1 ... x2		*/
		USHORT	msound		[SPWOOB_UCNT];
	} u;
	struct s_f {
		NAME	name		[SPWOOB_FCNT];
		BYTE	nation		[SPWOOB_FCNT];
		BYTE	start_mo	[SPWOOB_FCNT];
		BYTE	special		[SPWOOB_FCNT];
		BYTE	__data0		[8*SPWOOB_FCNT];	/* x1 ... x8		*/
		BYTE	start_yr	[SPWOOB_FCNT];
		BYTE	end_yr		[SPWOOB_FCNT];
		BYTE	purchscrn	[SPWOOB_FCNT];
		BYTE	type		[SPWOOB_FCNT];
		struct s_urid {
			USHORT	dat[SPWOOB_FMECNT];
		}	urid		[SPWOOB_FCNT];
		struct s_ucnt {
			BYTE	dat[SPWOOB_FMECNT];
		}	ucnt		[SPWOOB_FCNT];
	} f;
} RAWOOB;

#pragma pack(pop, r1)

#endif	/* RAW_SPWAW_H */
