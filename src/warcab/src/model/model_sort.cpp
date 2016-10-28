/** \file
 * The SPWaW war cabinet - data model handling - sorting.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "model_oob_data.h"
#include "model_roster_data.h"
#include "model_sort.h"

#define	NCMP(a_,b_)		((a_ < b_) ? -1 : ((a_>b_) ? 1 : 0))
#define	IDXCMPGMO(a_,b_)	NCMP (a_->data.f->snap->data.idx, b_->data.f->snap->data.idx)
#define	IDXCMPGMR(a_,b_)	NCMP (a_->uir->snap->data.idx, b_->uir->snap->data.idx)
#define	DCMP(a_,b_)		((a_ < b_) ? -1 : ((a_>b_) ? 1 : 0))
#define	REV(c_)			((c_ < 0) ? 1 : ((c_ > 0) ? -1 : c_))

static inline int
MDL_stdcmp_core (MDL_CMPDATA &va, MDL_CMPDATA &vb, MDL_CMPTYPE type, bool rev)
{
	int	c = 0;

	switch (type) {
		case MDL_CMPINT:
			c = NCMP (va.i, vb.i);
			break;
		case MDL_CMPSTR:
			c = strcmp (va.s, vb.s);
			break;
		case MDL_CMPDBL:
			c = DCMP (va.d, vb.d);
			break;
		case MDL_CMPUID:
			int	fida, fsida, fidb, fsidb;
			MDL_UNPACK_UID (va.u, fida, fsida);
			MDL_UNPACK_UID (vb.u, fidb, fsidb);
			if (!(c = NCMP (fida, fidb))) c = NCMP (fsida, fsidb);
			break;
		default:
			c = 0;
			break;
	}
	if (rev) c = REV(c);
	return (c);
}

static inline int
MDL_stdcmp_dlt (SPWDLT *da, SPWDLT *db, bool rev)
{
	int	c = 0;

	DEVASSERT (da != NULL); DEVASSERT (db != NULL);

	SPWDLT_check (da); SPWDLT_check (db);

	if (da->type != db->type) return (0);
	if (!da->delta && !db->delta) return (0);

	switch (da->type) {
		case SPWDLT_BOOL:
			c = NCMP (SPWDLT_getbool (da), SPWDLT_getbool (db));
			break;
		case SPWDLT_INT:
			c = NCMP (SPWDLT_getint (da), SPWDLT_getint (db));
			break;
		case SPWDLT_STR:
			c = NCMP (SPWDLT_getstr (da), SPWDLT_getstr (db));
			break;
		case SPWDLT_DBL:
			c = DCMP (SPWDLT_getdbl (da), SPWDLT_getdbl (db));
			break;
		case SPWDLT_NONE:
		default:
			c = 0;
			break;
	}
	if (rev) c = REV(c);
	return (c);
}

inline int
MDL_stdcmp_GMO (const void *a, const void *b, MDL_CMPDEF *def)
{
	MDLO_SMAP	*pa = NULL, *pb = NULL;
	int		i, col;
	MDL_CMPDATA	va, vb;
	int		c = 0;

	if (!def) return (0);

	DEVASSERT (a != NULL); DEVASSERT (b != NULL);
	pa = (MDLO_SMAP *)a; pb = (MDLO_SMAP *)b;

	for (i=0; i<def->cnt; i++) {
		col = def->def[i].col;
		if (*(pa->data->dltsort)) {
			c = MDL_stdcmp_dlt (&(pa->data->dlt[col]), &(pb->data->dlt[col]), def->def[i].rev);
		}
		if (!c) {
			MDLO_data_sort (col, pa->data, va); MDLO_data_sort (col, pb->data, vb);
			c = MDL_stdcmp_core (va, vb, def->def[i].type, def->def[i].rev);
		}
		if (*(pa->data->revsort)) c = REV (c);
		if (c) break;
	}

	if (!c) c = IDXCMPGMO (pa->data, pb->data);
	return (c);
}

inline int
MDL_stdcmp_GMR (const void *a, const void *b, MDL_CMPDEF *def)
{
	MDLR_SMAP	*pa = NULL, *pb = NULL;
	int		i, col;
	MDL_CMPDATA	va, vb;
	int		c = 0;

	if (!def) return (0);

	DEVASSERT (a != NULL); DEVASSERT (b != NULL);
	pa = (MDLR_SMAP *)a; pb = (MDLR_SMAP *)b;

	for (i=0; i<def->cnt; i++) {
		col = def->def[i].col;
		if (*(pa->data->dltsort)) {
			c = MDL_stdcmp_dlt (&(pa->data->dlt[col]), &(pb->data->dlt[col]), def->def[i].rev);
		}
		if (!c) {
			MDLR_data_sort (col, pa->data, va); MDLR_data_sort (col, pb->data, vb);
			c = MDL_stdcmp_core (va, vb, def->def[i].type, def->def[i].rev);
		}
		if (*(pa->data->revsort)) c = REV (c);
		if (c) break;
	}

	if (!c) c = IDXCMPGMR (pa->data, pb->data);
	return (c);
}

int
MDL_stdcmp_GMO (const void *a, const void *b, MDL_CMPTYPE type, int col, bool rev)
{
	MDL_CMPDEF	def;

	memset (&def, 0, sizeof (def));
	def.cnt = 1;
	def.def[0].type = type;
	def.def[0].col  = col;
	def.def[0].rev  = rev;

	return (MDL_stdcmp_GMO (a, b, &def));
}

int
MDL_stdcmp_GMR (const void *a, const void *b, MDL_CMPTYPE type, int col, bool rev)
{
	MDL_CMPDEF	def;

	memset (&def, 0, sizeof (def));
	def.cnt = 1;
	def.def[0].type = type;
	def.def[0].col  = col;
	def.def[0].rev  = rev;

	return (MDL_stdcmp_GMR (a, b, &def));
}
