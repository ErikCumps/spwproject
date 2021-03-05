/** \file
 * The SPWaW Library - SPWaW OOB handling - raw SPWaW data handling.
 *
 * Copyright (C) 2019-2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_spwoob_types.h>
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

SPWAW_ERROR
spwoob_load_raw_spwaw_data (SPWOOB_DATA *dst)
{
	SPWAW_ERROR	rc;
	RAWOOB_SPWAW	*raw;
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

	raw = (RAWOOB_SPWAW *)dst->rdata;

	for (i=0; i<dst->wcnt; i++) {
		if (raw->w.name[i].data[0] != '\0') {
			azstrcpy (raw->w.name[i].data, dst->wdata[i].name);
			dst->wdata[i].wclass		= SPWOOB_SPWAW_WCLASS_xlt (raw->w.wclass[i]);
			dst->wdata[i].size		= raw->w.size[i];
			dst->wdata[i].warhead		= raw->w.warhead[i];
			dst->wdata[i].kill_HE		= raw->w.kill[i];
			dst->wdata[i].kill_AP		= 0;
			dst->wdata[i].pen_HE		= raw->w.pen[i].HE;
			dst->wdata[i].pen_AP		= raw->w.pen[i].AP;
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
			dst->udata[i].nation		= raw->u.nation[i];
			azstrcpy (raw->u.name[i].data, dst->udata[i].name);
			dst->udata[i].type		= SPWOOB_SPWAW_UTYPE_xlt (raw->u.uclass[i]);
			dst->udata[i].uclass		= SPWOOB_UTYPE_classify (dst->udata[i].type);
			dst->udata[i].mclass		= SPWOOB_MOVCL_xlt (0);		//FIXME
			dst->udata[i].start_yr		= raw->u.start_yr[i] + SPWAW_STARTYEAR;
			dst->udata[i].start_mo		= raw->u.start_mo[i];
			dst->udata[i].end_yr		= raw->u.end_yr[i] + SPWAW_STARTYEAR;
			dst->udata[i].end_mo		= raw->u.end_mo[i];
			dst->udata[i].size		= raw->u.size[i];
			dst->udata[i].crew		= raw->u.crew[i];
			dst->udata[i].survive		= raw->u.survive[i];
			dst->udata[i].cost		= raw->u.cost[i];
			dst->udata[i].speed		= raw->u.speed[i];
			dst->udata[i].radio		= raw->u.radio[i];
			dst->udata[i].irvis		= raw->u.irvis[i];
			dst->udata[i].fc		= raw->u.fc[i];
			dst->udata[i].rf		= raw->u.rf[i];
			dst->udata[i].ew		= 0;
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
			dst->udata[i].arm_HE_FH		= 0;
			dst->udata[i].arm_HE_SH		= 0;
			dst->udata[i].arm_HE_RH		= 0;
			dst->udata[i].arm_HE_FT		= 0;
			dst->udata[i].arm_HE_ST		= 0;
			dst->udata[i].arm_HE_RT		= 0;
			dst->udata[i].arm_HE_TP		= 0;
			dst->udata[i].arm_HE_SK		= 0;
			dst->udata[i].arm_AP_FH		= raw->u.arm[i].FH;
			dst->udata[i].arm_AP_SH		= raw->u.arm[i].SH;
			dst->udata[i].arm_AP_RH		= raw->u.arm[i].RH;
			dst->udata[i].arm_AP_FT		= raw->u.arm[i].FT;
			dst->udata[i].arm_AP_ST		= raw->u.arm[i].ST;
			dst->udata[i].arm_AP_RT		= raw->u.arm[i].RT;
			dst->udata[i].arm_AP_TP		= raw->u.arm[i].TP;
			dst->udata[i].arm_AP_SK		= raw->u.arm_SK[i];
			dst->udata[i].slp_FH		= raw->u.slp[i].FH;
			dst->udata[i].slp_SH		= raw->u.slp[i].SH;
			dst->udata[i].slp_RH		= raw->u.slp[i].RH;
			dst->udata[i].slp_FT		= raw->u.slp[i].FT;
			dst->udata[i].slp_ST		= raw->u.slp[i].ST;
			dst->udata[i].slp_RT		= raw->u.slp[i].RT;
			dst->udata[i].sound		= raw->u.sound[i];
			dst->udata[i].text		= raw->u.text[i];
			dst->udata[i].lbm		= raw->u.lbm[i];
			dst->udata[i].icon		= raw->u.icon[i];
			dst->udata[i].valid		= true;
		}
	}

	for (i=0; i<dst->fcnt; i++) {
		if (raw->f.name[i].data[0] != '\0') {
			dst->fdata[i].nation		= raw->f.nation[i];
			azstrcpy (raw->f.name[i].data, dst->fdata[i].name);
			dst->fdata[i].stat		= SPWOOB_FSTAT_xlt (raw->f.status[i]);
			dst->fdata[i].type		= SPWOOB_FTYPE_xlt (raw->f.type[i]);
			dst->fdata[i].pscr		= SPWOOB_FPSCR_xlt (raw->f.purchscrn[i]);
			dst->fdata[i].start_yr		= raw->f.start_yr[i] + SPWAW_STARTYEAR;
			dst->fdata[i].start_mo		= raw->f.start_mo[i];
			dst->fdata[i].end_yr		= raw->f.end_yr[i] + SPWAW_STARTYEAR;
			dst->fdata[i].end_mo		= 12; /* Units are available until the end of the end year */
			dst->fdata[i].exp_mod		= 0 ;
			dst->fdata[i].mor_mod		= 0 ;
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
	return (spwoob_load_raw_files_core (oob, "oob*", spwaw_name2id, sizeof (RAWOOB_SPWAW), spwoob_load_raw_spwaw_data));
}

void
spwoob_dump_raw_spwaw_data (void *rdata, BYTE id, char *base)
{
	RAWOOB_SPWAW	*raw = (RAWOOB_SPWAW *)rdata;
	char		name[MAX_PATH+1];
	FILE		*file;
	int		i;
	char		azsname[OOBAZSNAME+1];

	if (!raw || !base) return;

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "raw_spwaw_%s_%02u_layout.txt", base, id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file, "RAWOOB_SPWAW: size=0x%8.8x (%u)\n", sizeof(*raw), sizeof(*raw));
		fprintf (file, "RAWOOB_SPWAW-  w       : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB_SPWAW,w), sizeof(raw->w), sizeof(raw->w));
		fprintf (file, "RAWOOB_SPWAW-  w-name  : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB_SPWAW,w.name), sizeof(raw->w.name), sizeof(raw->w.name));
		fprintf (file, "RAWOOB_SPWAW-  w-wclass: 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB_SPWAW,w.wclass), sizeof(raw->w.wclass), sizeof(raw->w.wclass));
		fprintf (file, "RAWOOB_SPWAW-  u       : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB_SPWAW,u), sizeof(raw->u), sizeof(raw->u));
		fprintf (file, "RAWOOB_SPWAW-  u-name  : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB_SPWAW,u.name), sizeof(raw->u.name), sizeof(raw->u.name));
		fprintf (file, "RAWOOB_SPWAW-  u-uclass: 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB_SPWAW,u.uclass), sizeof(raw->u.uclass), sizeof(raw->u.uclass));
		fprintf (file, "RAWOOB_SPWAW-  f       : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB_SPWAW,f), sizeof(raw->f), sizeof(raw->f));
		fprintf (file, "RAWOOB_SPWAW-  f-name  : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB_SPWAW,f.name), sizeof(raw->f.name), sizeof(raw->f.name));
		fprintf (file, "RAWOOB_SPWAW-  f-nation: 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB_SPWAW,f.nation), sizeof(raw->f.nation), sizeof(raw->f.nation));
		fclose (file);
	}

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "raw_spwaw_%s_%02u_w.csv", base, id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file,
			"rid,name,"
			"wclass,B16,size,"
			"warhead,pen.HE,pen.AP,"
			"kill,B22,accuracy,rng_max,rng_APCR,pen_APCR,pen_HEAT,sound,"
			"B30,B31,B32,B33,B34,B35,B36"
			"\n");

		for (i=1; i<SPWOOB_WCNT; i++) {
			azstrcpy (raw->w.name[i].data, azsname);
			fprintf (file,
				"%d,%s,"
				"%u,%u,%u,"
				"%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u"
				"\n",
				i, azsname,
				raw->w.wclass[i],
				raw->w.__data16[i],
				raw->w.size[i],
				raw->w.warhead[i],
				raw->w.pen[i].HE, raw->w.pen[i].AP,
				raw->w.kill[i],
				raw->w.__data22[i],
				raw->w.accuracy[i],
				raw->w.rng_max[i],
				raw->w.rng_APCR[i],
				raw->w.pen_APCR[i],
				raw->w.pen_HEAT[i],
				raw->w.sound[i],
				raw->w.__data30[i], raw->w.__data31[i],
				raw->w.__data32[i], raw->w.__data33[i],
				raw->w.__data34[i], raw->w.__data35[i],
				raw->w.__data36[i]
				);
		}
		fclose (file);
	}

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "raw_spwaw_%s_%02u_u.csv", base, id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file,
			"rid,name,"
			"uclass,arm_SK,size,crew,"
			"wpn_w1,wpn_w2,wpn_w3,wpn_w4,"
			"wpn_ammo_HE1,wpn_ammo_AP1,wpn_ammo_HE2,wpn_ammo_AP2,wpn_ammo_HE3,wpn_ammo_AP3,wpn_ammo_HE4,wpn_ammo_AP4,"
			"speed,"
			"arm.FH,arm.SH,arm.RH,arm.FT,arm.ST,arm.RT,arm.TP,"
			"arm.B07,arm.B08,arm.B09,arm.B10,arm.B11,arm.B12,arm.B13,"
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
			"text,sound,"
			"B89,B90"
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
				raw->u.arm[i].__data07, raw->u.arm[i].__data08, raw->u.arm[i].__data09, raw->u.arm[i].__data10,
				raw->u.arm[i].__data11, raw->u.arm[i].__data12, raw->u.arm[i].__data13,
				raw->u.__data46[i], raw->u.__data47[i], raw->u.__data48[i], raw->u.__data49[i],
				raw->u.__data50[i], raw->u.__data51[i], raw->u.__data52[i],
				raw->u.slp[i].FH,raw->u.slp[i].SH,raw->u.slp[i].RH,raw->u.slp[i].FT,raw->u.slp[i].ST,raw->u.slp[i].RT,
				raw->u.radio[i], raw->u.start_yr[i], raw->u.end_yr[i], raw->u.cost[i], raw->u.nation[i],
				raw->u.lbm[i], raw->u.irvis[i], raw->u.fc[i],
				raw->u.wpn1_APCRammo[i], raw->u.wpn1_HEATammo[i],
				raw->u.rof[i], raw->u.stab[i], raw->u.rf[i],
				raw->u.__data73[i],
				raw->u.load_cap[i], raw->u.survive[i], raw->u.load_cost[i],raw->u.icon[i],
				raw->u.swim[i], raw->u.end_mo[i], raw->u.smkdev[i], raw->u.start_mo[i], 
				raw->u.__data83[i], raw->u.__data84[i], raw->u.__data85[i], raw->u.__data86[i],
				raw->u.text[i], raw->u.sound[i],
				raw->u.__data89[i], raw->u.__data90[i]
				);
		}
		fclose (file);
	}

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "raw_spwaw_%s_%02u_f.csv", base, id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file,
			"rid,name,"
			"nation,start_mo,status,"
			"B18,B19,B20,B21,B22,B23,B24,B25,"
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
				raw->f.nation[i], raw->f.start_mo[i], raw->f.status[i],
				raw->f.__data18[i], raw->f.__data19[i], raw->f.__data20[i], raw->f.__data21[i],
				raw->f.__data22[i], raw->f.__data23[i], raw->f.__data24[i], raw->f.__data25[i],
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