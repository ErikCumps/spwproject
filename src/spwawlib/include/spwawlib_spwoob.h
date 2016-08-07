/** \file
 * The SPWaW Library - SPWaW OOB API.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
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
#include <spwawlib_spwoob_types.h>

#define	SPWOOB_WCNT	250
#define	SPWOOB_UCNT	250
#define	SPWOOB_FCNT	200
#define	SPWOOB_FMUCNT	10
#define	SPWOOB_DCNT	256

typedef struct s_SPWOOB_WDATA {
	bool		valid;
	char		name[SPWAW_AZSOOBNAME+1];
	SPWOOB_WCLASS	wclass;
	BYTE		size;
	BYTE		warhead;
	USHORT		kill;
	BYTE		pen_AP;
	BYTE		pen_HE;
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
	USHORT		start_yr;
	BYTE		start_mo;
	USHORT		end_yr;
	BYTE		end_mo;
	BYTE		size;
	BYTE		crew;
	BYTE		survive;
	BYTE		cost;
	BYTE		speed;
	SPWOOB_MOVCL	mclass;
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
	BYTE		arm_FH;
	BYTE		arm_SH;
	BYTE		arm_RH;
	BYTE		arm_FT;
	BYTE		arm_ST;
	BYTE		arm_RT;
	BYTE		arm_TP;
	BYTE		arm_SK;
	BYTE		slp_FH;
	BYTE		slp_SH;
	BYTE		slp_RH;
	BYTE		slp_FT;
	BYTE		slp_ST;
	BYTE		slp_RT;
} SPWOOB_UDATA;

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
	USHORT		unit_ids[10];
	BYTE		unit_cnt[10];
} SPWOOB_FDATA;

typedef struct s_SPWOOB_DATA {
	BYTE		id;
	ULONG		crc32;
	SPWOOB_WDATA	wdata[SPWOOB_WCNT];
	SPWOOB_UDATA	udata[SPWOOB_UCNT];
	SPWOOB_FDATA	fdata[SPWOOB_FCNT];
	void		*rdata;
	ULONG		rsize;
} SPWOOB_DATA;

typedef struct s_SPWOOB {
	char		srcdir[MAX_PATH+1];
	BYTE		count;
	SPWOOB_DATA	*data[SPWOOB_DCNT];
} SPWOOB;



/*** API ***/
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_oob_data		(SPWOOB *oob, BYTE id, SPWOOB_DATA **data);
extern SPWAWLIB_API const char *	SPWAW_oob_nation	(BYTE id);
extern SPWAWLIB_API const char *	SPWAW_oob_people	(BYTE id);
extern SPWAWLIB_API const char *	SPWAW_oob_prefix	(BYTE id);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_oob_dump		(SPWOOB *oob, char *base);

extern SPWAWLIB_API const char *	SPWAW_oob_uclass	(SPWOOB_UCLASS e);

#endif	/* SPWAW_LIB_OOB_H */

/** @} */
