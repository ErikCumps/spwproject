/** \file
 * The SPWaW Library - SPWaW OOB handling - raw winSPMBT data handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	RAW_WINSPMBT_H
#define	RAW_WINSPMBT_H	1

#include "spwoob/raw_winspmbt_api.h"

#define	SPWOOB_WCNT	250
#define	SPWOOB_UCNT	1000
#define	SPWOOB_FCNT	1000
#define	SPWOOB_FMECNT	10
#define	SPWOOB_EFSTART	1000

#pragma pack(push, r1, 1)

#define	OOBAZSNAME	15
typedef struct s_OOBNAME {
	char	data[OOBAZSNAME];
} OOBNAME;

#define	OOBAZSCMT	80 /*95*/
typedef struct s_OOBCMT {
	char	data[OOBAZSCMT];
} OOBCMT;

typedef struct s_RAWOOB_WINSPMBT {
	OOBCMT	cmt;
        struct s_w {
		OOBNAME	name		[SPWOOB_WCNT];
		BYTE	wclass		[SPWOOB_WCNT];
		BYTE	size		[SPWOOB_WCNT];
		BYTE	sound		[SPWOOB_WCNT];
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
		BYTE	__data28	[SPWOOB_WCNT];
		BYTE	__data29	[SPWOOB_WCNT];
		BYTE	__data30	[SPWOOB_WCNT];
		BYTE	__data31	[SPWOOB_WCNT];
		BYTE	__data32	[SPWOOB_WCNT];
		BYTE	__data33	[SPWOOB_WCNT];
		BYTE	__data34	[SPWOOB_WCNT];
		BYTE	__data35	[SPWOOB_WCNT];
		BYTE	__data36	[SPWOOB_WCNT];
		BYTE	__data37	[SPWOOB_WCNT];
		BYTE	__data38	[SPWOOB_WCNT];
		BYTE	__data39	[SPWOOB_WCNT];
	} w;
	struct s_u {
		OOBNAME	name		[SPWOOB_UCNT];
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
		BYTE	__data46	[SPWOOB_UCNT];
		BYTE	__data47	[SPWOOB_UCNT];
		BYTE	__data48	[SPWOOB_UCNT];
		BYTE	__data49	[SPWOOB_UCNT];
		BYTE	__data50	[SPWOOB_UCNT];
		BYTE	__data51	[SPWOOB_UCNT];
		BYTE	__data52	[SPWOOB_UCNT];
		BYTE	__data53	[SPWOOB_UCNT];
		BYTE	__data54	[SPWOOB_UCNT];
		BYTE	__data55	[SPWOOB_UCNT];
		BYTE	__data56	[SPWOOB_UCNT];
		BYTE	__data57	[SPWOOB_UCNT];
		BYTE	__data58	[SPWOOB_UCNT];
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
		BYTE	ewa		[SPWOOB_UCNT];	// TODO: consider adding to SPWOOB_DATA ?
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
		BYTE __data90		[SPWOOB_UCNT];
		USHORT	icon_winter	[SPWOOB_UCNT];
		USHORT	icon_desert	[SPWOOB_UCNT];
		BYTE	__data098	[SPWOOB_UCNT];
		BYTE	__data099	[SPWOOB_UCNT];
		BYTE	__data100	[SPWOOB_UCNT];
		BYTE	__data101	[SPWOOB_UCNT];
		BYTE	__data102	[SPWOOB_UCNT];
		BYTE	__data103	[SPWOOB_UCNT];
		BYTE	__data104	[SPWOOB_UCNT];
		BYTE	__data105	[SPWOOB_UCNT];
		BYTE	__data106	[SPWOOB_UCNT];
		BYTE	__data107	[SPWOOB_UCNT];
		BYTE	__data108	[SPWOOB_UCNT];
		BYTE	__data109	[SPWOOB_UCNT];
		BYTE	__data110	[SPWOOB_UCNT];
		BYTE	__data111	[SPWOOB_UCNT];
		BYTE	__data112	[SPWOOB_UCNT];
		BYTE	__data113	[SPWOOB_UCNT];
		BYTE	__data114	[SPWOOB_UCNT];
		BYTE	__data115	[SPWOOB_UCNT];
		BYTE	__data116	[SPWOOB_UCNT];
		BYTE	__data117	[SPWOOB_UCNT];
		BYTE	__data118	[SPWOOB_UCNT];
		BYTE	__data119	[SPWOOB_UCNT];
		BYTE	__data120	[SPWOOB_UCNT];
		BYTE	__data121	[SPWOOB_UCNT];
		BYTE	__data122	[SPWOOB_UCNT];
		BYTE	__data123	[SPWOOB_UCNT];
	} u;
	struct s_f {
		OOBNAME	name		[SPWOOB_FCNT];
		BYTE	nation		[SPWOOB_FCNT];
		BYTE	start_yr	[SPWOOB_FCNT];
		BYTE	end_yr		[SPWOOB_FCNT];
		BYTE	pscr		[SPWOOB_FCNT];
		BYTE	type		[SPWOOB_FCNT];
		struct s_urid {
			USHORT	dat[SPWOOB_FMECNT];
		}	urid		[SPWOOB_FCNT];
		BYTE	start_mo	[SPWOOB_FCNT];
		BYTE	end_mo		[SPWOOB_FCNT];
		SBYTE	exp_mod		[SPWOOB_FCNT];
		SBYTE	mor_mod		[SPWOOB_FCNT];
		BYTE	__data44	[SPWOOB_FCNT];
		BYTE	__data45	[SPWOOB_FCNT];
		BYTE	__data46	[SPWOOB_FCNT];
		BYTE	__data47	[SPWOOB_FCNT];
		BYTE	__data48	[SPWOOB_FCNT];
		BYTE	__data49	[SPWOOB_FCNT];
		BYTE	__data50	[SPWOOB_FCNT];
		BYTE	__data51	[SPWOOB_FCNT];
		BYTE	__data52	[SPWOOB_FCNT];
		BYTE	__data53	[SPWOOB_FCNT];
		BYTE	__data54	[SPWOOB_FCNT];
		BYTE	__data55	[SPWOOB_FCNT];
		BYTE	__data56	[SPWOOB_FCNT];
		BYTE	__data57	[SPWOOB_FCNT];
		BYTE	__data58	[SPWOOB_FCNT];
		BYTE	__data59	[SPWOOB_FCNT];
	} f;
} RAWOOB_WINSPMBT;

#pragma pack(pop, r1)

#endif	/* RAW_WINSPMBT_H */
