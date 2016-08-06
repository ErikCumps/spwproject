#include "stdafx.h"
#include <spwawlib_spwoob.h>
#include "common/internal.h"

void
spwoob_free (SPWOOB *oob)
{
	int	i;
	
	if (!oob) return;

	for (i=0; i<SPWOOB_DCNT; i++) {
		if (oob->data[i]) {
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
	int		i;

	CNULLARG (dst); CNULLARG (src);

	memcpy (dst, src, sizeof (*dst));
	for (i=0; i<SPWOOB_DCNT; i++) {
		if (!src->data[i]) continue;

		dst->data[i] = safe_malloc (SPWOOB_DATA);
		COOMGOTO (dst->data[i], "SPWOOB_DATA", handle_error);
		memcpy (dst->data[i], src->data[i], sizeof (*(dst->data[i])));

		if (!src->data[i]->rdata || !src->data[i]->rsize) continue;

		dst->data[i]->rsize = src->data[i]->rsize;
		dst->data[i]->rdata = safe_nmalloc (char, dst->data[i]->rsize);
		COOMGOTO (dst->data[i]->rdata, "SPWOOB_DATA raw data", handle_error);

		memcpy (dst->data[i]->rdata, src->data[i]->rdata, dst->data[i]->rsize);
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
	int	i, j;
	SPWOOB_WDATA	*wp;
	SPWOOB_UDATA	*up;
	SPWOOB_FDATA	*fp;

	if (!data || !base) return;

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "%s_%02u_w.csv", base, data->id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file,
			"id,name,wclass,wclass,"
			"size,warhead,kill,"
			"pen_AP,pen_HE,pen_HEAT,pen_APCR,"
			"accuracy,range_max,range_APCR\n");

		for (i=0; i<SPWOOB_WCNT; i++) {
			if (!data->wdata[i].valid) continue;

			wp = &(data->wdata[i]);
			fprintf (file, "%u,%s,%u,%s,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n",
				i, wp->name, wp->wclass, SPWOOB_WCLASS_lookup (wp->wclass),
				wp->size, wp->warhead, wp->kill,
				wp->pen_AP, wp->pen_HE, wp->pen_HEAT, wp->pen_APCR,
				wp->accuracy, wp->range_max, wp->range_APCR);
		}
		fclose (file);
	}

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "%s_%02u_u.csv", base, data->id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file,
			"id,nation,name,type,type,uclass,uclass,"
			"start_yr,start_mo,end_yr,end_mo,"
			"size,crew,survive,cost,speed,mclass,"
			"radio,irvis,fc,rf,stab,rof,load_cap,load_cost,swim,smkdev,"
			"wpn1,wpn1_HEammo,wpn1_APammo,wpn1_HEATammo,wpn1_APCRammo,"
			"wpn2,wpn2_HEammo,wpn2_APammo,wpn3,wpn3_HEammo,wpn3_APammo,wpn4,wpn4_HEammo,wpn4_APammo,"
			"arm_FH,arm_SH,arm_RH,arm_FT,arm_ST,arm_RT,arm_TP,arm_SK,"
			"slp_FH,slp_SH,slp_RH,slp_FT,slp_ST,slp_RT\n");

		for (i=0; i<SPWOOB_UCNT; i++) {
			if (!data->udata[i].valid) continue;

			up = &(data->udata[i]);
			fprintf (file,
				"%u,%u,%s,%u,%s,%u,%s,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u,%u,%u,"
				"%u,%u,%u,%u,%u,%u\n",
				i, up->nation,up->name,up->type,SPWOOB_UTYPE_lookup(up->type),up->uclass,SPWOOB_UCLASS_lookup(up->uclass),
				up->start_yr,up->start_mo,up->end_yr,up->end_mo,
				up->size,up->crew,up->survive,up->cost,up->speed,up->mclass,
				up->radio,up->irvis,up->fc,up->rf,up->stab,up->rof,up->load_cap,up->load_cost,up->swim,up->smkdev,
				up->wpn1,up->wpn1_HEammo,up->wpn1_APammo,up->wpn1_HEATammo,up->wpn1_APCRammo,
				up->wpn2,up->wpn2_HEammo,up->wpn2_APammo,up->wpn3,up->wpn3_HEammo,up->wpn3_APammo,up->wpn4,up->wpn4_HEammo,up->wpn4_APammo,
				up->arm_FH,up->arm_SH,up->arm_RH,up->arm_FT,up->arm_ST,up->arm_RT,up->arm_TP,up->arm_SK,
				up->slp_FH,up->slp_SH,up->slp_RH,up->slp_FT,up->slp_ST,up->slp_RT);
		}
		fclose (file);
	}

	memset (name, 0, sizeof (name));
	snprintf (name, sizeof (name) - 1, "%s_%02u_f.csv", base, data->id);
	file = fopen (name, "w");
	if (file) {
		fprintf (file, "id,nation,name,stat,stat,type,type,pscr,pscr,start_yr,start_mo,end_yr");
		for (j=0; j<10; j++) fprintf (file, ",unit_ids[%d],unit_cnt[%d]", j, j);
		fprintf (file, "\n");

		for (i=0; i<SPWOOB_FCNT; i++) {
			if (!data->fdata[i].valid) continue;

			fp = &(data->fdata[i]);
			fprintf (file, "%u,%u,%s,%u,%s,%u,%s,%u,%s,%u,%u,%u",
				i, fp->nation, fp->name,
				fp->stat, SPWOOB_FSTAT_lookup (fp->stat),
				fp->type, SPWOOB_FTYPE_lookup (fp->type),
				fp->pscr, SPWOOB_FPSCR_lookup (fp->pscr),
				fp->start_yr, fp->start_mo, fp->end_yr);
			for (j=0; j<10; j++) fprintf (file, ",%u,%u", fp->unit_ids[j], fp->unit_cnt[j]);
			fprintf (file, "\n");
		}
		fclose (file);
	}

}

void
spwoob_dump (SPWOOB *oob, char *base)
{
	int	i;

	if (!oob || !base) return;

	for (i=0; i<SPWOOB_DCNT; i++) {
		if (oob->data[i]) { spwoob_dump_data (oob->data[i], base); }
	}
}
