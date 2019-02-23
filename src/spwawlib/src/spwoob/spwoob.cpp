/** \file
 * The SPWaW Library - SPWaW OOB handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_spwoob.h>
#include "spwoob/raw.h"
#include "common/internal.h"

void
spwoob_free (SPWOOB *oob)
{
	int	i;

	if (!oob) return;

	for (i=0; i<SPWOOB_DCNT; i++) {
		if (oob->data[i]) {
			if (oob->data[i]->wdata) safe_free (oob->data[i]->wdata);
			if (oob->data[i]->udata) safe_free (oob->data[i]->udata);
			if (oob->data[i]->fdata) safe_free (oob->data[i]->fdata);
			if (oob->data[i]->edata) safe_free (oob->data[i]->edata);
			if (oob->data[i]->rdata) safe_free (oob->data[i]->rdata);
			safe_free (oob->data[i]);
		}
	}
	clear_ptr (oob);
}


SPWAW_ERROR
SPWOOB_new (SPWOOB **oob)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWOOB		*p = NULL;

	CNULLARG (oob);
	*oob = NULL;

	p = safe_malloc (SPWOOB); COOMGOTO (p, "SPWOOB", handle_error);

	*oob = p;
	return (SPWERR_OK);

handle_error:
	if (p) safe_free (p);
	return (rc);
}

SPWAW_ERROR
SPWOOB_copy (SPWOOB *dst, SPWOOB *src)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	int		i, j;

	CNULLARG (dst); CNULLARG (src);

	memcpy (dst, src, sizeof (*dst));
	for (i=0; i<SPWOOB_DCNT; i++) {
		if (!src->data[i]) continue;

		dst->data[i] = safe_malloc (SPWOOB_DATA);
		COOMGOTO (dst->data[i], "SPWOOB_DATA", handle_error);
		memcpy (dst->data[i], src->data[i], sizeof (*(dst->data[i])));

		// Fix up parent pointer
		dst->data[i]->spwoob = dst;

		// Copy OOB data, if present
		if (src->data[i]->wdata && src->data[i]->wcnt) {
			dst->data[i]->wcnt = src->data[i]->wcnt;
			dst->data[i]->wdata = safe_nmalloc (SPWOOB_WDATA, dst->data[i]->wcnt);
			COOMGOTO (dst->data[i]->wdata, "SPWOOB_WDATA weapon data", handle_error);
			memcpy (dst->data[i]->wdata, src->data[i]->wdata, dst->data[i]->wcnt * sizeof (SPWOOB_WDATA));
		}
		if (src->data[i]->udata && src->data[i]->ucnt) {
			dst->data[i]->ucnt = src->data[i]->ucnt;
			dst->data[i]->udata = safe_nmalloc (SPWOOB_UDATA, dst->data[i]->ucnt);
			COOMGOTO (dst->data[i]->udata, "SPWOOB_UDATA unit data", handle_error);
			memcpy (dst->data[i]->udata, src->data[i]->udata, dst->data[i]->ucnt * sizeof (SPWOOB_UDATA));
		}
		if (src->data[i]->fdata && src->data[i]->fcnt) {
			dst->data[i]->fcnt = src->data[i]->fcnt;
			dst->data[i]->fdata = safe_nmalloc (SPWOOB_FDATA, dst->data[i]->fcnt);
			COOMGOTO (dst->data[i]->fdata, "SPWOOB_FDATA formation data", handle_error);
			memcpy (dst->data[i]->fdata, src->data[i]->fdata, dst->data[i]->fcnt * sizeof (SPWOOB_FDATA));
		}
		if (src->data[i]->edata && src->data[i]->fmecnt && dst->data[i]->fcnt) {
			dst->data[i]->fmecnt = src->data[i]->fmecnt;
			dst->data[i]->edata = safe_nmalloc (SPWOOB_EDATA, dst->data[i]->fcnt * dst->data[i]->fmecnt);
			COOMGOTO (dst->data[i]->edata, "SPWOOBE_DATA formation elements data", handle_error);
			memcpy (dst->data[i]->edata, src->data[i]->edata, dst->data[i]->fcnt * dst->data[i]->fmecnt * sizeof(SPWOOB_EDATA));

			for (j=0; j<dst->data[i]->fcnt; j++) {
				dst->data[i]->fdata[j].elements = &(((SPWOOB_EDATA *)dst->data[i]->edata)[j*dst->data[i]->fmecnt]);
			}
		}

		// Copy raw OOB data, if present
		if (src->data[i]->rdata && src->data[i]->rsize) {
			dst->data[i]->rsize = src->data[i]->rsize;
			dst->data[i]->rdata = safe_nmalloc (char, dst->data[i]->rsize);
			COOMGOTO (dst->data[i]->rdata, "SPWOOB_DATA raw data", handle_error);
			memcpy (dst->data[i]->rdata, src->data[i]->rdata, dst->data[i]->rsize);
		}
	}

	return (SPWERR_OK);

handle_error:
	spwoob_free (dst);
	return (rc);
}

void
SPWOOB_free (SPWOOB **oob)
{
	SPWOOB	*p;

	if (!oob || !*oob) return;

	p = *oob; *oob = NULL;

	spwoob_free (p);
	safe_free (p);
}

SPWAW_ERROR
SPWOOB_compare (SPWOOB *oob1, SPWOOB *oob2)
{
	int	i;

	CNULLARG (oob1); CNULLARG (oob2);

	if (oob1->count != oob2->count) RWE (SPWERR_NOMATCH_OOBDATA, "oob counts do not match");

	for (i=0; i<SPWOOB_DCNT; i++) {
		if (oob1->data[i] == oob2->data[i]) continue;

		if (oob1->data[i] == NULL) RWE (SPWERR_NOMATCH_OOBDATA, "OOB data missing for SPWOOB #1");
		if (oob2->data[i] == NULL) RWE (SPWERR_NOMATCH_OOBDATA, "OOB data missing for SPWOOB #2");
		if (oob1->data[i]->id != oob2->data[i]->id) RWE (SPWERR_NOMATCH_OOBDATA, "OOB ID mismatch");
		if (oob1->data[i]->crc32 != oob2->data[i]->crc32) RWE (SPWERR_NOMATCH_OOBDATA, "OOB CRC mismatch");
	}

	return (SPWERR_OK);
}

SPWOOB_DATA	*
spwoob_data (SPWOOB *oob, BYTE id)
{
	if (!oob) return (NULL);
	if (!oob->data[id]) return (NULL);

	return (oob->data[id]);
}

static void
spwoob_dump_data (SPWOOB_DATA *data, char *base)
{
	char	name[MAX_PATH+1];
	FILE	*file;
	int	i;

	if (!data || !base) return;

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "%s_%02u_w.csv", base, data->id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file,
			"id,name,"
			"wclass,wclass,"
			"size,warhead,"
			"kill_HE,kill_AP,"
			"pen_HE,pen_AP,pen_HEAT,pen_APCR,"
			"accuracy,range_max,range_APCR\n");

		for (i=0; i<data->wcnt; i++) {
			if (!data->wdata[i].valid) continue;

			SPWOOB_WDATA *wp = &(data->wdata[i]);
			fprintf (file,
				"%d,%s,"
				"%d,%s,"
				"%u,%u,"
				"%u,%u,"
				"%u,%u,%u,%u,"
				"%u,"
				"%u,%u\n",
				i, wp->name, wp->wclass, SPWOOB_WCLASS_lookup (wp->wclass),
				wp->size, wp->warhead,
				wp->kill_HE, wp->kill_AP,
				wp->pen_HE, wp->pen_AP, wp->pen_HEAT, wp->pen_APCR,
				wp->accuracy,
				wp->range_max, wp->range_APCR);
		}
		fclose (file);
	}

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "%s_%02u_u.csv", base, data->id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file,
			"id,nation,name,"
			"type,type,"
			"uclass,uclass,"
			"mclass,mclass,"
			"start_yr,start_mo,end_yr,end_mo,"
			"size,crew,survive,cost,speed,"
			"radio,irvis,fc,rf,stab,rof,"
			"load_cap,load_cost,"
			"swim,smkdev,"
			"wpn1,wpn1_HEammo,wpn1_APammo,wpn1_HEATammo,wpn1_APCRammo,"
			"wpn2,wpn2_HEammo,wpn2_APammo,"
			"wpn3,wpn3_HEammo,wpn3_APammo,"
			"wpn4,wpn4_HEammo,wpn4_APammo,"
			"arm_HE_FH,arm_HE_SH,arm_HE_RH,"
			"arm_HE_FT,arm_HE_ST,arm_HE_RT,"
			"arm_HE_TP,arm_HE_SK,"
			"arm_AP_FH,arm_AP_SH,arm_AP_RH,"
			"arm_AP_FT,arm_AP_ST,arm_AP_RT,"
			"arm_AP_TP,arm_AP_SK,"
			"slp_FH,slp_SH,slp_RH,"
			"slp_FT,slp_ST,slp_RT,"
			"sound,"
			"lbm,"
			"text,"
			"icon,icon_desert,icon_winter\n");

		for (i=0; i<data->ucnt; i++) {
			if (!data->udata[i].valid) continue;

			SPWOOB_UDATA *up = &(data->udata[i]);
			fprintf (file,
				"%d,%u,%s,"
				"%d,%s,"
				"%d,%s,"
				"%d,%s,"
				"%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,"
				"%u,%u,"
				"%u,%u,"
				"%u,%u,%u,%u,%u,"
				"%u,%u,%u,"
				"%u,%u,%u,"
				"%u,%u,%u,"
				"%u,%u,%u,"
				"%u,%u,%u,"
				"%u,%u,"
				"%u,%u,%u,"
				"%u,%u,%u,"
				"%u,%u,"
				"%u,%u,%u,"
				"%u,%u,%u,"
				"%u,"
				"%u,"
				"%u,"
				"%u,%u,%u\n",
				i, up->nation,up->name,
				up->type,SPWOOB_UTYPE_lookup(up->type),
				up->uclass,SPWOOB_UCLASS_lookup(up->uclass),
				up->mclass,SPWOOB_MOVCL_lookup(up->mclass),
				up->start_yr,up->start_mo,up->end_yr,up->end_mo,
				up->size,up->crew,up->survive,up->cost,up->speed,
				up->radio,up->irvis,up->fc,up->rf,up->stab,up->rof,
				up->load_cap,up->load_cost,
				up->swim,up->smkdev,
				up->wpn1,up->wpn1_HEammo,up->wpn1_APammo,up->wpn1_HEATammo,up->wpn1_APCRammo,
				up->wpn2,up->wpn2_HEammo,up->wpn2_APammo,
				up->wpn3,up->wpn3_HEammo,up->wpn3_APammo,
				up->wpn4,up->wpn4_HEammo,up->wpn4_APammo,
				up->arm_HE_FH,up->arm_HE_SH,up->arm_HE_RH,
				up->arm_HE_FT,up->arm_HE_ST,up->arm_HE_RT,
				up->arm_HE_TP,up->arm_HE_SK,
				up->arm_AP_FH,up->arm_AP_SH,up->arm_AP_RH,
				up->arm_AP_FT,up->arm_AP_ST,up->arm_AP_RT,
				up->arm_AP_TP,up->arm_AP_SK,
				up->slp_FH,up->slp_SH,up->slp_RH,
				up->slp_FT,up->slp_ST,up->slp_RT,
				up->sound,
				up->lbm,
				up->text,
				up->icon, up->icon_desert, up->icon_winter);
		}
		fclose (file);
	}

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "%s_%02u_f.csv", base, data->id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file,
			"id,nation,name,"
			"stat,stat,"
			"type,type,"
			"pscr,pscr,"
			"start_yr,start_mo,end_yr,end_mo,"
			"exp_mod,mor_mod"
			);
		for (int j=0; j<10; j++) fprintf (file, ",element_rid[%d],element_cnt[%d]", j, j);
		fprintf (file, "\n");

		for (i=0; i<data->fcnt; i++) {
			if (!data->fdata[i].valid) continue;

			SPWOOB_FDATA *fp = &(data->fdata[i]);
			fprintf (file,
				"%d,%u,%s,"
				"%d,%s,"
				"%d,%s,"
				"%d,%s,"
				"%u,%u,%u,%u,"
				"%d,%d",
				i, fp->nation, fp->name,
				fp->stat, SPWOOB_FSTAT_lookup (fp->stat),
				fp->type, SPWOOB_FTYPE_lookup (fp->type),
				fp->pscr, SPWOOB_FPSCR_lookup (fp->pscr),
				fp->start_yr, fp->start_mo, fp->end_yr, fp->end_mo,
				fp->exp_mod, fp->mor_mod
				);
			for (int j=0; j<10; j++) fprintf (file, ",%u,%u", fp->elements[j].rid, fp->elements[j].cnt);
			fprintf (file, "\n");
		}
		fclose (file);
	}

}

void
spwoob_dump (SPWOOB *oob, char *base, bool raw)
{
	int	i;

	if (!oob || !base) return;

	for (i=0; i<SPWOOB_DCNT; i++) {
		if (oob->data[i]) {
			spwoob_dump_data (oob->data[i], base);
			if (raw) spwoob_dump_raw_data (oob->gametype, oob->data[i]->rdata, oob->data[i]->id, base);
		}
	}
}
