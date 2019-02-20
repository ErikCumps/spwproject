/** \file
 * The SPWaW Library - SPWaW OOB handling - raw SPWaW data handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "spwoob/raw_spwaw.h"
#include "spwoob/raw.h"
#include "common/internal.h"
#include "utils/compression.h"

static BYTE
spwaw_name2id (const char *name)
{
	char	local[16];
	int	id;
	int	rc;

	if (strlen (name) > 6) return (SPWOOB_BADOOBID);
	if (strnicmp (name, "oob", 3) != 0) return (SPWOOB_BADOOBID);

	memset (local, 0, sizeof (local));
	snprintf (local, sizeof (local) - 1, "%s", name);

	local[0] = (char)tolower (local[0]);
	local[1] = (char)tolower (local[1]);
	local[2] = (char)tolower (local[2]);

	rc = sscanf (local, "oob%d", &id);
	if (rc <= 0) id = SPWOOB_BADOOBID;
	if ((id < 0) || (id > 255)) id = SPWOOB_BADOOBID;

	return ((BYTE)id);
}

static SPWOOB_UCLASS
spwaw_utype2class (SPWOOB_UTYPE ut)
{
	SPWOOB_UCLASS	uc = SPWOOB_UCLASS_OTH;

	switch (ut) {
		case SPWOOB_UTYPE_INF:
		case SPWOOB_UTYPE_LT_INF:
		case SPWOOB_UTYPE_MD_INF:
		case SPWOOB_UTYPE_HV_INF:
		case SPWOOB_UTYPE_MOT_INF:
		case SPWOOB_UTYPE_ARM_INF:
		case SPWOOB_UTYPE_ENG:
		case SPWOOB_UTYPE_MOT_ENG:
		case SPWOOB_UTYPE_ARM_ENG:
		case SPWOOB_UTYPE_MG:
		case SPWOOB_UTYPE_HV_MG:
		case SPWOOB_UTYPE_SKI:
		case SPWOOB_UTYPE_AIR_INF:
		case SPWOOB_UTYPE_BCYCLE:
		case SPWOOB_UTYPE_ELITE:
		case SPWOOB_UTYPE_SNIPER:
		case SPWOOB_UTYPE_SPECOP:
		case SPWOOB_UTYPE_GUERILLA:
		case SPWOOB_UTYPE_CONSCR:
		case SPWOOB_UTYPE_SLINF:
			uc = SPWOOB_UCLASS_INF;
			break;
		case SPWOOB_UTYPE_CAV:
			uc = SPWOOB_UCLASS_CAV;
			break;
		case SPWOOB_UTYPE_AHT:
		case SPWOOB_UTYPE_LT_TNK:
		case SPWOOB_UTYPE_MD_TNK:
		case SPWOOB_UTYPE_HV_TNK:
		case SPWOOB_UTYPE_VHV_TNK:
		case SPWOOB_UTYPE_MB_TNK:
		case SPWOOB_UTYPE_CR_TNK:
		case SPWOOB_UTYPE_CS_TNK:
		case SPWOOB_UTYPE_INF_TNK:
		case SPWOOB_UTYPE_MINECLR_TNK:
		case SPWOOB_UTYPE_ENG_TNK:
		case SPWOOB_UTYPE_FLM_TNK:
		case SPWOOB_UTYPE_SPEC_TNK:
		case SPWOOB_UTYPE_LL_TNK:
		case SPWOOB_UTYPE_CAPT_TNK:
		case SPWOOB_UTYPE_ASSAULT:
			uc = SPWOOB_UCLASS_ARM;
			break;
		case SPWOOB_UTYPE_HT:
		case SPWOOB_UTYPE_WH_APC:
		case SPWOOB_UTYPE_TR_APC:
		case SPWOOB_UTYPE_HV_TR_APC:
			uc = SPWOOB_UCLASS_APC;
			break;
		case SPWOOB_UTYPE_LT_MORT:
		case SPWOOB_UTYPE_MORT:
		case SPWOOB_UTYPE_HV_MORT:
		case SPWOOB_UTYPE_SP_MORT:
		case SPWOOB_UTYPE_SP_GUN:
		case SPWOOB_UTYPE_INF_GUN:
		case SPWOOB_UTYPE_HWTZR:
		case SPWOOB_UTYPE_SP_ART:
		case SPWOOB_UTYPE_OB_ART:
		case SPWOOB_UTYPE_RKT:
		case SPWOOB_UTYPE_SP_RKT:
		case SPWOOB_UTYPE_OB_RKT:
		case SPWOOB_UTYPE_CAPT_SPG:
			uc = SPWOOB_UCLASS_ART;
			break;
		case SPWOOB_UTYPE_INF_AT:
		case SPWOOB_UTYPE_AT:
		case SPWOOB_UTYPE_WH_AT:
		case SPWOOB_UTYPE_SP_AT:
		case SPWOOB_UTYPE_TD:
		case SPWOOB_UTYPE_HV_TD:
		case SPWOOB_UTYPE_TTD:
			uc = SPWOOB_UCLASS_AT;
			break;
		case SPWOOB_UTYPE_FLAK:
		case SPWOOB_UTYPE_HV_FLAK:
		case SPWOOB_UTYPE_WH_FLAK:
		case SPWOOB_UTYPE_SP_FLAK:
			uc = SPWOOB_UCLASS_AA;
			break;
		case SPWOOB_UTYPE_AMPHIB:
		case SPWOOB_UTYPE_HV_AMPHIB:
		case SPWOOB_UTYPE_AMPHIB_TNK:
		case SPWOOB_UTYPE_MRNS:
			uc = SPWOOB_UCLASS_MAR;
			break;
		case SPWOOB_UTYPE_SCOUT:
		case SPWOOB_UTYPE_VEH_SCOUT:
		case SPWOOB_UTYPE_APC_SCOUT:
		case SPWOOB_UTYPE_FO:
		case SPWOOB_UTYPE_VEH_FO:
		case SPWOOB_UTYPE_TR_FO:
		case SPWOOB_UTYPE_MCYCLE:
		case SPWOOB_UTYPE_RHT:
		case SPWOOB_UTYPE_LT_ARMCAR:
		case SPWOOB_UTYPE_ARMCAR:
		case SPWOOB_UTYPE_HV_ARMCAR:
		case SPWOOB_UTYPE_RE_TNK:
			uc = SPWOOB_UCLASS_REC;
			break;
		case SPWOOB_UTYPE_HQ:
		case SPWOOB_UTYPE_PLT_HQ:
		case SPWOOB_UTYPE_CC_TNK:
			uc = SPWOOB_UCLASS_CC;
			break;
		case SPWOOB_UTYPE_PRMV:
		case SPWOOB_UTYPE_MD_TRK:
		case SPWOOB_UTYPE_HV_TRK:
		case SPWOOB_UTYPE_BCARR:
		case SPWOOB_UTYPE_WAGON:
		case SPWOOB_UTYPE_TEAM:
		case SPWOOB_UTYPE_GTEAM:
		case SPWOOB_UTYPE_PACK:
			uc = SPWOOB_UCLASS_TRN;
			break;
		case SPWOOB_UTYPE_AIR:
		case SPWOOB_UTYPE_AIRB:
		case SPWOOB_UTYPE_CRG_AIR:
		case SPWOOB_UTYPE_GLIDER:
		case SPWOOB_UTYPE_HELI:
		case SPWOOB_UTYPE_AHELI:
		case SPWOOB_UTYPE_OHELI:
			uc = SPWOOB_UCLASS_AIR;
			break;
		case SPWOOB_UTYPE_BARGE:
		case SPWOOB_UTYPE_ABOAT:
		case SPWOOB_UTYPE_LCS:
		case SPWOOB_UTYPE_LCG:
		case SPWOOB_UTYPE_SHIP:
		case SPWOOB_UTYPE_NAVB:
			uc = SPWOOB_UCLASS_NVY;
			break;
		case SPWOOB_UTYPE_FORT:
		case SPWOOB_UTYPE_EMP_FORT:
		case SPWOOB_UTYPE_NAV_FORT:
			uc = SPWOOB_UCLASS_FRT;
			break;
		case SPWOOB_UTYPE_UTIL:
		case SPWOOB_UTYPE_CREW:
		case SPWOOB_UTYPE_AMMOC:
		case SPWOOB_UTYPE_AMMOD:
		case SPWOOB_UTYPE_CAVE:
		default:
			uc = SPWOOB_UCLASS_OTH;
			break;
	}
	return (uc);
}

SPWAW_ERROR
spwoob_load_raw_spwaw_data (SPWOOB_DATA *dst)
{
	SPWAW_ERROR	rc;
	RAWOOB		*raw;
	int		i, j;

	CNULLARG (dst); CNULLARG (dst->rdata);

	rc = zbuildcrc ((char *)dst->rdata, dst->rsize, &(dst->crc32));
	ROE ("zbuildcrc()");

	dst->wcnt = SPWOOB_WCNT;
	dst->wdata = safe_nmalloc (SPWOOB_WDATA, dst->wcnt);
	COOMGOTO (dst->wdata, "SPWOOB_WDATA weapon data", handle_error);

	dst->ucnt = SPWOOB_UCNT;
	dst->udata = safe_nmalloc (SPWOOB_UDATA, dst->ucnt);
	COOMGOTO (dst->udata, "SPWOOB_UDATA unit data", handle_error);

	dst->fcnt = SPWOOB_FCNT;
	dst->fdata = safe_nmalloc (SPWOOB_FDATA, dst->fcnt);
	COOMGOTO (dst->fdata, "SPWOOB_FDATA formation data", handle_error);

	dst->fmecnt = SPWOOB_FMECNT;
	dst->edata = safe_nmalloc (SPWOOB_EDATA, dst->fcnt * dst->fmecnt);
	COOMGOTO (dst->edata, "SPWOOBE_DATA formation elements data", handle_error);
	for (i=0; i<dst->fcnt; i++) {
		dst->fdata[i].elements = &(((SPWOOB_EDATA *)dst->edata)[i*dst->fmecnt]);
	}

	dst->efstart = SPWOOB_EFSTART;

	raw = (RAWOOB *)dst->rdata;

	for (i=0; i<dst->wcnt; i++) {
		if (raw->w.name[i].data[0] != '\0') {
			azstrcpy (raw->w.name[i].data, dst->wdata[i].name);
			dst->wdata[i].wclass		= SPWOOB_WCLASS_xlt (raw->w.wclass[i]);
			dst->wdata[i].size		= raw->w.size[i];
			dst->wdata[i].warhead		= raw->w.warhead[i];
			dst->wdata[i].kill		= raw->w.kill[i];
			dst->wdata[i].pen_AP		= raw->w.pen[i].AP;
			dst->wdata[i].pen_HE		= raw->w.pen[i].HE;
			dst->wdata[i].pen_HEAT		= raw->w.pen_HEAT[i];
			dst->wdata[i].pen_APCR		= raw->w.pen_APCR[i];
			dst->wdata[i].accuracy		= raw->w.accuracy[i];
			dst->wdata[i].range_max		= raw->w.rng_max[i];
			dst->wdata[i].range_APCR	= raw->w.rng_APCR[i];
			dst->wdata[i].valid		= true;
		}
	}

	for (i=0; i<dst->ucnt; i++) {
		if (raw->u.name[i].data[0] != '\0') {
			azstrcpy (raw->u.name[i].data, dst->udata[i].name);
			dst->udata[i].nation		= raw->u.nation[i];
			dst->udata[i].type		= SPWOOB_UTYPE_xlt (raw->u.uclass[i]);
			dst->udata[i].uclass		= spwaw_utype2class (dst->udata[i].type);
			dst->udata[i].start_yr		= raw->u.start_yr[i] + SPWAW_STARTYEAR;
			dst->udata[i].start_mo		= raw->u.start_mo[i];
			dst->udata[i].end_yr		= raw->u.end_yr[i] + SPWAW_STARTYEAR;
			dst->udata[i].end_mo		= raw->u.end_mo[i];
			dst->udata[i].size		= raw->u.size[i];
			dst->udata[i].crew		= raw->u.crew[i];
			dst->udata[i].survive		= raw->u.survive[i];
			dst->udata[i].cost		= raw->u.cost[i];
			dst->udata[i].speed		= raw->u.speed[i];
			//dst->udata[i].mclass		= SPWOOB_MOVCL_xlt (raw->u.movcl[i]);
			dst->udata[i].mclass		= SPWOOB_MOVCL_xlt (0);		//FIXME
			dst->udata[i].radio		= raw->u.radio[i];
			dst->udata[i].irvis		= raw->u.irvis[i];
			dst->udata[i].fc		= raw->u.fc[i];
			dst->udata[i].rf		= raw->u.rf[i];
			dst->udata[i].stab		= raw->u.stab[i];
			dst->udata[i].rof		= raw->u.rof[i];
			dst->udata[i].load_cap		= raw->u.load_cap[i];
			dst->udata[i].load_cost		= raw->u.load_cost[i];
			dst->udata[i].swim		= raw->u.swim[i];
			dst->udata[i].smkdev		= raw->u.smkdev[i];
			dst->udata[i].wpn1		= raw->u.wpn[i].w1;
			dst->udata[i].wpn1_HEammo	= raw->u.wpn_ammo[i].HE1;
			dst->udata[i].wpn1_APammo	= raw->u.wpn_ammo[i].AP1;
			dst->udata[i].wpn1_HEATammo	= raw->u.wpn1_HEATammo[i];
			dst->udata[i].wpn1_APCRammo	= raw->u.wpn1_APCRammo[i];
			dst->udata[i].wpn2		= raw->u.wpn[i].w2;
			dst->udata[i].wpn2_HEammo	= raw->u.wpn_ammo[i].HE2;
			dst->udata[i].wpn2_APammo	= raw->u.wpn_ammo[i].AP2;
			dst->udata[i].wpn3		= raw->u.wpn[i].w3;
			dst->udata[i].wpn3_HEammo	= raw->u.wpn_ammo[i].HE3;
			dst->udata[i].wpn3_APammo	= raw->u.wpn_ammo[i].AP3;
			dst->udata[i].wpn4		= raw->u.wpn[i].w4;
			dst->udata[i].wpn4_HEammo	= raw->u.wpn_ammo[i].HE4;
			dst->udata[i].wpn4_APammo	= raw->u.wpn_ammo[i].AP4;
			dst->udata[i].arm_FH		= raw->u.arm[i].FH;
			dst->udata[i].arm_SH		= raw->u.arm[i].SH;
			dst->udata[i].arm_RH		= raw->u.arm[i].RH;
			dst->udata[i].arm_FT		= raw->u.arm[i].FT;
			dst->udata[i].arm_ST		= raw->u.arm[i].ST;
			dst->udata[i].arm_RT		= raw->u.arm[i].RT;
			dst->udata[i].arm_TP		= raw->u.arm[i].TP;
			dst->udata[i].arm_SK		= raw->u.arm_SK[i];
			dst->udata[i].slp_FH		= raw->u.slp[i].FH;
			dst->udata[i].slp_SH		= raw->u.slp[i].SH;
			dst->udata[i].slp_RH		= raw->u.slp[i].RH;
			dst->udata[i].slp_FT		= raw->u.slp[i].FT;
			dst->udata[i].slp_ST		= raw->u.slp[i].ST;
			dst->udata[i].slp_RT		= raw->u.slp[i].RT;
			dst->udata[i].valid		= true;
		}
	}

	for (i=0; i<dst->fcnt; i++) {
		if (raw->f.name[i].data[0] != '\0') {
			azstrcpy (raw->f.name[i].data, dst->fdata[i].name);
			dst->fdata[i].nation		= raw->f.nation[i];
			dst->fdata[i].stat		= SPWOOB_FSTAT_xlt (raw->f.special[i]);
			dst->fdata[i].type		= SPWOOB_FTYPE_xlt (raw->f.type[i]);
			dst->fdata[i].pscr		= SPWOOB_FPSCR_xlt (raw->f.purchscrn[i]);
			dst->fdata[i].start_yr		= raw->f.start_yr[i] + SPWAW_STARTYEAR;
			dst->fdata[i].start_mo		= raw->f.start_mo[i];
			dst->fdata[i].end_yr		= raw->f.end_yr[i] + SPWAW_STARTYEAR;
			for (j=0; j<dst->fmecnt; j++) {
				dst->fdata[i].elements[j].rid = raw->f.urid[i].dat[j];
				dst->fdata[i].elements[j].cnt = raw->f.ucnt[i].dat[j];
			}
			dst->fdata[i].valid		= true;
		}
	}

	return (SPWERR_OK);

handle_error:
	if (dst->edata) safe_free (dst->edata);
	if (dst->wdata) safe_free (dst->wdata);
	if (dst->udata) safe_free (dst->udata);
	if (dst->fdata) safe_free (dst->fdata);
	return (rc);
}

SPWAW_ERROR
spwoob_load_raw_spwaw_files (SPWOOB *oob)
{
	return (spwoob_load_raw_files_core (oob, "oob*", spwaw_name2id, sizeof (RAWOOB), spwoob_load_raw_spwaw_data));
}

void
spwoob_dump_raw_spwaw_data (void *rdata, BYTE id, char *base)
{
	RAWOOB	*raw = (RAWOOB *)rdata;
	char	name[MAX_PATH+1];
	FILE	*file;
	int	i;
	char	azsname[OOBAZSNAME+1];

	if (!raw || !base) return;

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "raw_%s_%02u_layout.txt", base, id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file, "RAWOOB: size=0x%8.8x (%u)\n", sizeof(*raw), sizeof(*raw));
		fprintf (file, "RAWOOB-  w       : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,w), sizeof(raw->w), sizeof(raw->w));
		fprintf (file, "RAWOOB-  w-name  : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,w.name), sizeof(raw->w.name), sizeof(raw->w.name));
		fprintf (file, "RAWOOB-  w-wclass: 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,w.wclass), sizeof(raw->w.wclass), sizeof(raw->w.wclass));
		fprintf (file, "RAWOOB-  u       : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,u), sizeof(raw->u), sizeof(raw->u));
		fprintf (file, "RAWOOB-  u-name  : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,u.name), sizeof(raw->u.name), sizeof(raw->u.name));
		fprintf (file, "RAWOOB-  u-uclass: 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,u.uclass), sizeof(raw->u.uclass), sizeof(raw->u.uclass));
		fprintf (file, "RAWOOB-  f       : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,f), sizeof(raw->f), sizeof(raw->f));
		fprintf (file, "RAWOOB-  f-name  : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,f.name), sizeof(raw->f.name), sizeof(raw->f.name));
		fprintf (file, "RAWOOB-  f-nation: 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,f.nation), sizeof(raw->f.nation), sizeof(raw->f.nation));
		fclose (file);
	}

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "raw_%s_%02u_w.csv", base, id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file,
			"rid,name,"
			"wclass,B16,size,"
			"warhead,pen.HE,pen.AP,kill,accuracy,rng_max,rng_APCR,pen_APCR,pen_HEAT,sound,"
			"X1,X2,X3,X4,X5,X6,X7"
			"\n");

		for (i=1; i<SPWOOB_WCNT; i++) {
			azstrcpy (raw->w.name[i].data, azsname);
			fprintf (file,
				"%d,%s,"
				"%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u"
				"\n",
				i, azsname,
				raw->w.wclass[i],
				raw->w.__data0[i],
				raw->w.size[i],
				raw->w.warhead[i],
				raw->w.pen[i].HE, raw->w.pen[i].AP,
				raw->w.kill[i],
				raw->w.accuracy[i],
				raw->w.rng_max[i],
				raw->w.rng_APCR[i],
				raw->w.pen_APCR[i],
				raw->w.pen_HEAT[i],
				raw->w.sound[i],
				raw->w.__data2[0*SPWOOB_WCNT+i], raw->w.__data2[1*SPWOOB_WCNT+i],
				raw->w.__data2[2*SPWOOB_WCNT+i], raw->w.__data2[3*SPWOOB_WCNT+i],
				raw->w.__data2[4*SPWOOB_WCNT+i], raw->w.__data2[5*SPWOOB_WCNT+i],
				raw->w.__data2[6*SPWOOB_WCNT+i]
				);
		}
		fclose (file);
	}

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "raw_%s_%02u_u.csv", base, id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file,
			"rid,name,"
			"uclass,arm_SK,size,crew,"
			"wpn_w1,wpn_w2,wpn_w3,wpn_w4,"
			"wpn_ammo_HE1,wpn_ammo_AP1,wpn_ammo_HE2,wpn_ammo_AP2,wpn_ammo_HE3,wpn_ammo_AP3,wpn_ammo_HE4,wpn_ammo_AP4,"
			"speed,"
			"arm.FH,arm.SH,arm.RH,arm.FT,arm.ST,arm.RT,arm.TP,"
			"B39,B40,B41,B42,B43,B44,B45,"
			"B46,B47,B48,B49,B50,B51,B52,"
			"slp.FH,slp.SH,slp.RH,slp.FT,slp.ST,slp.RT,"
			"radio,start_yr,end_yr,cost,nation,"
			"lbm,irvis,fc,"
			"wpn1_APCRammo,wpn1_HEATammo,"
			"rof,stab,rf,"
			"B73,"
			"load_cap,survive,load_cost,icon,"
			"swim,end_mo,smkdev,start_mo,"
			"B83,B84,B85,B86,"
			"text,msound,"
			"X1,X2"
			"\n");

		for (i=1; i<SPWOOB_UCNT; i++) {
			azstrcpy (raw->u.name[i].data, azsname);
			fprintf (file,
				"%d,%s,"
				"%u,%u,%u,%u,"
				"%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,"
				"%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,"
                                "%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,"
				"%u,%u,%u,"
				"%u,%u,"
				"%u,%u,%u,"
				"%u,"
				"%u,%u,%u,%u,"
				"%u,%u,%u,%u,"
				"%u,%u,%u,%u,"
				"%u,%u,"
				"%u,%u"
				"\n",
				i, azsname,
				raw->u.uclass[i], raw->u.arm_SK[i], raw->u.size[i], raw->u.crew[i],
				raw->u.wpn[i].w1, raw->u.wpn[i].w2, raw->u.wpn[i].w3, raw->u.wpn[i].w4,
				raw->u.wpn_ammo[i].HE1, raw->u.wpn_ammo[i].AP1, raw->u.wpn_ammo[i].HE2, raw->u.wpn_ammo[i].AP2,
				raw->u.wpn_ammo[i].HE3, raw->u.wpn_ammo[i].AP3, raw->u.wpn_ammo[i].HE4, raw->u.wpn_ammo[i].AP4,
				raw->u.speed[i],
				raw->u.arm[i].FH,raw->u.arm[i].SH,raw->u.arm[i].RH,raw->u.arm[i].FT,raw->u.arm[i].ST,raw->u.arm[i].RT,raw->u.arm[i].TP,
				raw->u.arm[i].__data0[0],raw->u.arm[i].__data0[1],raw->u.arm[i].__data0[2],raw->u.arm[i].__data0[3],
				raw->u.arm[i].__data0[4],raw->u.arm[i].__data0[5],raw->u.arm[i].__data0[6],
				raw->u.__data1[0*SPWOOB_UCNT+i],raw->u.__data1[1*SPWOOB_UCNT+i],raw->u.__data1[2*SPWOOB_UCNT+i],raw->u.__data1[3*SPWOOB_UCNT+i],
				raw->u.__data1[4*SPWOOB_UCNT+i],raw->u.__data1[5*SPWOOB_UCNT+i],raw->u.__data1[6*SPWOOB_UCNT+i],
				raw->u.slp[i].FH,raw->u.slp[i].SH,raw->u.slp[i].RH,raw->u.slp[i].FT,raw->u.slp[i].ST,raw->u.slp[i].RT,
				raw->u.radio[i], raw->u.start_yr[i], raw->u.end_yr[i], raw->u.cost[i], raw->u.nation[i],
				raw->u.lbm[i], raw->u.irvis[i], raw->u.fc[i],
				raw->u.wpn1_APCRammo[i], raw->u.wpn1_HEATammo[i],
				raw->u.rof[i], raw->u.stab[i], raw->u.rf[i],
				raw->u.__data2[i],
				raw->u.load_cap[i], raw->u.survive[i], raw->u.load_cost[i],raw->u.icon[i],
				raw->u.swim[i], raw->u.end_mo[i], raw->u.smkdev[i], raw->u.start_mo[i], 
				raw->u.__data3[0*SPWOOB_UCNT+i],raw->u.__data3[1*SPWOOB_UCNT+i],
				raw->u.__data3[2*SPWOOB_UCNT+i],raw->u.__data3[3*SPWOOB_UCNT+i],
				raw->u.text[i], raw->u.msound[i],
				raw->u.__data4[0*SPWOOB_UCNT+i],raw->u.__data4[1*SPWOOB_UCNT+i]
				);
		}
		fclose (file);
	}

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "raw_%s_%02u_f.csv", base, id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file,
			"rid,name,"
			"nation,start_mo,special,"
			"X1,X2,X3,X4,X5,X6,X7,X8,"
			"start_yr,end_yr,purchscrn,type,"
			"ERID[1],ECNT[1],ERID[2],ECNT[2],ERID[3],ECNT[3],ERID[4],ECNT[4],ERID[5],ECNT[5],"
			"ERID[6],ECNT[6],ERID[7],ECNT[7],ERID[8],ECNT[8],ERID[9],ECNT[9],ERID[10],ECNT[10]"
			"\n");

		for (i=1; i<SPWOOB_FCNT; i++) {
			azstrcpy (raw->f.name[i].data, azsname);
			fprintf (file,
				"%d,%s,"
				"%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u"
				"\n",
				i, azsname,
				raw->f.nation[i], raw->f.start_mo[i], raw->f.special[i],
				raw->f.__data0[0*SPWOOB_FCNT+i], raw->f.__data0[1*SPWOOB_FCNT+i],
				raw->f.__data0[2*SPWOOB_FCNT+i], raw->f.__data0[3*SPWOOB_FCNT+i],
				raw->f.__data0[4*SPWOOB_FCNT+i], raw->f.__data0[5*SPWOOB_FCNT+i],
				raw->f.__data0[6*SPWOOB_FCNT+i], raw->f.__data0[7*SPWOOB_FCNT+i],
				raw->f.start_yr[i], raw->f.end_yr[i], raw->f.purchscrn[i], raw->f.type[i],
				raw->f.urid[i].dat[0], raw->f.ucnt[i].dat[0],
				raw->f.urid[i].dat[1], raw->f.ucnt[i].dat[1],
				raw->f.urid[i].dat[2], raw->f.ucnt[i].dat[2],
				raw->f.urid[i].dat[3], raw->f.ucnt[i].dat[3],
				raw->f.urid[i].dat[4], raw->f.ucnt[i].dat[4],
				raw->f.urid[i].dat[5], raw->f.ucnt[i].dat[5],
				raw->f.urid[i].dat[6], raw->f.ucnt[i].dat[6],
				raw->f.urid[i].dat[7], raw->f.ucnt[i].dat[7],
				raw->f.urid[i].dat[8], raw->f.ucnt[i].dat[8],
				raw->f.urid[i].dat[9], raw->f.ucnt[i].dat[9]
				);
		}
		fclose (file);
	}
}