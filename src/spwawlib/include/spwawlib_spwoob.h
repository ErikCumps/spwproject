/** \file
 * The SPWaW Library - SPWaW OOB API.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 *
 * \addtogroup API
 * @{
 */

#ifndef	SPWAW_LIB_OOB_H
#define	SPWAW_LIB_OOB_H	1

#include <spwawlib_api.h>
#include <spwawlib_defines.h>
#include <spwawlib_types.h>
#include <spwawlib_spwoob_types.h>

/* Forward declaration for convenience */
typedef struct s_SPWOOB SPWOOB;

/* Maximum number of supported OOBs */
#define	SPWOOB_DCNT	256

/* A signed byte type */
typedef signed char	SBYTE;

typedef struct s_SPWOOB_WDATA {
	bool		valid;
	char		name[SPWAW_AZSOOBNAME+1];
	SPWOOB_WCLASS	wclass;
	BYTE		size;
	BYTE		warhead;
	BYTE		kill_HE;
	BYTE		kill_AP;
	BYTE		pen_HE;
	BYTE		pen_AP;
	BYTE		pen_HEAT;
	BYTE		pen_APCR;
	BYTE		accuracy;
	BYTE		range_max;
	BYTE		range_APCR;
} SPWOOB_WDATA;

typedef struct s_SPWOOB_UDATA {
	bool		valid;
	BYTE		nation;
	char		name[SPWAW_AZSOOBNAME+1];
	SPWOOB_UTYPE	type;
	SPWOOB_UCLASS	uclass;
	SPWOOB_MOVCL	mclass;
	USHORT		start_yr;
	BYTE		start_mo;
	USHORT		end_yr;
	BYTE		end_mo;
	BYTE		size;
	BYTE		crew;
	BYTE		survive;
	USHORT		cost;
	BYTE		speed;
	BYTE		radio;
	BYTE		irvis;
	BYTE		fc;
	BYTE		rf;
	BYTE		stab;
	BYTE		rof;
	BYTE		load_cap;
	BYTE		load_cost;
	BYTE		swim;
	BYTE		smkdev;
	BYTE		wpn1;
	BYTE		wpn1_HEammo;
	BYTE		wpn1_APammo;
	BYTE		wpn1_HEATammo;
	BYTE		wpn1_APCRammo;
	BYTE		wpn2;
	BYTE		wpn2_HEammo;
	BYTE		wpn2_APammo;
	BYTE		wpn3;
	BYTE		wpn3_HEammo;
	BYTE		wpn3_APammo;
	BYTE		wpn4;
	BYTE		wpn4_HEammo;
	BYTE		wpn4_APammo;
	BYTE		arm_HE_FH;
	BYTE		arm_HE_SH;
	BYTE		arm_HE_RH;
	BYTE		arm_HE_FT;
	BYTE		arm_HE_ST;
	BYTE		arm_HE_RT;
	BYTE		arm_HE_TP;
	BYTE		arm_HE_SK;
	BYTE		arm_AP_FH;
	BYTE		arm_AP_SH;
	BYTE		arm_AP_RH;
	BYTE		arm_AP_FT;
	BYTE		arm_AP_ST;
	BYTE		arm_AP_RT;
	BYTE		arm_AP_TP;
	BYTE		arm_AP_SK;
	BYTE		slp_FH;
	BYTE		slp_SH;
	BYTE		slp_RH;
	BYTE		slp_FT;
	BYTE		slp_ST;
	BYTE		slp_RT;
	USHORT		sound;
	USHORT		lbm;
	USHORT		text;
	USHORT		icon;
	USHORT		icon_desert;
	USHORT		icon_winter;
} SPWOOB_UDATA;

typedef struct s_SPWOOB_EDATA {
	USHORT		rid;
	BYTE		cnt;
} SPWOOB_EDATA;

typedef struct s_SPWOOB_FDATA {
	bool		valid;
	BYTE		nation;
	char		name[SPWAW_AZSOOBNAME+1];
	SPWOOB_FSTAT	stat;
	SPWOOB_FTYPE	type;
	SPWOOB_FPSCR	pscr;
	USHORT		start_yr;
	BYTE		start_mo;
	USHORT		end_yr;
	BYTE		end_mo;
	SBYTE		exp_mod;
	SBYTE		mor_mod;
	SPWOOB_EDATA	*elements;
} SPWOOB_FDATA;

typedef struct s_SPWOOB_DATA {
	BYTE		id;
	ULONG		crc32;
	void		*rdata;
	ULONG		rsize;
	SPWOOB		*spwoob;
	USHORT		wcnt;
	SPWOOB_WDATA	*wdata;
	USHORT		ucnt;
	SPWOOB_UDATA	*udata;
	USHORT		fcnt;
	SPWOOB_FDATA	*fdata;
	USHORT		fmecnt;
	void		*edata;
	USHORT		efstart;
} SPWOOB_DATA;

typedef struct s_SPWOOB {
	SPWAW_GAME_TYPE	gametype;
	char		srcdir[MAX_PATH+1];
	BYTE		count;
	SPWOOB_DATA	*data[SPWOOB_DCNT];
} SPWOOB;



/*** API ***/
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_SPWOOB		(SPWOOB **oob);

extern SPWAWLIB_API SPWAW_ERROR		SPWAW_oob_data		(SPWOOB *oob, BYTE id, SPWOOB_DATA **data);

extern SPWAWLIB_API const char *	SPWAW_oob_nation	(SPWAW_GAME_TYPE gametype, BYTE id);
extern SPWAWLIB_API const char *	SPWAW_oob_people	(SPWAW_GAME_TYPE gametype, BYTE id);
extern SPWAWLIB_API const char *	SPWAW_oob_prefix	(SPWAW_GAME_TYPE gametype, BYTE id);

extern SPWAWLIB_API const char *	SPWAW_oob_uclass	(SPWOOB_UCLASS e);

extern SPWAWLIB_API SPWAW_ERROR		SPWAW_oob_dump		(SPWOOB *oob, char *base, bool raw);

#endif	/* SPWAW_LIB_OOB_H */

/** @} */
