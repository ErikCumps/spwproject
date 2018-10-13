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
		fprintf (file, "RAWOOB-cmt     : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,cmt), sizeof(raw->cmt), sizeof(raw->cmt));
		fprintf (file, "RAWOOB-  w     : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,w), sizeof(raw->w), sizeof(raw->w));
		fprintf (file, "RAWOOB-  w-name: 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,w.name), sizeof(raw->w.name), sizeof(raw->w.name));
		fprintf (file, "RAWOOB-  w-d00 : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,w.__data00), sizeof(raw->w.__data00), sizeof(raw->w.__data00));
		//fprintf (file, "RAWOOB-pad0    : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,__padding0), sizeof(raw->__padding0), sizeof(raw->__padding0));
		fprintf (file, "RAWOOB-  u     : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,u), sizeof(raw->u), sizeof(raw->u));
		fprintf (file, "RAWOOB-  u-name: 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,u.name), sizeof(raw->u.name), sizeof(raw->u.name));
		fprintf (file, "RAWOOB-  u-d00 : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,u.__data00), sizeof(raw->u.__data00), sizeof(raw->u.__data00));
		//fprintf (file, "RAWOOB-pad1    : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,__padding1), sizeof(raw->__padding1), sizeof(raw->__padding1));
		fprintf (file, "RAWOOB-  f     : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,f), sizeof(raw->f), sizeof(raw->f));
		fprintf (file, "RAWOOB-  f-name: 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,f.name), sizeof(raw->f.name), sizeof(raw->f.name));
		fprintf (file, "RAWOOB-  f-d00 : 0x%8.8x - size=0x%8.8x (%u)\n", offsetof(RAWOOB,f.__data00), sizeof(raw->f.__data00), sizeof(raw->f.__data00));
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
			"B00,B01,B02,B03,B04,B05,B06,B07,B08,B09,B10,B11,B12,B13,"
			"X0,X1,X2,X3,X4,X5,X6,X7,X8,X9,X10,X11"
			"\n");

		for (i=1; i<SPWOOB_WCNT; i++) {
			azstrcpy (raw->w.name[i].data, azsname);
			fprintf (file,
				"%d,%s,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u"
				"\n",
				i, azsname,
				raw->w.__data00[i], raw->w.__data01[i], raw->w.__data02[i], raw->w.__data03[i],
				raw->w.__data04[i], raw->w.__data05[i], raw->w.__data06[i], raw->w.__data07[i],
				raw->w.__data08[i], raw->w.__data09[i], raw->w.__data10[i], raw->w.__data11[i],
				raw->w.__data12[i], raw->w.__data13[i],
				raw->w.__datax[11*i+0], raw->w.__datax[11*i+1], raw->w.__datax[11*i+2], raw->w.__datax[11*i+3],
				raw->w.__datax[11*i+4], raw->w.__datax[11*i+5], raw->w.__datax[11*i+6], raw->w.__datax[11*i+7],
				raw->w.__datax[11*i+8], raw->w.__datax[11*i+9], raw->w.__datax[11*i+10]
				);
		}
		fclose (file);
	}

	//memset (name, 0, sizeof (name));
	//snprintf (name, sizeof (name) - 1, "raw_%s_%02u_padding0.dat", base, id);
	//file = fopen (name, "w");
	//if (file) {
	//	fwrite (&(raw->__padding0), sizeof(raw->__padding0), 1, file);
	//	fclose (file);
	//}

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "raw_%s_%02u_u.csv", base, id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file,
			"id,name,"
			"B00,B01,B02,B03,B04,B05,B06,B07,B08,B09,"
			"B10,B11,B12,B13,B14,B15,B16,B17,B18,B19,"
			"B20,B21,B22,B23,B24,B25,B26,B27,B28,B29,"
			"B30,B31,B32,B33,B34,B35,B36,B37,B38,B39,"
			"B40,B41,B42,B43,B44,B45,B46,B47,B48,B49,"
			"B50,B51,B52,B53,B54,B55,B56,B57,B58,B59,"
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
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u"
				"\n",
				i, azsname,
				raw->u.__data00[i], raw->u.__data01[i], raw->u.__data02[i], raw->u.__data03[i], raw->u.__data04[i],
				raw->u.__data05[i], raw->u.__data06[i], raw->u.__data07[i], raw->u.__data08[i], raw->u.__data09[i],
				raw->u.__data10[i], raw->u.__data11[i], raw->u.__data12[i], raw->u.__data13[i], raw->u.__data14[i],
				raw->u.__data15[i], raw->u.__data16[i], raw->u.__data17[i], raw->u.__data18[i], raw->u.__data19[i],
				raw->u.__data20[i], raw->u.__data21[i], raw->u.__data22[i], raw->u.__data23[i], raw->u.__data24[i],
				raw->u.__data25[i], raw->u.__data26[i], raw->u.__data27[i], raw->u.__data28[i], raw->u.__data29[i],
				raw->u.__data30[i], raw->u.__data31[i], raw->u.__data32[i], raw->u.__data33[i], raw->u.__data34[i],
				raw->u.__data35[i], raw->u.__data36[i], raw->u.__data37[i], raw->u.__data38[i], raw->u.__data39[i],
				raw->u.__data40[i], raw->u.__data41[i], raw->u.__data42[i], raw->u.__data43[i], raw->u.__data44[i],
				raw->u.__data45[i], raw->u.__data46[i], raw->u.__data47[i], raw->u.__data48[i], raw->u.__data49[i],
				raw->u.__data50[i], raw->u.__data51[i], raw->u.__data52[i], raw->u.__data53[i], raw->u.__data54[i],
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

	//memset (name, 0, sizeof (name));
	//snprintf (name, sizeof (name) - 1, "raw_%s_%02u_padding1.dat", base, id);
	//file = fopen (name, "w");
	//if (file) {
	//	fwrite (&(raw->__padding1), sizeof(raw->__padding1), 1, file);
	//	fclose (file);
	//}

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "raw_%s_%02u_f.csv", base, id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file,
			"id,name,"
			"B00,B01,B02,B03,B04,B05,B06,B07,B08,B09,B10,B11,B12,B13,B14,"
			"UR1,UR2,UR3,UR4,UR5,UR6,UR7,UR8,UR9,UR10,"
			"UC1,UC2,UC3,UC4,UC5,UC6,UC7,UC8,UC9,UC10"
			"\n");

		for (i=1; i<SPWOOB_FCNT; i++) {
			azstrcpy (raw->f.name[i].data, azsname);
			fprintf (file,
				"%d,%s,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u"
				"\n",
				i, azsname,
				raw->f.__data00[i], raw->f.__data01[i], raw->f.__data02[i], raw->f.__data03[i],
				raw->f.__data04[i], raw->f.__data05[i], raw->f.__data06[i], raw->f.__data07[i],
				raw->f.__data08[i], raw->f.__data09[i], raw->f.__data10[i], raw->f.__data11[i],
				raw->f.__data12[i], raw->f.__data13[i], raw->f.__data14[i],
				raw->f.urid[i].data[0], raw->f.urid[i].data[1], raw->f.urid[i].data[2], raw->f.urid[i].data[3], raw->f.urid[i].data[4],
				raw->f.urid[i].data[5], raw->f.urid[i].data[6], raw->f.urid[i].data[7], raw->f.urid[i].data[8], raw->f.urid[i].data[9],
				raw->f.ucnt[i].data[0], raw->f.ucnt[i].data[1], raw->f.ucnt[i].data[2], raw->f.ucnt[i].data[3], raw->f.ucnt[i].data[4],
				raw->f.ucnt[i].data[5], raw->f.ucnt[i].data[6], raw->f.ucnt[i].data[7], raw->f.ucnt[i].data[8], raw->f.ucnt[i].data[9]
				);
		}
		fclose (file);
	}
}