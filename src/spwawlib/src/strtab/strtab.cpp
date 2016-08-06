/** \file
 * The SPWaW Library - string table handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "strtab.h"
#include "fileio/fileio.h"
#include "utils/textconv.h"
#include "common/internal.h"
#include "utils/compression.h"

#define	STRTAB_MAGIC	"SPWAWLIB_STRTAB"
#define	STRTAB_MAGICLEN	16
#define	STRTAB_VERSION	5

#pragma pack(push, r1, 1)

typedef struct s_STRTAB_HDR {
	char	magic[STRTAB_MAGICLEN];	/* STRTAB magic string				*/
	ULONG	version;		/* STRTAB file format version			*/
	ULONG	size;			/* data size					*/
	ULONG	comp;			/* compressed data size (0 if no compression)	*/
	ULONG	cnt;			/* symbol count					*/
	ULONG	buf;			/* STRBUF offset, relative to start of header	*/
} STRTAB_HDR;

#pragma pack(pop, r1)

#define	BADSTR	"!STRING_NOT_FOUND!"

#define	HASHBUCKETS	211

#define	MAXREFCNT	(ULONG_MAX-1)

#define	MAXIDXGROW	512

// djb2 string hash function
static inline unsigned long
string_hash (char *str)
{
	unsigned long	h = 5381;
	int		c;

	while ((c = *str++) != 0) h = ((h << 5) + h) + c; /* h * 33 + c */

	return (h);
}

static inline unsigned long
idforkey (void *key)
{
	unsigned long	id;

	if (key) {
		id = string_hash ((char *)key);
	} else {
		ERROR0 ("can not fetch id from NULL data pointer");
		id = 0;
	}

	return (id);
}

static unsigned int
hash_func (void *key)
{
	return (idforkey (key) % HASHBUCKETS);
}

static int
hash_match (void *key1, void *key2)
{
	return (!strcmp ((char *)key1, (char *)key2));
}

static SPWAW_ERROR
grow_idx (STRTAB *tab)
{
	ULONG	nilen;
	STRIDX	*nidx;

	CNULLARG (tab);

	if (tab->ilen) {
		if (tab->ilen < MAXIDXGROW) {
			nilen = tab->ilen * 2;
		} else {
			nilen = tab->ilen + MAXIDXGROW;
		}
		nidx = (STRIDX *)safe_realloc (tab->idx, nilen * sizeof (STRIDX));
	} else {
		nilen = 2;
		nidx = safe_nmalloc (STRIDX, nilen);
	}
	COOM (nidx, "STRIDX list");

	memset (nidx + tab->ilen, 0, (nilen - tab->ilen) * sizeof (STRIDX));
        tab->idx  = nidx;
	tab->ilen = nilen;

	return (SPWERR_OK);
}

SPWAW_ERROR
STRTAB_new (STRTAB **tab)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	STRTAB		*p;

	CNULLARG (tab);
	*tab = NULL;

	p = safe_malloc (STRTAB); COOMGOTO (p, "STRTAB", handle_error);

	if (STRBUF_new (&(p->buf))) {
		ERROR0 ("failed to create new STRBUF for STRTAB");
		goto handle_error;
	}

	p->hash = ADhash_setup (HASHBUCKETS, hash_func, hash_match, NULL);
	if (!p->hash) {
		ERROR0 ("failed to create new hash for STRTAB");
		goto handle_error;
	}

	*tab = p;
	return (SPWERR_OK);

handle_error:
	if (p) {
		if (p->hash) ADhash_destroy (p->hash);
		if (p->buf) STRBUF_free (&(p->buf));
		free (p);
	}
	return (SPWERR_FAILED);
}

void
STRTAB_free (STRTAB **tab)
{
	STRTAB	*p;

	if (!tab || !*tab) return;

	p = *tab; *tab = NULL;

	STRBUF_free (&(p->buf));
	if (p->idx) free (p->idx);
	if (p->hash) ADhash_destroy (p->hash);
	free (p);
}

void
STRTAB_clear (STRTAB *tab)
{
	if (!tab) return;

	tab->cnt = tab->icnt = 0;
	STRBUF_clear (tab->buf);
	if (tab->idx) memset (tab->idx, 0, tab->ilen * sizeof (STRIDX));
	ADhash_clear (tab->hash);
}

SPWAW_ERROR
STRTAB_init (STRTAB *tab, char *buffer, ULONG size)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	CHAR		*p, *q;
	ULONG		idx;

	CNULLARG (tab); CNULLARG (buffer);
	STRTAB_clear (tab);

	p = buffer;
	while (p) {
		if (p >= (buffer + size)) break;

		q = p; p = strchr (p, '\0'); if (!p) break;

		if ((q = STRBUF_add (tab->buf, q)) == NULL)
			FAILGOTO (SPWERR_FAILED, "STRBUF_add()", handle_error);

		if (tab->icnt >= tab->ilen) {
			rc = grow_idx (tab);
			ERRORGOTO ("grow_idx()", handle_error);
		}

		idx = tab->icnt;
		tab->idx[idx].ptr = q;
		tab->idx[idx].ref = 1;
		if (!ADhash_add (tab->hash, q, (void *)(idx+1)))
			FAILGOTO (SPWERR_FAILED, "ADhash_add()", handle_error);

		tab->cnt++; tab->icnt++; p++;
	}

	return (rc);

handle_error:
	ADhash_clear (tab->hash);
	STRBUF_clear (tab->buf);
	STRTAB_clear (tab);
	return (rc);
}

SPWAW_ERROR
STRTAB_load (STRTAB *tab, char *file)
{
	SPWAW_ERROR	rc;
	int		fd;

	CNULLARG (tab); CNULLARG (file);

	fd = open (file, _O_RDONLY|_O_BINARY);
	if (fd < 0) RWE (SPWERR_FOFAILED, "failed to open STRTAB file for reading");

	rc = STRTAB_fdload (tab, fd); close (fd);
	return (rc);
}

SPWAW_ERROR
STRTAB_fdload (STRTAB* tab, int fd)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		pos;
	STRTAB_HDR	hdr;
	char		*data = NULL;
	long		size;
	SBR		*sbr = NULL;
	char		*p;
	ULONG		i;
	CBIO		cbio;

	CNULLARG (tab);
	STRTAB_clear (tab);

	pos = bseekget (fd);

	if (!bread (fd, (char *)&hdr, sizeof (hdr), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(hdr)", handle_error);

	if ((memcmp (hdr.magic, STRTAB_MAGIC, STRTAB_MAGICLEN) != 0) || (hdr.version != STRTAB_VERSION))
		FAILGOTO (SPWERR_BADSTABDATA, "STRTAB header check", handle_error);

	if (!hdr.size) return (SPWERR_OK);

	tab->cnt = tab->icnt = hdr.cnt;

	while (hdr.cnt > tab->ilen) {
		rc = grow_idx (tab);
		ERRORGOTO ("grow_idx()", handle_error);
	}

	size = hdr.size;
	data = safe_smalloc (char, size); COOMGOTO (data, "STRTAB data buffer", handle_error);

	cbio.data = data; cbio.size = size; cbio.comp = &(hdr.comp);
	if (!cbread (fd, cbio, "STRTAB data")) FAILGOTO (SPWERR_FRFAILED, "cbread(STRTAB data) failed", handle_error);

	sbr = sbread_init (data, size);
	if (!sbr) FAILGOTO (SPWERR_FRFAILED, "sbread_init() failed", handle_error);

	for (i=0; i<hdr.cnt; i++) {
		if (sbread (sbr, (char *)&(tab->idx[i].ref), sizeof (tab->idx[i].ref)) != sizeof (tab->idx[i].ref))
			FAILGOTO (SPWERR_FRFAILED, "sbread(refcnt)", handle_error);
	}

	sbread_stop (sbr); sbr = NULL;
	safe_free (data);

	bseekset (fd, pos + hdr.buf);

	rc = STRBUF_fdload (tab->buf, fd);
	ERRORGOTO ("STRBUF_fdload()", handle_error);

	p = STRBUF_next (tab->buf, NULL); i = 0;
	while (p && (i<tab->icnt)) {
		if (!tab->idx[i].ref) {
			tab->idx[i].ptr = NULL;
		} else {
			tab->idx[i].ptr = p;
			if (!ADhash_add (tab->hash, p, (void *)(i+1)))
				FAILGOTO (SPWERR_FAILED, "ADhash_add()", handle_error);
		}
		p = STRBUF_next (tab->buf, p); i++;
	}

	return (SPWERR_OK);

handle_error:
	if (sbr) sbread_stop (sbr);
	if (data) safe_free (data);
	ADhash_clear (tab->hash);
	STRBUF_clear (tab->buf);
	STRTAB_clear (tab);
	return (rc);
}

SPWAW_ERROR
STRTAB_compact (STRTAB *tab)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	STRIDX		*nidx = NULL, *oidx = NULL;
	ULONG		ncnt, i, j;

	CNULLARG (tab);

	//{
	//	FILE	*rf = fopen ("STRTAB_report.txt", "a");
	//	debug_STRTAB_report (rf, tab, "before compacting");
	//	fclose (rf);
	//}

	// Count existing entries
	for (i=ncnt=0; i<tab->icnt; i++) if (tab->idx[i].ref) ncnt++;

	// Allocate new index
	nidx = safe_nmalloc (STRIDX, ncnt);
	COOMGOTO (nidx, "compact STRIDX", handle_error);

	// Fill new index
	for (i=j=0; i<tab->icnt; i++) {
		if (!tab->idx[i].ref) continue;
		memcpy (&(nidx[j++]), &(tab->idx[i]), sizeof (STRIDX));
	}

	// Replace index in stab and clean up
	oidx = tab->idx; tab->idx = nidx; tab->ilen = tab->icnt = ncnt;
	free (oidx); oidx = nidx = NULL;

	// Update hash table
	for (i=0; i<tab->icnt; i++) {
		if (!ADhash_del (tab->hash, tab->idx[i].ptr))
			FAILGOTO (SPWERR_FAILED, "ADhash_del()", handle_error);
		if (!ADhash_add (tab->hash, tab->idx[i].ptr, (void *)(i+1)))
			FAILGOTO (SPWERR_FAILED, "ADhash_add()", handle_error);
	}

	//{
	//	FILE	*rf = fopen ("STRTAB_report.txt", "a");
	//	debug_STRTAB_report (rf, tab, "after compacting");
	//	fclose (rf);
	//}

	return (SPWERR_OK);

handle_error:
	return (rc);
}

SPWAW_ERROR
STRTAB_save (STRTAB *tab, char *file, bool compress)
{
	SPWAW_ERROR	rc;
	int		fd;

	CNULLARG (tab); CNULLARG (file);

	fd = open (file, _O_CREAT|_O_TRUNC|_O_WRONLY|_O_BINARY, 0666);
	if (fd < 0) RWE (SPWERR_FOFAILED, "failed to open STRTAB file for writing");

	rc = STRTAB_fdsave (tab, fd, compress); close (fd);
	if (rc != SPWERR_OK) unlink (file);
	return (rc);
}

SPWAW_ERROR
STRTAB_fdsave (STRTAB *tab, int fd, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	STRTAB_HDR	hdr;
	ULONG		i, j;
	long		p0, p1;
	SBW		*sbw = NULL;
	char		*p = NULL;
	long		todo;
	CBIO		cbio;

	CNULLARG (tab);

	memset (&hdr, 0, sizeof (hdr));
	memcpy (hdr.magic, STRTAB_MAGIC, strlen (STRTAB_MAGIC));
	hdr.version = STRTAB_VERSION;

	p0 = bseekget (fd);
	bseekmove (fd, sizeof (hdr));

	sbw = sbwrite_init (NULL, 0);
	if (!sbw) FAILGOTO (SPWERR_FWFAILED, "sbwrite_init() failed", handle_error);

	for (i=j=0; (j<tab->cnt) && (i<tab->icnt); i++) {
		if (!tab->idx[i].ref) continue; // FIXME: do we risk mismatched indexes here???
		if (sbwrite (sbw, (char *)&(tab->idx[i].ref), sizeof (tab->idx[i].ref)) != sizeof (tab->idx[i].ref))
			FAILGOTO (SPWERR_FWFAILED, "sbwrite(refcnt)", handle_error);

		j++;
	}
	hdr.cnt = j;

	sbwrite_stop (sbw, &p, &todo); sbw = NULL;
	hdr.size = todo;

	cbio.data = p; cbio.size = todo; cbio.comp = &(hdr.comp);
	if (!cbwrite (fd, cbio, "STRTAB data", compress)) FAILGOTO (SPWERR_FWFAILED, "cbwrite(STRTAB data) failed", handle_error);
	safe_free (p);

	hdr.buf = bseekget (fd) - p0;

	rc = STRBUF_fdsave (tab->buf, fd, compress);
	ERRORGOTO ("STRBUF_fdsave()", handle_error);

	p1 = bseekget (fd); bseekset (fd, p0);
	if (!bwrite (fd, (char *)&hdr, sizeof (hdr)))
		FAILGOTO (SPWERR_FWFAILED, "bwrite(hdr)", handle_error);
	bseekset (fd, p1);

	return (rc);

handle_error:
	if (p) safe_free (p);
	if (sbw) sbwrite_stop (sbw);
	bseekset (fd, p0);
	return (rc);
}

SPWAW_ERROR
STRTAB_add (STRTAB *tab, char *str, STRINFO *inf)
{
	return (STRTAB_add (tab, str, 1, inf));
}

SPWAW_ERROR
STRTAB_add (STRTAB *tab, char *str, ULONG cnt, STRINFO *inf)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	ULONG		idx;
	char		*p = NULL;

	CNULLARG (tab); CNULLARG (str);

	if (inf) {
		inf->idx = BADSTRIDX;
		inf->str = BADSTR;
	}

	if (cnt > MAXREFCNT) RWE (SPWERR_BADSTABREFCNT, "invalid STRTAB reference count");

	idx = (ULONG)ADhash_get (tab->hash, str);

	if (idx) {
		idx--;
		if (tab->idx[idx].ref > (MAXREFCNT - cnt))
			RWE (SPWERR_BADSTABREFCNT, "invalid STRTAB item reference count");

		// Prevent warning C4244
		tab->idx[idx].ref = tab->idx[idx].ref + cnt;
	} else {
		if ((p = STRBUF_add (tab->buf, str)) == NULL)
			FAILGOTO (SPWERR_FAILED, "STRBUF_add()", handle_error);

		if ((tab->icnt+1) > tab->ilen) {
			rc = grow_idx (tab);
			ERRORGOTO ("grow_idx()", handle_error);
		}

		idx = tab->icnt;
		tab->idx[idx].ptr = p;
		tab->idx[idx].ref = cnt;
		if (!ADhash_add (tab->hash, p, (void *)(idx+1)))
			FAILGOTO (SPWERR_FAILED, "ADhash_add()", handle_error);

		tab->cnt++; tab->icnt++;
	}

	if(inf) {
		inf->idx = idx;
		inf->str = tab->idx[idx].ptr;
	}

	return (rc);

handle_error:
	if (p) {
		STRBUF_del (tab->buf, str);
		ADhash_del (tab->hash, (void *)(idx+1));
		tab->idx[idx].ptr = NULL;
		tab->idx[idx].ref = 0;
	}
	if (inf) {
		inf->idx = BADSTRIDX;
		inf->str = BADSTR;
	}
	return (rc);
}

char *
STRTAB_add (STRTAB *tab, char *str)
{
	STRINFO	si;

	if (STRTAB_add (tab, str, &si) != SPWERR_OK) return (NULL);
	return (si.str);
}

char *
STRTAB_add (STRTAB *tab, char *str, ULONG cnt)
{
	STRINFO	si;

	if (STRTAB_add (tab, str, cnt, &si) != SPWERR_OK) return (NULL);
	return (si.str);
}

void
STRTAB_del (STRTAB *tab, STRINFO *inf)
{
	STRTAB_del (tab, inf, 1);
}

void
STRTAB_del (STRTAB *tab, STRINFO *inf, ULONG cnt)
{
	SPWAW_ERROR	rc;
	STRIDX		*idx;

	if (!tab || !inf) return;

	rc = STRTAB_getidx (tab, inf);
	if (rc != SPWERR_OK) return;

	idx = &(tab->idx[inf->idx]);
	if (idx->ref <= cnt) {
		ADhash_del (tab->hash, idx->ptr);
		STRBUF_del (tab->buf, idx->ptr);
		idx->ptr = NULL;
		idx->ref = 0;
		tab->cnt--;
	} else {
		// Prevent warning C4244
		idx->ref = idx->ref - cnt;
	}
}

void
STRTAB_del (STRTAB *tab, char *str)
{
	STRINFO	inf;

	inf.str = str;
	STRTAB_del (tab, &inf, 1);
}

void
STRTAB_del (STRTAB *tab, char *str, ULONG cnt)
{
	STRINFO	inf;

	inf.str = str;
	STRTAB_del (tab, &inf, cnt);
}

SPWAW_ERROR
STRTAB_getstr (STRTAB *tab, STRINFO *inf)
{
	CNULLARG (tab); CNULLARG (inf);

	inf->str = BADSTR;

	if ((inf->idx < tab->ilen) && (tab->idx[inf->idx].ref))
		inf->str = tab->idx[inf->idx].ptr;

	return (SPWERR_OK);
}

char *
STRTAB_getstr (STRTAB *tab, ULONG idx)
{
	SPWAW_ERROR	rc;
	STRINFO		si;

	si.idx = idx;
	rc = STRTAB_getstr (tab, &si);
	if (rc != SPWERR_OK)
		return (BADSTR);
	else
		return (si.str);
}

SPWAW_ERROR
STRTAB_getidx (STRTAB *tab, STRINFO *inf)
{
	ULONG	idx = 0;

	CNULLARG (tab); CNULLARG (inf);

	inf->idx = BADSTRIDX;

	if (inf->str) idx = (ULONG)ADhash_get (tab->hash, inf->str);
	if (idx-- && tab->idx[idx].ref) inf->idx = idx;

	return (SPWERR_OK);
}

ULONG
STRTAB_getidx (STRTAB *tab, char *str)
{
	SPWAW_ERROR	rc;
	STRINFO		si;

	si.str = str;
	rc = STRTAB_getidx (tab, &si);
	if (rc != SPWERR_OK)
		return (BADSTRIDX);
	else
		return (si.idx);
}

SPWAW_ERROR
STRTAB_merge (STRTAB *tab, STRTAB *ptr)
{
	SPWAW_ERROR	rc;
	ULONG		i, cnt;
	STRINFO		si;

	CNULLARG (tab);

	if (!ptr) return (SPWERR_OK);

	cnt = 0;
	for (i=0; i<ptr->icnt; i++) {
		if (!ptr->idx[i].ref) continue;
		rc = STRTAB_add (tab, ptr->idx[i].ptr, ptr->idx[i].ref, &si);
		ERRORGOTO ("STRTAB_add()", handle_error);
		cnt++;
	}

	return (SPWERR_OK);

handle_error:
	for (i=0; i<cnt; i++) {
		if (!ptr->idx[i].ref) continue;
		si.str = ptr->idx[i].ptr; STRTAB_del (tab, &si, ptr->idx[i].ref);
	}
	return (rc);
}

SPWAW_ERROR
STRTAB_remove (STRTAB *tab, STRTAB *ptr)
{
	ULONG		i;
	STRINFO		si;

	CNULLARG (tab);
	if (!ptr) return (SPWERR_OK);

	for (i=0; i<ptr->icnt; i++) {
		if (!ptr->idx[i].ref) continue;
		si.str = ptr->idx[i].ptr;
		STRTAB_del (tab, &si, ptr->idx[i].ref);
	}

	return (SPWERR_OK);
}

#define	ROF(b)	do { if (!b) { rc = SPWERR_FRFAILED; ERRORGOTO ("TC_v2t()", handle_error); } } while (0)

SPWAW_ERROR
STRTAB_export (int src, FILE *dst)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		pos;
	STRTAB_HDR	hdr;
	ULONG		i;
	USHORT		ref;
	bool		b;

	CNULLARG (dst);

	pos = bseekget (src);

	if (!bread (src, (char *)&hdr, sizeof (hdr), false)) rc = SPWERR_FRFAILED;
	ERRORGOTO ("bread(header)", handle_error);

	b = TC_v2t (dst, "stab.header.magic",	TC_DATA_ASCIIZ, &(hdr.magic), sizeof (hdr.magic));	ROF(b);
	b = TC_v2t (dst, "stab.header.version",	TC_DATA_ULONG, &(hdr.version));	ROF(b);
	b = TC_v2t (dst, "stab.header.cnt",	TC_DATA_ULONG, &(hdr.cnt));	ROF(b);
	b = TC_v2t (dst, "//stab.header.buf",	TC_DATA_ULONG, &(hdr.buf));	ROF(b);

	for (i=0; i<hdr.cnt; i++) {
		if (!bread (src, (char *)&ref, sizeof (ref), false)) rc = SPWERR_FRFAILED;
		ERRORGOTO ("bread(refcnt)", handle_error);

		b = TC_v2t (dst, "stab.refcnt", TC_DATA_IDX_USHORT, &ref, 0, i); ROF(b);
	}

	bseekset (src, pos + hdr.buf);

	rc = STRBUF_export (src, dst);
	ERRORGOTO ("STRBUF_fdload()", handle_error);

	return (SPWERR_OK);

handle_error:
	return (rc);
}

SPWAW_ERROR
STRTAB_import (FILE *src, int /*dst*/)
{
	CNULLARG (src);

	return (SPWERR_OK);
}

void
debug_STRTAB_report (FILE *rf, STRTAB *tab, char *msg)
{
	ADhash_stats_t	s;
	ULONG		i;

	if (tab) {
		fprintf (rf, "--- STRTAB (%s) = 0x%8.8x ---\n", msg?msg:"", tab);
	} else {
		fprintf (rf, "--- STRTAB (%s) NULL pointer! ---\n", msg?msg:"");
		fprintf (rf, "\n\n");
		return;
	}

	fprintf (rf, "cnt  = %lu\n", tab->cnt);
	fprintf (rf, "\n");

	STRBUF_report (rf, tab->buf);

	fprintf (rf, "idx  = 0x%8.8x, len %lu, cnt %lu\n", tab->idx, tab->ilen, tab->icnt);
	fprintf (rf, "hash = 0x%8.8x, size %lu\n", tab->hash, ADhash_size (tab->hash));
	ADhash_stats (tab->hash, &s);
	fprintf (rf, "     : buckets total=%lu, used=%lu (%6.2f %%)\n", s.total, s.used, (double)s.used/(double)s.total*100.0);
	fprintf (rf, "     : min bucket usage=%lu, count=%lu\n", s.min_fill, s.min_count);
	fprintf (rf, "     : max bucket usage=%lu, count=%lu\n", s.max_fill, s.max_count);
	fprintf (rf, "     : avg bucket usage=%.2f\n", s.avg_fill);
	fprintf (rf, "\n");

	for (i=0; i<tab->icnt; i++) {
		if (!tab->idx[i].ptr) continue;

		fprintf (rf, "idx[%4lu] 0x%8.8x \"%s\" (ref %lu)\n", i, tab->idx[i].ptr, tab->idx[i].ptr, tab->idx[i].ref);
	}
	fprintf (rf, "\n");

	debug_ADhash_report (rf, tab->hash, msg);

	fprintf (rf, "------------------------------\n\n");
	fflush (rf);
}
