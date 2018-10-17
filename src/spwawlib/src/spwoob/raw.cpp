/** \file
 * The SPWaW Library - SPWaW OOB handling.
 *
 * Copyright (C) 2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "common/internal.h"
#include "raw.h"

void
spwoob_dump_raw_data (RAWOOB *raw, BYTE id, char *base)
{
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
		fprintf (file, "RAWOOB-cmt       : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,cmt), sizeof(raw->cmt), sizeof(raw->cmt));
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
	snprintf (name, sizeof (name) - 1, "raw_%s_%02u_cmt.dat", base, id);
	file = fopen (name, "w");
	if (file) {
		fwrite (&(raw->cmt), sizeof(raw->cmt), 1, file);
		fclose (file);
	}

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "raw_%s_%02u_w.csv", base, id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file,
			"id,name,"
			"wclass,size,"
			"B02,"
			"warhead,pen.HE,pen.AP,kill.HE,kill.AP,accuracy,range_max,range_APCR,pen_APCR,pen_HEAT,"
			"X1,X2,X3,X4,X5,X6,X7,X8,X9,X10,X11,X12"
			"\n");

		for (i=1; i<SPWOOB_WCNT; i++) {
			azstrcpy (raw->w.name[i].data, azsname);
			fprintf (file,
				"%d,%s,"
				"%u,%u,"
				"%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u"
				"\n",
				i, azsname,
				raw->w.wclass[i], raw->w.size[i],
				raw->w.__data02[i],
				raw->w.warhead[i],
				raw->w.pen[i].HE, raw->w.pen[i].AP,
				raw->w.kill[i].HE, raw->w.kill[i].AP,
				raw->w.accuracy[i], raw->w.rng_max[i], raw->w.rng_APCR[i],
				raw->w.pen_APCR[i], raw->w.pen_HEAT[i],
				raw->w.__dataX[ 0*SPWOOB_WCNT+i], raw->w.__dataX[ 1*SPWOOB_WCNT+i],
				raw->w.__dataX[ 2*SPWOOB_WCNT+i], raw->w.__dataX[ 3*SPWOOB_WCNT+i],
				raw->w.__dataX[ 4*SPWOOB_WCNT+i], raw->w.__dataX[ 5*SPWOOB_WCNT+i],
				raw->w.__dataX[ 6*SPWOOB_WCNT+i], raw->w.__dataX[ 7*SPWOOB_WCNT+i],
				raw->w.__dataX[ 8*SPWOOB_WCNT+i], raw->w.__dataX[ 9*SPWOOB_WCNT+i],
				raw->w.__dataX[10*SPWOOB_WCNT+i], raw->w.__dataX[11*SPWOOB_WCNT+i]
				);
		}
		fclose (file);
	}

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "raw_%s_%02u_u.csv", base, id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file,
			"id,name,"
			"uclass,arm_SK,size,crew,"
			"wpn_w1,wpn_w2,wpn_w3,wpn_w4,"
			"wpn_ammo_HE1,wpn_ammo_AP1,wpn_ammo_HE2,wpn_ammo_AP2,wpn_ammo_HE3,wpn_ammo_AP3,wpn_ammo_HE4,wpn_ammo_AP4,"
			"speed,"
			"arm.AP_FH,arm.AP_SH,arm.AP_RH,arm.AP_FT,arm.AP_ST,arm.AP_RT,arm.AP_TP,"
			"arm.HE_FH,arm.HE_SH,arm.HE_RH,arm.HE_FT,arm.HE_ST,arm.HE_RT,arm.HE_TP,"
			"B31,B32,B33,B34,B35,B36,B37,B38,B39,B40,B41,B42,B43,"
			"radio,start_yr,end_yr,cost,nation,"
			"lbm,irvis,fc,"
			"wpn1_APCRammo,wpn1_HEATammo,"
			"rof,stab,rf,"
			"B59,"
			"load_cap,survive,load_cost,"
			"icon,"
			"swim,movcl,smkdev,"
			"start_mo,end_mo,"
			"sound,wpn_snd_s1,wpn_snd_s2,wpn_snd_s3,wpn_snd_s4,"
			"B75,"
			"icon_winter,icon_desert,"
			"X01,X02,X03,X04,X05,X06,X07,X08,X09,X11,X12,X13,X14,X15,X16,X17,X18,X19,X21,X22,X23,X24,X25,X26"
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
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,"
				"%u,%u,%u,"
				"%u,%u,"
				"%u,%u,%u,"
				"%u,"
				"%u,%u,%u,"
				"%u,"
				"%u,%u,%u,"
				"%u,%u,"
				"%u,%u,%u,%u,%u,"
				"%u,"
				"%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u"
				"\n",
				i, azsname,
				raw->u.uclass[i], raw->u.arm_SK[i], raw->u.size[i], raw->u.crew[i],
				raw->u.wpn[i].w1, raw->u.wpn[i].w2, raw->u.wpn[i].w3, raw->u.wpn[i].w4,
				raw->u.wpn_ammo[i].HE1, raw->u.wpn_ammo[i].AP1, raw->u.wpn_ammo[i].HE2, raw->u.wpn_ammo[i].AP2,
				raw->u.wpn_ammo[i].HE3, raw->u.wpn_ammo[i].AP3, raw->u.wpn_ammo[i].HE4, raw->u.wpn_ammo[i].AP4,
				raw->u.speed[i],
				raw->u.arm[i].AP_FH,raw->u.arm[i].AP_SH,raw->u.arm[i].AP_RH,raw->u.arm[i].AP_FT,raw->u.arm[i].AP_ST,raw->u.arm[i].AP_RT,raw->u.arm[i].AP_TP,
				raw->u.arm[i].HE_FH,raw->u.arm[i].HE_SH,raw->u.arm[i].HE_RH,raw->u.arm[i].HE_FT,raw->u.arm[i].HE_ST,raw->u.arm[i].HE_RT,raw->u.arm[i].HE_TP,
				raw->u.__data31[i], raw->u.__data32[i], raw->u.__data33[i], raw->u.__data34[i], raw->u.__data35[i], raw->u.__data36[i], raw->u.__data37[i], raw->u.__data38[i], raw->u.__data39[i], raw->u.__data40[i], raw->u.__data41[i], raw->u.__data42[i], raw->u.__data43[i],
				raw->u.radio[i], raw->u.start_yr[i], raw->u.end_yr[i], raw->u.cost[i], raw->u.nation[i],
				raw->u.lbm[i], raw->u.irvis[i], raw->u.fc[i],
				raw->u.wpn1_APCRammo[i], raw->u.wpn1_HEATammo[i],
				raw->u.rof[i], raw->u.stab[i], raw->u.rf[i],
				raw->u.__data59[i],
				raw->u.load_cap[i], raw->u.survive[i], raw->u.load_cost[i],
				raw->u.icon[i],
				raw->u.swim[i], raw->u.movcl[i], raw->u.smkdev[i],
				raw->u.start_mo[i], raw->u.end_mo[i],
				raw->u.sound[i], raw->u.wpn_snd[i].s1, raw->u.wpn_snd[i].s2, raw->u.wpn_snd[i].s3, raw->u.wpn_snd[i].s4,
				raw->u.__data75[i],
				raw->u.icon_winter[i], raw->u.icon_desert[i],
				raw->u.__dataX[ 0*SPWOOB_UCNT+i], raw->u.__dataX[ 1*SPWOOB_UCNT+i],
				raw->u.__dataX[ 2*SPWOOB_UCNT+i], raw->u.__dataX[ 3*SPWOOB_UCNT+i],
				raw->u.__dataX[ 4*SPWOOB_UCNT+i], raw->u.__dataX[ 5*SPWOOB_UCNT+i],
				raw->u.__dataX[ 6*SPWOOB_UCNT+i], raw->u.__dataX[ 7*SPWOOB_UCNT+i],
				raw->u.__dataX[ 8*SPWOOB_UCNT+i], raw->u.__dataX[ 9*SPWOOB_UCNT+i],
				raw->u.__dataX[10*SPWOOB_UCNT+i], raw->u.__dataX[11*SPWOOB_UCNT+i],
				raw->u.__dataX[12*SPWOOB_UCNT+i], raw->u.__dataX[13*SPWOOB_UCNT+i],
				raw->u.__dataX[14*SPWOOB_UCNT+i], raw->u.__dataX[15*SPWOOB_UCNT+i],
				raw->u.__dataX[16*SPWOOB_UCNT+i], raw->u.__dataX[17*SPWOOB_UCNT+i],
				raw->u.__dataX[18*SPWOOB_UCNT+i], raw->u.__dataX[19*SPWOOB_UCNT+i],
				raw->u.__dataX[20*SPWOOB_UCNT+i], raw->u.__dataX[21*SPWOOB_UCNT+i],
				raw->u.__dataX[22*SPWOOB_UCNT+i], raw->u.__dataX[23*SPWOOB_UCNT+i],
				raw->u.__dataX[24*SPWOOB_UCNT+i], raw->u.__dataX[25*SPWOOB_UCNT+i]
				);
		}
		fclose (file);
	}

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "raw_%s_%02u_f.csv", base, id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file,
			"id,name,"
			"nation,start_yr,end_yr,pscr,type,"
			"URID[1],URID[2],URID[3],URID[4],URID[5],URID[6],URID[7],URID[8],URID[9],URID[10],"
			"start_mo,end_mo,exp_mod,mor_mod,"
			"X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16"
			"\n");

		for (i=1; i<SPWOOB_FCNT; i++) {
			azstrcpy (raw->f.name[i].data, azsname);
			fprintf (file,
				"%d,%s,"
				"%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u"
				"\n",
				i, azsname,
				raw->f.nation[i], raw->f.start_yr[i], raw->f.end_yr[i], raw->f.pscr[i], raw->f.type[i],
				raw->f.urid[i].dat[0], raw->f.urid[i].dat[1],
				raw->f.urid[i].dat[2], raw->f.urid[i].dat[3],
				raw->f.urid[i].dat[4], raw->f.urid[i].dat[5],
				raw->f.urid[i].dat[6], raw->f.urid[i].dat[7],
				raw->f.urid[i].dat[8], raw->f.urid[i].dat[9],
				raw->f.start_mo[i], raw->f.end_mo[i], raw->f.exp_mod[i], raw->f.mor_mod[i],
				raw->f.__dataX[ 0*SPWOOB_FCNT+i], raw->f.__dataX[ 1*SPWOOB_FCNT+i],
				raw->f.__dataX[ 2*SPWOOB_FCNT+i], raw->f.__dataX[ 3*SPWOOB_FCNT+i],
				raw->f.__dataX[ 4*SPWOOB_FCNT+i], raw->f.__dataX[ 5*SPWOOB_FCNT+i],
				raw->f.__dataX[ 6*SPWOOB_FCNT+i], raw->f.__dataX[ 7*SPWOOB_FCNT+i],
				raw->f.__dataX[ 8*SPWOOB_FCNT+i], raw->f.__dataX[ 9*SPWOOB_FCNT+i],
				raw->f.__dataX[10*SPWOOB_FCNT+i], raw->f.__dataX[11*SPWOOB_FCNT+i],
				raw->f.__dataX[12*SPWOOB_FCNT+i], raw->f.__dataX[13*SPWOOB_FCNT+i],
				raw->f.__dataX[14*SPWOOB_FCNT+i], raw->f.__dataX[15*SPWOOB_FCNT+i]
				);
		}
		fclose (file);
	}
}