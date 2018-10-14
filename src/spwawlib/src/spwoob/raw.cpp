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
			"B00,"
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
				raw->w.__data0[i],
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
			"B17,B18,B19,"
			"B20,B21,B22,B23,B24,B25,B26,B27,B28,B29,"
			"B30,B31,B32,B33,B34,B35,B36,B37,B38,B39,"
			"B40,B41,B42,B43,"
			"radio,start_yr,end_yr,cost,nation,"
			//"B50,B51,"
			"lbm,"
			"B52,B53,B54,B55,B56,B57,B58,B59,"
			"B60,B61,B62,B63,B64,B65,B66,B67,B68,B69,"
			"B70,B71,B72,B73,B74,B75,B76,B77,B78,B79,"
			"B80,B81,B82,B83,B84,B85,B86,B87,B88,B89,"
			"B90,B91,B92,B93,B94,B95,B96,B97,B98,B99,"
			"B100,B101,B102,B103,B104,B105"
			"\n");

		for (i=1; i<SPWOOB_UCNT; i++) {
			azstrcpy (raw->u.name[i].data, azsname);
			fprintf (file,
				"%d,%s,"
				"%u,%u,%u,%u,"
				"%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,"
				"%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,"
				//"%u,%u,"
				"%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u"
				"\n",
				i, azsname,
				raw->u.uclass[i], raw->u.arm_SK[i], raw->u.size[i], raw->u.crew[i],
				raw->u.wpn[i].w1, raw->u.wpn[i].w2, raw->u.wpn[i].w3, raw->u.wpn[i].w4,
				raw->u.wpn_ammo[i].HE1, raw->u.wpn_ammo[i].AP1, raw->u.wpn_ammo[i].HE2, raw->u.wpn_ammo[i].AP2,
				raw->u.wpn_ammo[i].HE3, raw->u.wpn_ammo[i].AP3, raw->u.wpn_ammo[i].HE4, raw->u.wpn_ammo[i].AP4,
				raw->u.speed[i],
				raw->u.__data17[i], raw->u.__data18[i], raw->u.__data19[i],
				raw->u.__data20[i], raw->u.__data21[i], raw->u.__data22[i], raw->u.__data23[i], raw->u.__data24[i],
				raw->u.__data25[i], raw->u.__data26[i], raw->u.__data27[i], raw->u.__data28[i], raw->u.__data29[i],
				raw->u.__data30[i], raw->u.__data31[i], raw->u.__data32[i], raw->u.__data33[i], raw->u.__data34[i],
				raw->u.__data35[i], raw->u.__data36[i], raw->u.__data37[i], raw->u.__data38[i], raw->u.__data39[i],
				raw->u.__data40[i], raw->u.__data41[i], raw->u.__data42[i], raw->u.__data43[i],
				raw->u.radio[i], raw->u.start_yr[i], raw->u.end_yr[i], raw->u.cost[i], raw->u.nation[i],
				//raw->u.__data50[i], raw->u.__data51[i],
				raw->u.lbm[i],
				raw->u.__data52[i], raw->u.__data53[i], raw->u.__data54[i],
				raw->u.__data55[i], raw->u.__data56[i], raw->u.__data57[i], raw->u.__data58[i], raw->u.__data59[i],
				raw->u.__data60[i], raw->u.__data61[i], raw->u.__data62[i], raw->u.__data63[i], raw->u.__data64[i],
				raw->u.__data65[i], raw->u.__data66[i], raw->u.__data67[i], raw->u.__data68[i], raw->u.__data69[i],
				raw->u.__data70[i], raw->u.__data71[i], raw->u.__data72[i], raw->u.__data73[i], raw->u.__data74[i],
				raw->u.__data75[i], raw->u.__data76[i], raw->u.__data77[i], raw->u.__data78[i], raw->u.__data79[i],
				raw->u.__data80[i], raw->u.__data81[i], raw->u.__data82[i], raw->u.__data83[i], raw->u.__data84[i],
				raw->u.__data85[i], raw->u.__data86[i], raw->u.__data87[i], raw->u.__data88[i], raw->u.__data89[i],
				raw->u.__data90[i], raw->u.__data91[i], raw->u.__data92[i], raw->u.__data93[i], raw->u.__data94[i],
				raw->u.__data95[i], raw->u.__data96[i], raw->u.__data97[i], raw->u.__data98[i], raw->u.__data99[i],
				raw->u.__data100[i], raw->u.__data101[i], raw->u.__data102[i],
				raw->u.__data103[i], raw->u.__data104[i], raw->u.__data105[i]
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