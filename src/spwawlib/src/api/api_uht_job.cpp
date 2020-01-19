/** \file
 * The SPWaW Library - unit history tracking list job API implementation.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_uht_job.h>
#include "common/internal.h"

SPWAWLIB_API SPWAW_ERROR
SPWAW_UHT_list_init (SPWAW_UHT_LIST_JOB_TYPE type, unsigned int len, SPWAW_UHT_LIST **list)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_UHT_LIST	*p;

	CNULLARG (list);
	*list = NULL;

	p = safe_malloc (SPWAW_UHT_LIST); COOMGOTO (p, "SPWAW_UHT_LIST", handle_error);

	p->type = type;
	p->len = len;
	p->cnt = 0;

	if (p->len) {
		p->list = safe_nmalloc (SPWAW_UHT_LISTEL, p->len);
		COOMGOTO (p->list, "SPWAW_UHT_LISTEL list", handle_error);
		p->smap = safe_nmalloc (SPWAW_UHT_LISTEL *, p->len);
		COOMGOTO (p->smap, "SPWAW_UHT_LISTEL smap", handle_error);
	}

	*list = p;

	return (SPWERR_OK);

handle_error:
	if (p) {
		if (p->smap) safe_free (p->smap);
		if (p->list) safe_free (p->list);
		safe_free (p);
	}
	return (rc);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_UHT_list_free (SPWAW_UHT_LIST **list)
{
	SPWAW_UHT_LIST	*p;

	CNULLARG (list);
	p = *list; *list = NULL;

	if (p) {
		if (p->smap) safe_free (p->smap);
		if (p->list) safe_free (p->list);
		safe_free (p);
	}

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_UHT_list_add (SPWAW_UHT_LIST *list, SPWAW_UHTE *uhte, SPWAW_UHT_LISTEL **listel)
{
	unsigned int		idx;
	SPWAW_UHT_LISTEL	*p;

	CNULLARG (list); CNULLARG (uhte); CNULLARG (listel);
	*listel = NULL;

	if (list->cnt >= list->len) RWE (SPWERR_OUTOFMEM, "no room left in SPWAW_UHT_LIST");

	idx = list->cnt++;
	p = &(list->list[idx]); list->smap[idx] = p;
	p->uhte = uhte;
	*listel = p;

	return (SPWERR_OK);
}

SPWAWLIB_API inline int
SPWAW_UHT_list_cmp_UID_ascending (const void *a, const void *b)
{
	SPWAW_UHT_LISTEL	*ea = *((SPWAW_UHT_LISTEL **)a);
	SPWAW_UHT_LISTEL	*eb = *((SPWAW_UHT_LISTEL **)b);

	int fma = ea->uhte->FMID;
	int fmb = eb->uhte->FMID;

	int fsa = ea->uhte->FSID;
	int fsb = eb->uhte->FSID;

	return ((fma==fmb)?((fsa==fsb)?0:((fsa<fsb)?-1:+1)):((fma<fmb)?-1:+1));
}

SPWAWLIB_API int
SPWAW_UHT_list_cmp_UID_descending (const void *a, const void *b)
{
	return (-SPWAW_UHT_list_cmp_UID_ascending(a,b));
}

SPWAWLIB_API int
SPWAW_UHT_list_cmp_CNT_ascending (const void *a, const void *b)
{
	SPWAW_UHT_LISTEL	*ea = *((SPWAW_UHT_LISTEL **)a);
	SPWAW_UHT_LISTEL	*eb = *((SPWAW_UHT_LISTEL **)b);

	if (ea->count == eb->count) return (SPWAW_UHT_list_cmp_UID_ascending(a,b));
	return ((ea->count < eb->count)?-1:+1);
}

SPWAWLIB_API int
SPWAW_UHT_list_cmp_CNT_descending (const void *a, const void *b)
{
	SPWAW_UHT_LISTEL	*ea = *((SPWAW_UHT_LISTEL **)a);
	SPWAW_UHT_LISTEL	*eb = *((SPWAW_UHT_LISTEL **)b);

	if (ea->count == eb->count) return (SPWAW_UHT_list_cmp_UID_ascending(a,b));
	return ((ea->count < eb->count)?+1:-1);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_UHT_list_sort (SPWAW_UHT_LIST *list, SPWAW_UHT_LIST_SORT type, bool ascending, SPWAW_UHT_list_cmp cmp)
{
	SPWAW_UHT_list_cmp	cmpfunc;

	CNULLARG (list);

	switch (type) {
		default:
		case SPWAW_UHT_SORT_UID:
			if (ascending) 
				cmpfunc = SPWAW_UHT_list_cmp_UID_ascending;
			else
				cmpfunc = SPWAW_UHT_list_cmp_UID_descending;
			break;
		case SPWAW_UHT_SORT_COUNT:
			if (ascending) 
				cmpfunc = SPWAW_UHT_list_cmp_CNT_ascending;
			else
				cmpfunc = SPWAW_UHT_list_cmp_CNT_descending;
			break;
		case SPWAW_UHT_SORT_USER:
			CNULLARG (cmp);
			cmpfunc = cmp;
			break;
	}
	qsort (list->smap, list->cnt, sizeof (SPWAW_UHTE *), cmpfunc);

	return (SPWERR_OK);
}

static SPWAW_ERROR
perform_uht_chain_list_job (SPWAW_UHT_LIST_JOB &job)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	SPWAW_UHTE		*uhte, *uptr, *nuptr;
	SPWAW_UHT_LISTEL	*ulle;
	bool			first;

	CNULLARG (job.src.uht);

	rc = SPWAW_UHT_list_init (job.type, job.src.uht->icnt, &(job.dst)); ROE ("SPWAW_UHT_list_init()");

	for (unsigned int i=0; i<job.src.uht->cnt; i++) {
		uhte = job.src.uht->smap[i];
		if (!SPWAW_UHT_is_initial (uhte)) continue;
		bool is_decommissioned = SPWAW_UHT_is_decommissioned (uhte);
		if (is_decommissioned && !job.allow_decomm) continue;

		if (!job.reversed) {
			uptr = SPWAW_UHT_first (uhte, job.status);
			if (!uptr) continue;

			rc = SPWAW_UHT_list_add (job.dst, SPWAW_UHT_last (uhte), &ulle); ROE ("SPWAW_UHT_list_add()");

			if (job.data_cb) {
				SPWAW_UHT_LIST_CBCTX cbctx = {
					uhte,
					&(uhte->FBI),
					SPWAW_UHT_lookup (uhte, &(uhte->FBI)),
					NULL,
					NULL,
					is_decommissioned,
					true,
					false,
					&(ulle->data),
					job.extra
				};
				job.data_cb (cbctx);
			}

			while (uptr) {
				ulle->count++;

				nuptr = SPWAW_UHT_next (uptr, job.status);

				if (job.data_cb) {
					SPWAW_UHT_LIST_CBCTX cbctx = {
						uptr,
						&(uptr->FBI),
						SPWAW_UHT_lookup (uptr, &(uptr->FBI)),
						NULL,
						NULL,
						is_decommissioned,
						false,
						!nuptr,
						&(ulle->data),
						job.extra
					};
					job.data_cb (cbctx);
				}

				uptr = nuptr;
			}
		} else {
			uptr = SPWAW_UHT_last (uhte, job.status);
			if (!uptr) continue;

			rc = SPWAW_UHT_list_add (job.dst, uptr, &ulle); ROE ("SPWAW_UHT_list_add()");

			first = true;
			while (uptr) {
				ulle->count++;

				nuptr = SPWAW_UHT_prev (uptr, job.status);

				if (job.data_cb) {
					SPWAW_UHT_LIST_CBCTX cbctx = {
						uptr,
						&(uptr->FBI),
						SPWAW_UHT_lookup (uptr, &(uptr->FBI)),
						NULL,
						NULL,
						is_decommissioned,
						first,
						false,
						&(ulle->data),
						job.extra
					};
					job.data_cb (cbctx);
				}

				uptr = nuptr;
				first = false;
			}
			if (job.data_cb) {
				SPWAW_UHT_LIST_CBCTX cbctx = {
					uhte,
					&(uhte->FBI),
					SPWAW_UHT_lookup (uhte, &(uhte->FBI)),
					NULL,
					NULL,
					is_decommissioned,
					false,
					true,
					&(ulle->data),
					job.extra
				};
				job.data_cb (cbctx);
			}
		}
	}

	return (SPWERR_OK);
}

static SPWAW_ERROR
perform_uht_binfo_list_job (SPWAW_UHT_LIST_JOB &job)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	bool			post = false;
	SPWAW_UHTE		*uhte;
	SPWAW_UHT_LISTEL	*ulle;

	CNULLARG (job.src.bid.bid); CNULLARG (job.src.bid.nbd);

	post = ((job.status & UHT_POST_BATTLE) != 0);

	rc = SPWAW_UHT_list_init (job.type, job.src.bid.bid->cnt, &(job.dst)); ROE ("SPWAW_UHT_list_init()");

	for (unsigned int i=0; i<job.src.bid.bid->cnt; i++) {
		uhte = job.src.bid.bid->list[i];
		bool is_decommissioned = SPWAW_UHT_is_decommissioned (uhte);
		if (is_decommissioned && !job.allow_decomm) continue;
		if (post) {
			if (!SPWAW_UHT_is (uhte, job.src.bid.nbd, job.status)) continue;
		} else {
			if (!SPWAW_UHT_is (uhte, &(job.src.bid.bid->bdate), job.status)) continue;
		}

		rc = SPWAW_UHT_list_add (job.dst, uhte, &ulle); ROE ("SPWAW_UHT_list_add()");
		ulle->count = 1;

		if (job.data_cb) {
			SPWAW_UHT_LIST_CBCTX cbctx = {
				uhte,
				&(job.src.bid.bid->bdate),
				SPWAW_UHT_lookup (uhte, &(job.src.bid.bid->bdate)),
				NULL,
				NULL,
				is_decommissioned,
				true,
				true,
				&(ulle->data),
				job.extra
			};
			if (post) {
				cbctx.nbdate = job.src.bid.nbd;
				cbctx.nuir = SPWAW_UHT_lookup (uhte, job.src.bid.nbd);
			}
			job.data_cb (cbctx);
		}

	}

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_UHT_list_job (SPWAW_UHT_LIST_JOB &job)
{
	SPWAW_ERROR		rc = SPWERR_OK;

	switch (job.type) {
		default:
		case SPWAW_UHT_LIST_CHAIN:
			rc = perform_uht_chain_list_job (job);
			break;
		case SPWAW_UHT_LIST_BINFO:
			rc = perform_uht_binfo_list_job (job);
			break;
	}

	return (rc);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_UHT_list_job_sort (SPWAW_UHT_LIST_JOB &job, SPWAW_UHT_LIST_SORT type, bool ascending, SPWAW_UHT_list_cmp cmp)
{
	return (SPWAW_UHT_list_sort (job.dst, type, ascending, cmp));
}