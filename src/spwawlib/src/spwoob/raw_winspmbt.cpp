/** \file
 * The SPWaW Library - SPWaW OOB handling - raw winSPMBT data handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_spwoob_types.h>
#include "spwoob/raw_winspmbt.h"
#include "spwoob/raw.h"
#include "common/internal.h"
#include "utils/compression.h"

static BYTE
winspmbt_name2id (const char *name)
{
	char	local[16];
	int	id;
	int	rc;

	if (strlen (name) > 11) return (SPWOOB_BADOOBID);
	if (strnicmp (name, "obat", 4) != 0) return (SPWOOB_BADOOBID);

	memset (local, 0, sizeof (local));
	snprintf (local, sizeof (local) - 1, "%s", name);

	local[0] = (char)tolower (local[0]);
	local[1] = (char)tolower (local[1]);
	local[2] = (char)tolower (local[2]);
	local[3] = (char)tolower (local[3]);

	rc = sscanf (local, "obat%d", &id);
	if (rc <= 0) id = SPWOOB_BADOOBID;
	if ((id < 0) || (id > 255)) id = SPWOOB_BADOOBID;

	return ((BYTE)id);
}

SPWAW_ERROR
spwoob_load_raw_winspmbt_data (SPWOOB_DATA *dst)
{
	SPWAW_ERROR	rc;
	RAWOOB_WINSPMBT	*raw;
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

	raw = (RAWOOB_WINSPMBT *)dst->rdata;

	for (i=0; i<SPWOOB_WCNT; i++) {
		if (raw->w.name[i].data[0] != '\0') {
			azstrcpy (raw->w.name[i].data, dst->wdata[i].name);
			dst->wdata[i].wclass		= SPWOOB_WINSPMBT_WCLASS_xlt (raw->w.wclass[i]);
			dst->wdata[i].size		= raw->w.size[i];
			dst->wdata[i].warhead		= raw->w.warhead[i];
			dst->wdata[i].kill_HE		= raw->w.kill[i].HE;
			dst->wdata[i].kill_AP		= raw->w.kill[i].AP;
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

	for (i=0; i<SPWOOB_UCNT; i++) {
		if (raw->u.name[i].data[0] != '\0') {
			azstrcpy (raw->u.name[i].data, dst->udata[i].name);
			dst->udata[i].nation		= raw->u.nation[i];
			dst->udata[i].type		= SPWOOB_WINSPMBT_UTYPE_xlt (raw->u.uclass[i]);
			dst->udata[i].uclass		= SPWOOB_UTYPE_classify (dst->udata[i].type);
			dst->udata[i].mclass		= SPWOOB_MOVCL_xlt (raw->u.movcl[i]);
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
			dst->udata[i].arm_HE_FH		= raw->u.arm[i].HE_FH;
			dst->udata[i].arm_HE_SH		= raw->u.arm[i].HE_SH;
			dst->udata[i].arm_HE_RH		= raw->u.arm[i].HE_RH;
			dst->udata[i].arm_HE_FT		= raw->u.arm[i].HE_FT;
			dst->udata[i].arm_HE_ST		= raw->u.arm[i].HE_ST;
			dst->udata[i].arm_HE_RT		= raw->u.arm[i].HE_RT;
			dst->udata[i].arm_HE_TP		= raw->u.arm[i].HE_TP;
			dst->udata[i].arm_HE_SK		= 0;
			dst->udata[i].arm_AP_FH		= raw->u.arm[i].AP_FH;
			dst->udata[i].arm_AP_SH		= raw->u.arm[i].AP_SH;
			dst->udata[i].arm_AP_RH		= raw->u.arm[i].AP_RH;
			dst->udata[i].arm_AP_FT		= raw->u.arm[i].AP_FT;
			dst->udata[i].arm_AP_ST		= raw->u.arm[i].AP_ST;
			dst->udata[i].arm_AP_RT		= raw->u.arm[i].AP_RT;
			dst->udata[i].arm_AP_TP		= raw->u.arm[i].AP_TP;
			dst->udata[i].arm_AP_SK		= raw->u.arm_SK[i];
			dst->udata[i].slp_FH		= 0;
			dst->udata[i].slp_SH		= 0;
			dst->udata[i].slp_RH		= 0;
			dst->udata[i].slp_FT		= 0;
			dst->udata[i].slp_ST		= 0;
			dst->udata[i].slp_RT		= 0;
			dst->udata[i].sound		= raw->u.sound[i];
			dst->udata[i].lbm		= raw->u.lbm[i];
			dst->udata[i].icon		= raw->u.icon[i];
			dst->udata[i].icon_desert	= raw->u.icon_desert[i];
			dst->udata[i].icon_winter	= raw->u.icon_winter[i];
			dst->udata[i].valid		= true;
		}
	}

	for (i=0; i<SPWOOB_FCNT; i++) {
		if (raw->f.name[i].data[0] != '\0') {
			azstrcpy (raw->f.name[i].data, dst->fdata[i].name);
			dst->fdata[i].nation		= raw->f.nation[i];
			dst->fdata[i].stat		= SPWOOB_FSTAT_D;
			dst->fdata[i].type		= SPWOOB_FTYPE_xlt (raw->f.type[i]);
			dst->fdata[i].pscr		= SPWOOB_FPSCR_xlt (raw->f.pscr[i]);
			dst->fdata[i].start_yr		= raw->f.start_yr[i] + SPWAW_STARTYEAR;
			dst->fdata[i].start_mo		= raw->f.start_mo[i];
			dst->fdata[i].end_yr		= raw->f.end_yr[i] + SPWAW_STARTYEAR;
			dst->fdata[i].end_mo		= raw->f.end_mo[i];
			dst->fdata[i].exp_mod		= raw->f.exp_mod[i];
			dst->fdata[i].mor_mod		= raw->f.mor_mod[i];
			for (j=0; j<dst->fmecnt; j++) {
				dst->fdata[i].elements[j].rid = raw->f.urid[i].dat[j];
				dst->fdata[i].elements[j].cnt = dst->fdata[i].elements[j].rid ? 1 : 0;
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
spwoob_load_raw_winspmbt_files (SPWOOB *oob)
{
	return (spwoob_load_raw_files_core (oob, "*.obf", winspmbt_name2id, sizeof (RAWOOB_WINSPMBT), spwoob_load_raw_winspmbt_data));
}

void
spwoob_dump_raw_winspmbt_data (void *rdata, BYTE id, char *base)
{
	RAWOOB_WINSPMBT	*raw = (RAWOOB_WINSPMBT *)rdata;
	char		name[MAX_PATH+1];
	FILE		*file;
	int		i;
	char		azsname[OOBAZSNAME+1];

	if (!raw || !base) return;

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "raw_winspmbt_%s_%02u_layout.txt", base, id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file, "RAWOOB_WINSPMBT: size=0x%8.8x (%u)\n", sizeof(*raw), sizeof(*raw));
		fprintf (file, "RAWOOB_WINSPMBT-cmt       : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB_WINSPMBT,cmt), sizeof(raw->cmt), sizeof(raw->cmt));
		fprintf (file, "RAWOOB_WINSPMBT-  w       : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB_WINSPMBT,w), sizeof(raw->w), sizeof(raw->w));
		fprintf (file, "RAWOOB_WINSPMBT-  w-name  : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB_WINSPMBT,w.name), sizeof(raw->w.name), sizeof(raw->w.name));
		fprintf (file, "RAWOOB_WINSPMBT-  w-wclass: 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB_WINSPMBT,w.wclass), sizeof(raw->w.wclass), sizeof(raw->w.wclass));
		fprintf (file, "RAWOOB_WINSPMBT-  u       : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB_WINSPMBT,u), sizeof(raw->u), sizeof(raw->u));
		fprintf (file, "RAWOOB_WINSPMBT-  u-name  : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB_WINSPMBT,u.name), sizeof(raw->u.name), sizeof(raw->u.name));
		fprintf (file, "RAWOOB_WINSPMBT-  u-uclass: 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB_WINSPMBT,u.uclass), sizeof(raw->u.uclass), sizeof(raw->u.uclass));
		fprintf (file, "RAWOOB_WINSPMBT-  f       : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB_WINSPMBT,f), sizeof(raw->f), sizeof(raw->f));
		fprintf (file, "RAWOOB_WINSPMBT-  f-name  : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB_WINSPMBT,f.name), sizeof(raw->f.name), sizeof(raw->f.name));
		fprintf (file, "RAWOOB_WINSPMBT-  f-nation: 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB_WINSPMBT,f.nation), sizeof(raw->f.nation), sizeof(raw->f.nation));
		fclose (file);
	}

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "raw_winspmbt_%s_%02u_cmt.dat", base, id);
	file = fopen (name, "w");
	if (file) {
		fwrite (&(raw->cmt), sizeof(raw->cmt), 1, file);
		fclose (file);
	}

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "raw_winspmbt_%s_%02u_w.csv", base, id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file,
			"rid,name,"
			"wclass,size,sound,"
			"warhead,pen.HE,pen.AP,kill.HE,kill.AP,accuracy,range_max,range_APCR,pen_APCR,pen_HEAT,"
			"B28,B29,B30,B31,B32,B33,B34,B35,B36,B37,B38,B39"
			"\n");

		for (i=1; i<SPWOOB_WCNT; i++) {
			azstrcpy (raw->w.name[i].data, azsname);
			fprintf (file,
				"%d,%s,"
				"%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u"
				"\n",
				i, azsname,
				raw->w.wclass[i], raw->w.size[i], raw->w.sound[i],
				raw->w.warhead[i],
				raw->w.pen[i].HE, raw->w.pen[i].AP,
				raw->w.kill[i].HE, raw->w.kill[i].AP,
				raw->w.accuracy[i], raw->w.rng_max[i], raw->w.rng_APCR[i],
				raw->w.pen_APCR[i], raw->w.pen_HEAT[i],
				raw->w.__data28[i], raw->w.__data29[i], raw->w.__data30[i], raw->w.__data31[i],
				raw->w.__data32[i], raw->w.__data33[i], raw->w.__data34[i], raw->w.__data35[i],
				raw->w.__data36[i], raw->w.__data37[i], raw->w.__data38[i], raw->w.__data39[i]
				);
		}
		fclose (file);
	}

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "raw_winspmbt_%s_%02u_u.csv", base, id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file,
			"rid,name,"
			"uclass,arm_SK,size,crew,"
			"wpn_w1,wpn_w2,wpn_w3,wpn_w4,"
			"wpn_ammo_HE1,wpn_ammo_AP1,wpn_ammo_HE2,wpn_ammo_AP2,wpn_ammo_HE3,wpn_ammo_AP3,wpn_ammo_HE4,wpn_ammo_AP4,"
			"speed,"
			"arm.AP_FH,arm.AP_SH,arm.AP_RH,arm.AP_FT,arm.AP_ST,arm.AP_RT,arm.AP_TP,"
			"arm.HE_FH,arm.HE_SH,arm.HE_RH,arm.HE_FT,arm.HE_ST,arm.HE_RT,arm.HE_TP,"
			"B46,B47,B48,B49,"
			"B50,B51,B52,B53,"
			"B54,B55,B56,B57,"
			"B58,"
			"radio,start_yr,end_yr,cost,nation,"
			"lbm,irvis,fc,"
			"wpn1_APCRammo,wpn1_HEATammo,"
			"rof,stab,rf,"
			"ewa,"
			"load_cap,survive,load_cost,"
			"icon,"
			"swim,movcl,smkdev,"
			"start_mo,end_mo,"
			"sound,"
			"wpn_snd_s1,wpn_snd_s2,wpn_snd_s3,wpn_snd_s4,"
			"B90,"
			"icon_winter,icon_desert,"
			"B98,B99,"
			"B100,B101,B102,B103,B104,B105,B106,B107,B108,B109,"
			"B110,B111,B112,B113,B114,B115,B116,B117,B118,B119,"
			"B120,B121,B122,B123"
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
				"%u,%u,%u,%u,"
				"%u,%u,%u,%u,"
				"%u,%u,%u,%u,"
				"%u,"
				"%u,%u,%u,%u,%u,"
				"%u,%u,%u,"
				"%u,%u,"
				"%u,%u,%u,"
				"%u,"
				"%u,%u,%u,"
				"%u,"
				"%u,%u,%u,"
				"%u,%u,"
				"%u,"
				"%u,%u,%u,%u,"
				"%u,"
				"%u,%u,"
				"%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u"
				"\n",
				i, azsname,
				raw->u.uclass[i], raw->u.arm_SK[i], raw->u.size[i], raw->u.crew[i],
				raw->u.wpn[i].w1, raw->u.wpn[i].w2, raw->u.wpn[i].w3, raw->u.wpn[i].w4,
				raw->u.wpn_ammo[i].HE1, raw->u.wpn_ammo[i].AP1, raw->u.wpn_ammo[i].HE2, raw->u.wpn_ammo[i].AP2,
				raw->u.wpn_ammo[i].HE3, raw->u.wpn_ammo[i].AP3, raw->u.wpn_ammo[i].HE4, raw->u.wpn_ammo[i].AP4,
				raw->u.speed[i],
				raw->u.arm[i].AP_FH,raw->u.arm[i].AP_SH,raw->u.arm[i].AP_RH,raw->u.arm[i].AP_FT,raw->u.arm[i].AP_ST,raw->u.arm[i].AP_RT,raw->u.arm[i].AP_TP,
				raw->u.arm[i].HE_FH,raw->u.arm[i].HE_SH,raw->u.arm[i].HE_RH,raw->u.arm[i].HE_FT,raw->u.arm[i].HE_ST,raw->u.arm[i].HE_RT,raw->u.arm[i].HE_TP,
				raw->u.__data46[i], raw->u.__data47[i], raw->u.__data48[i], raw->u.__data49[i],
				raw->u.__data50[i], raw->u.__data51[i], raw->u.__data52[i], raw->u.__data53[i],
				raw->u.__data54[i], raw->u.__data55[i], raw->u.__data56[i], raw->u.__data57[i],
				raw->u.__data58[i],
				raw->u.radio[i], raw->u.start_yr[i], raw->u.end_yr[i], raw->u.cost[i], raw->u.nation[i],
				raw->u.lbm[i], raw->u.irvis[i], raw->u.fc[i],
				raw->u.wpn1_APCRammo[i], raw->u.wpn1_HEATammo[i],
				raw->u.rof[i], raw->u.stab[i], raw->u.rf[i],
				raw->u.ewa[i],
				raw->u.load_cap[i], raw->u.survive[i], raw->u.load_cost[i],
				raw->u.icon[i],
				raw->u.swim[i], raw->u.movcl[i], raw->u.smkdev[i],
				raw->u.start_mo[i], raw->u.end_mo[i],
				raw->u.sound[i], raw->u.wpn_snd[i].s1, raw->u.wpn_snd[i].s2, raw->u.wpn_snd[i].s3, raw->u.wpn_snd[i].s4,
				raw->u.__data90[i],
				raw->u.icon_winter[i], raw->u.icon_desert[i],
				raw->u.__data098[i],
				raw->u.__data099[i],
				raw->u.__data100[i],
				raw->u.__data101[i],
				raw->u.__data102[i],
				raw->u.__data103[i],
				raw->u.__data104[i],
				raw->u.__data105[i],
				raw->u.__data106[i],
				raw->u.__data107[i],
				raw->u.__data108[i],
				raw->u.__data109[i],
				raw->u.__data110[i],
				raw->u.__data111[i],
				raw->u.__data112[i],
				raw->u.__data113[i],
				raw->u.__data114[i],
				raw->u.__data115[i],
				raw->u.__data116[i],
				raw->u.__data117[i],
				raw->u.__data118[i],
				raw->u.__data119[i],
				raw->u.__data120[i],
				raw->u.__data121[i],
				raw->u.__data122[i],
				raw->u.__data123[i]
				);
		}
		fclose (file);
	}

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "raw_winspmbt_%s_%02u_f.csv", base, id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file,
			"rid,name,"
			"nation,start_yr,end_yr,pscr,type,"
			"ERID[1],ERID[2],ERID[3],ERID[4],ERID[5],ERID[6],ERID[7],ERID[8],ERID[9],ERID[10],"
			"start_mo,end_mo,exp_mod,mor_mod,"
			"B44,B45,B46,B47,"
			"B48,B49,B50,B51,"
			"B52,B53,B54,B55,"
			"B56,B57,B58,B59"
			"\n");

		for (i=1; i<SPWOOB_FCNT; i++) {
			azstrcpy (raw->f.name[i].data, azsname);
			fprintf (file,
				"%d,%s,"
				"%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%d,%d,"
				"%u,%u,%u,%u,"
				"%u,%u,%u,%u,"
				"%u,%u,%u,%u,"
				"%u,%u,%u,%u"
				"\n",
				i, azsname,
				raw->f.nation[i], raw->f.start_yr[i], raw->f.end_yr[i], raw->f.pscr[i], raw->f.type[i],
				raw->f.urid[i].dat[0], raw->f.urid[i].dat[1],
				raw->f.urid[i].dat[2], raw->f.urid[i].dat[3],
				raw->f.urid[i].dat[4], raw->f.urid[i].dat[5],
				raw->f.urid[i].dat[6], raw->f.urid[i].dat[7],
				raw->f.urid[i].dat[8], raw->f.urid[i].dat[9],
				raw->f.start_mo[i], raw->f.end_mo[i], raw->f.exp_mod[i], raw->f.mor_mod[i],
				raw->f.__data44[i], raw->f.__data45[i], raw->f.__data46[i], raw->f.__data47[i],
				raw->f.__data48[i], raw->f.__data49[i], raw->f.__data50[i], raw->f.__data51[i],
				raw->f.__data52[i], raw->f.__data53[i], raw->f.__data54[i], raw->f.__data55[i],
				raw->f.__data56[i], raw->f.__data57[i], raw->f.__data58[i], raw->f.__data59[i]
				);
		}
		fclose (file);
	}
}