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
		NAME	name		[SPWOOB_WCNT];
		BYTE	wclass		[SPWOOB_WCNT];
		BYTE	size		[SPWOOB_WCNT];
		BYTE	__data02	[SPWOOB_WCNT];		/* ???			*/
		BYTE	warhead		[SPWOOB_WCNT];
		struct s_pen {
			BYTE	HE;
			BYTE	AP;
		}	pen		[SPWOOB_WCNT];
		struct s_kill {
			BYTE	HE;
			BYTE	AP;
		}	kill		[SPWOOB_WCNT];
		BYTE	accuracy	[SPWOOB_WCNT];
		BYTE	rng_max		[SPWOOB_WCNT];
		BYTE	rng_APCR	[SPWOOB_WCNT];
		BYTE	pen_APCR	[SPWOOB_WCNT];
		BYTE	pen_HEAT	[SPWOOB_WCNT];
		BYTE	__dataX		[12*SPWOOB_WCNT];	/* X1 ... X12		*/
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
			BYTE	AP_FH;
			BYTE	AP_SH;
			BYTE	AP_RH;
			BYTE	AP_FT;
			BYTE	AP_ST;
			BYTE	AP_RT;
			BYTE	AP_TP;
			BYTE	HE_FH;
			BYTE	HE_SH;
			BYTE	HE_RH;
			BYTE	HE_FT;
			BYTE	HE_ST;
			BYTE	HE_RT;
			BYTE	HE_TP;
		}	arm		[SPWOOB_UCNT];
		BYTE	__data31	[SPWOOB_UCNT];		/* ???			*/
		BYTE	__data32	[SPWOOB_UCNT];		/* ???			*/
		BYTE	__data33	[SPWOOB_UCNT];		/* ???			*/
		BYTE	__data34	[SPWOOB_UCNT];		/* ???			*/
		BYTE	__data35	[SPWOOB_UCNT];		/* ???			*/
		BYTE	__data36	[SPWOOB_UCNT];		/* ???			*/
		BYTE	__data37	[SPWOOB_UCNT];		/* ???			*/
		BYTE	__data38	[SPWOOB_UCNT];		/* ???			*/
		BYTE	__data39	[SPWOOB_UCNT];		/* ???			*/
		BYTE	__data40	[SPWOOB_UCNT];		/* ???			*/
		BYTE	__data41	[SPWOOB_UCNT];		/* ???			*/
		BYTE	__data42	[SPWOOB_UCNT];		/* ???			*/
		BYTE	__data43	[SPWOOB_UCNT];		/* ???			*/
		BYTE	radio		[SPWOOB_UCNT];
		BYTE	start_yr	[SPWOOB_UCNT];
		BYTE	end_yr		[SPWOOB_UCNT];
		USHORT	cost		[SPWOOB_UCNT];
		BYTE	nation		[SPWOOB_UCNT];
		USHORT	lbm		[SPWOOB_UCNT];
		BYTE	irvis		[SPWOOB_UCNT];
		BYTE	fc		[SPWOOB_UCNT];
		BYTE	wpn1_APCRammo	[SPWOOB_UCNT];
		BYTE	wpn1_HEATammo	[SPWOOB_UCNT];
		BYTE	rof		[SPWOOB_UCNT];
		BYTE	stab		[SPWOOB_UCNT];
		BYTE	rf		[SPWOOB_UCNT];
		BYTE	__data59	[SPWOOB_UCNT];		/* ???			*/
		BYTE	load_cap	[SPWOOB_UCNT];
		BYTE	survive		[SPWOOB_UCNT];
		BYTE	load_cost	[SPWOOB_UCNT];
		USHORT	icon		[SPWOOB_UCNT];
		BYTE	swim		[SPWOOB_UCNT];
		BYTE	movcl		[SPWOOB_UCNT];
		BYTE	smkdev		[SPWOOB_UCNT];
		BYTE	start_mo	[SPWOOB_UCNT];
		BYTE	end_mo		[SPWOOB_UCNT];
		BYTE	sound		[SPWOOB_UCNT];
		struct	s_wpn_snd {
			BYTE	s1;
			BYTE	s2;
			BYTE	s3;
			BYTE	s4;
		}	wpn_snd		[SPWOOB_UCNT];
		BYTE __data75		[SPWOOB_UCNT];		/* ???			*/
		USHORT	icon_winter	[SPWOOB_UCNT];
		USHORT	icon_desert	[SPWOOB_UCNT];
		BYTE	__dataX		[26*SPWOOB_UCNT];	/* X1 .. X26		*/
	} u;
	struct s_f {
		NAME	name		[SPWOOB_FCNT];
		BYTE	nation		[SPWOOB_FCNT];
		BYTE	start_yr	[SPWOOB_FCNT];
		BYTE	end_yr		[SPWOOB_FCNT];
		BYTE	pscr		[SPWOOB_FCNT];
		BYTE	type		[SPWOOB_FCNT];
		struct s_urid {
			USHORT	dat[SPWOOB_FMUCNT];
		}	urid		[SPWOOB_FCNT];
		BYTE	start_mo	[SPWOOB_FCNT];
		BYTE	end_mo		[SPWOOB_FCNT];
		BYTE	exp_mod		[SPWOOB_FCNT];
		BYTE	mor_mod		[SPWOOB_FCNT];
		BYTE	__dataX		[16*SPWOOB_FCNT];	/* X1 ... X16		*/
	} f;
} RAWOOB;

#pragma pack(pop, r1)

extern void	spwoob_dump_raw_data	(RAWOOB *raw, BYTE id, char *base);

#endif	/* RAW_H */
