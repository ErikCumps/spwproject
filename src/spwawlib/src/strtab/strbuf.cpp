#include "stdafx.h"
#include "strbuf.h"
#include "fileio/fileio.h"
#include "utils/textconv.h"
#include "common/internal.h"
#include "utils/compression.h"

#define	STRBUF_MAGIC	"SPWAWLIB_STRBUF"
#define	STRBUF_MAGICLEN	16
#define	STRBUF_VERSION	2

#pragma pack(push, r1, 1)

typedef struct s_STRBUF_HDR {
	char	magic[STRBUF_MAGICLEN];	/* STRBUF magic string				*/
	ULONG	version;		/* STRBUF file format version			*/
	ULONG	size;			/* data size					*/
	ULONG	comp;			/* compressed data size (0 if no compression)	*/
} STRBUF_HDR;

#pragma pack(pop, r1)

#define	BLOCKSIZE	4096

static char *
block_alloc (STRBUF *buf, ULONG len)
{
	STRBUF_BLOCK	*cb, *nb;
	char		*p;
	ULONG		bsize, tsize;

	if (!len) return (NULL);

	cb = buf->last;
	if (cb && (cb->left >= len)) {
		p = cb->last;
		cb->used += len; cb->left -= len; cb->last += len;
	} else {
		bsize = (len / BLOCKSIZE) * BLOCKSIZE; if (len > bsize) bsize += BLOCKSIZE;
		tsize = bsize + sizeof (STRBUF_BLOCK);
		nb = safe_smalloc (STRBUF_BLOCK, tsize); COOMRET (nb, "STRBUF_BLOCK", NULL);
		memset (nb, 0, tsize);
		
		nb->size = bsize;
		nb->used = len;
		nb->left = bsize - len;
		nb->data = (char *)nb + sizeof (STRBUF_BLOCK);
		nb->last = nb->data + len;
		p = nb->data;

		if (cb) {
			cb->next = nb; cb->left = 0;
		} else {
			buf->list = nb;
		}
		buf->last = nb;
	}

	return (p);
}

static SPWAW_ERROR
add_hole (STRBUF_BLOCK *block, char *ptr)
{
	STRBUF_HOLE	*p, *pb, *pa;

	p = safe_malloc (STRBUF_HOLE); COOM (p, "STRBUF_HOLE");

	p->ptr = ptr;
	p->len = strlen (ptr) + 1;
	
	pb = pa = block->hole;
	if (block->hole) {
		if (p->ptr > pb->ptr) {
			pa = pb->next;
			while (pa && (p->ptr > pa->ptr)) { pb = pa; pa = pb->next; }
		} else {
			pb = NULL;
		}
		if (pa && ((p->ptr+p->len) == pa->ptr)) {
			p->len += pa->len;
			p->next = pa->next;
			free (pa);
		} else {
			p->next = pa;
		}
		if (pb) {
			if ((pb->ptr+pb->len) == p->ptr) {
				pb->len += p->len;
				pb->next = p->next;
				free (p); p = pb;
			} else {
				pb->next = p;
			}
		} else {
			block->hole = p;
		}
	} else {
		block->hole = p;
	}

	if (p->len >= (2 + sizeof (p->len))) {
		memset (p->ptr, 0xFF, p->len);
		memcpy (p->ptr+2, &(p->len), sizeof (p->len));
	} else {
		memset (p->ptr, 0, p->len);
		p->ptr[0] = (unsigned char)0xFF;
	}

	return (SPWERR_OK);
}

SPWAW_ERROR
STRBUF_new (STRBUF **buf)
{
	STRBUF	*p;

	CNULLARG (buf);
	*buf = NULL;

	p = safe_malloc (STRBUF); COOM (p, "STRBUF");

	*buf = p;

	return (SPWERR_OK);
}

void
STRBUF_free (STRBUF **buf)
{
	STRBUF		*p;
	STRBUF_BLOCK	*bp;
	STRBUF_HOLE	*hp;

	if (!buf || !*buf) return;
	p = *buf; *buf = NULL;

	while ((bp = p->list) != NULL) {
		p->list = bp->next;
		while ((hp = bp->hole) != NULL) { bp->hole = hp->next; free (hp); }
		free (bp);
	}
	free (p);
}

SPWAW_ERROR
STRBUF_init (STRBUF *buf)
{
	CNULLARG (buf);
	STRBUF_clear (buf);

	return (SPWERR_OK);
}

void
STRBUF_clear (STRBUF *buf)
{
	STRBUF_BLOCK	*bp;
	STRBUF_HOLE	*hp;

	if (!buf) return;

	while ((bp = buf->list) != NULL) {
		buf->list = bp->next;
		while ((hp = bp->hole) != NULL) { bp->hole = hp->next; free (hp); }
		free (bp);
	}
	buf->list = buf->last = NULL;
}

SPWAW_ERROR
STRBUF_load (STRBUF *buf, char *file)
{
	int		fd;
	SPWAW_ERROR	rc;

	CNULLARG (buf); CNULLARG (file);

	fd = open (file, _O_RDONLY|_O_BINARY);
	if (fd < 0) RWE (SPWERR_FOFAILED, "failed to open STRBUF file for reading");

	rc = STRBUF_fdload (buf, fd); close (fd);
	return (rc);
}

SPWAW_ERROR
STRBUF_fdload (STRBUF *buf, int fd)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	STRBUF_HDR	hdr;
	char		*data = NULL;
	long		size;
	SBR		*sbr = NULL;
	char		*p;
	CBIO		cbio;

	CNULLARG (buf);
	STRBUF_clear (buf);

	if (!bread (fd, (char *)&hdr, sizeof (hdr), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(STRBUF hdr) failed", handle_error);

	if ((memcmp (hdr.magic, STRBUF_MAGIC, STRBUF_MAGICLEN) != 0) || (hdr.version != STRBUF_VERSION))
		FAILGOTO (SPWERR_BADSBUFDATA, "STRBUF header check failed", handle_error);

	if (!hdr.size) return (SPWERR_OK);

	size = hdr.size;
	data = safe_smalloc (char, size); COOMGOTO (data, "STRBUF data buffer", handle_error);

	cbio.data = data; cbio.size = size; cbio.comp = &(hdr.comp);
	if (!cbread (fd, cbio, "STRBUF data")) FAILGOTO (SPWERR_FRFAILED, "cbread(STRBUF data) failed", handle_error);

	sbr = sbread_init (data, size);
	if (!sbr) FAILGOTO (SPWERR_FRFAILED, "sbread_init() failed", handle_error);

	p = block_alloc (buf, size); COOMGOTO (p, "STRBUF block", handle_error);

	if (sbread (sbr, p, size) != size)
		FAILGOTO (SPWERR_FRFAILED, "sbread(STRBUF data) failed", handle_error);

	sbread_stop (sbr); sbr = NULL;
	safe_free (data);

	return (SPWERR_OK);

handle_error:
	// FIXME: consider undoing block_alloc() here?
	if (sbr) sbread_stop (sbr);
	if (data) safe_free (data);
	return (rc);
}

SPWAW_ERROR
STRBUF_save (STRBUF *buf, char *file, bool compress)
{
	int		fd;
	SPWAW_ERROR	rc;

	CNULLARG (buf); CNULLARG (file);

	fd = open (file, _O_CREAT|_O_TRUNC|_O_WRONLY|_O_BINARY, 0666);
	if (fd < 0) RWE (SPWERR_FOFAILED, "failed to open STRBUF file for writing");

	rc = STRBUF_fdsave (buf, fd, compress); close (fd);
	return (rc);
}

SPWAW_ERROR
STRBUF_fdsave (STRBUF *buf, int fd, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	STRBUF_HDR	hdr;
	long		p0, p1;
	SBW		*sbw = NULL;
	STRBUF_BLOCK	*bp;
	STRBUF_HOLE	*hp;
	char		*p = NULL;
	long		todo;
	CBIO		cbio;

	CNULLARG (buf);

	memset (&hdr, 0, sizeof (hdr));
	memcpy (hdr.magic, STRBUF_MAGIC, strlen (STRBUF_MAGIC));
	hdr.version = STRBUF_VERSION;

	hdr.size = 0;
	hdr.comp = 0;

	p0 = bseekget (fd);
	bseekmove (fd, sizeof (hdr));

	sbw = sbwrite_init (NULL, 0);
	if (!sbw) FAILGOTO (SPWERR_FWFAILED, "sbwrite_init() failed", handle_error);

	bp = buf->list;
	while (bp) {
		p = bp->data; hp = bp->hole;
		while (hp) {
			todo = hp->ptr - p;
			if (sbwrite (sbw, p, todo) != todo)
				FAILGOTO (SPWERR_FWFAILED, "sbwrite(STRBUF data) failed", handle_error);
			hdr.size += todo; p += (todo + hp->len);
			hp = hp->next;
		}
		todo = bp->last - p;
		if (sbwrite (sbw, p, todo) != todo)
			FAILGOTO (SPWERR_FWFAILED, "sbwrite(STRBUF data) failed", handle_error);
		hdr.size += todo;
		bp = bp->next;
	}

	sbwrite_stop (sbw, &p, &todo); sbw = NULL;

	cbio.data = p; cbio.size = todo; cbio.comp = &(hdr.comp);
	if (!cbwrite (fd, cbio, "STRBUF data", compress)) FAILGOTO (SPWERR_FWFAILED, "cbwrite(STRBUF data) failed", handle_error);
	safe_free (p);

	p1 = bseekget (fd); bseekset (fd, p0);
	if (!bwrite (fd, (char *)&hdr, sizeof (hdr))) FAILGOTO (SPWERR_FWFAILED, "bwrite(STRBUF hdr) failed", handle_error);
	bseekset (fd, p1);

	return (SPWERR_OK);

handle_error:
	if (p) safe_free (p);
	if (sbw) sbwrite_stop (sbw);
	bseekset (fd, p0);
	return (rc);
}

char *
STRBUF_add (STRBUF *buf, char *str)
{
	ULONG	len;
	char	*p;

	if (!buf || !str) return (NULL);

	len = strlen (str) + 1;
	if ((p = block_alloc (buf, len)) == NULL) return (NULL);

	memcpy (p, str, len);

	return (p);
}

static inline STRBUF_BLOCK *
find_block (STRBUF *buf, char *str)
{
	STRBUF_BLOCK	*bp;

	bp = buf->list;
	while (bp) {
		if ((bp->data <= str) && (str < (bp->data + bp->used))) break;
		bp = bp->next;
	}
	return (bp);
}

void
STRBUF_del (STRBUF *buf, char *str)
{
	STRBUF_BLOCK	*bp;

	if (!buf || !str) return;

	bp = find_block (buf, str);
	if (!bp) return;

	add_hole (bp, str);
}

static bool
skip_core (STRBUF_BLOCK **bp, char **ptr)
{
	char	*p;
	ULONG	len;

	p = *ptr;

	if (*p != (unsigned char)0xFF) return (false);
	
	p++;
	if (*p == (unsigned char)0xFF) {
		len = (ULONG)++p; p += len;
	} else {
		while (!*++p && (p < (*bp)->last));
	}

	if (p >= (*bp)->last) {
		*bp = (*bp)->next;
		*ptr = (*bp)?(*bp)->data:NULL;
	} else {
		*ptr = p;
	}

	return (*ptr?true:false);
}

char *
STRBUF_next (STRBUF *buf, char *str)
{
	STRBUF_BLOCK	*bp;
	char		*p;

	if (!buf) return (NULL);

	if (!str) {
		if (!buf->list || !buf->list->data) return (NULL);
		bp = buf->list;
		p = bp->data;
	} else {
		if ((bp = find_block (buf, str)) == NULL) return (NULL);
		p = strchr (str, '\0');	if (!p) return (NULL);
		p++; if (p >= bp->last) {
			bp = bp->next; if (!bp) return (NULL);
			p = bp->data;
		}
	}
	while (skip_core (&bp, &p));

	return (p);
}

#define	ROF(b)	do { if (!b) { rc = SPWERR_FRFAILED; ERRORGOTO ("TC_v2t()", handle_error); } } while (0)

SPWAW_ERROR
STRBUF_export (int src, FILE *dst)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	STRBUF_HDR	hdr;
	bool		b;
	char		*p = NULL;

	CNULLARG (dst);

	if (!bread (src, (char *)&hdr, sizeof (hdr), false)) rc = SPWERR_FRFAILED;
	ROE ("bread(header)");

	b = TC_v2t (dst, "strbuf.header.magic",		TC_DATA_ASCIIZ, &(hdr.magic), sizeof (hdr.magic));	ROF(b);
	b = TC_v2t (dst, "strbuf.header.version",	TC_DATA_ULONG, &(hdr.version));	ROF(b);
	b = TC_v2t (dst, "strbuf.header.size",		TC_DATA_ULONG, &(hdr.size));	ROF(b);

	if (hdr.size) {
		if ((p = (char *)malloc (hdr.size)) == NULL) rc = SPWERR_FRFAILED;
		ERRORGOTO ("malloc(data)", handle_error);
			
		if (!bread (src, p, hdr.size, false)) rc = SPWERR_FRFAILED;
		ERRORGOTO ("bread(data)", handle_error);

		b = TC_v2t (dst, "strbuf.data",	TC_DATA_STRBUF, p, hdr.size);	ROF(b);
	}

handle_error:
	if (p) free (p);
	return (rc);
}

SPWAW_ERROR
STRBUF_import (FILE *src, int /*dst*/)
{
	CNULLARG (src);

	return (SPWERR_OK);
}

void
STRBUF_report (FILE *rf, STRBUF *buf)
{
	STRBUF_BLOCK	*bp;
	STRBUF_HOLE	*hp;

	if (buf) {
		fprintf (rf, "STRBUF = 0x%8.8x\n", buf);
	} else {
		fprintf (rf, "STRBUF NULL pointer!\n");
		fprintf (rf, "\n\n");
		return;
	}

	fprintf (rf, "  list = 0x%8.8x\n", buf->list);
	fprintf (rf, "  last = 0x%8.8x\n", buf->last);
	bp = buf->list;
	while (bp) {
		fprintf (rf, "    : block 0x%8.8x\n", bp);
		fprintf (rf, "    :    next=0x%8.8x\n", bp->next);
		fprintf (rf, "    :    size=%lu\n", bp->size);
		fprintf (rf, "    :    used=%lu\n", bp->used);
		fprintf (rf, "    :    left=%lu\n", bp->left);
		fprintf (rf, "    :    data=0x%8.8x\n", bp->data);
		fprintf (rf, "    :    last=0x%8.8x\n", bp->last);
		fprintf (rf, "    :    hole=0x%8.8x\n", bp->hole);
		if ((hp = bp->hole) != NULL) {
			while (hp) {
				fprintf (rf, "    :    : next=0x%8.8x\n", hp->next);
				fprintf (rf, "    :    :    ptr=0x%8.8x\n", hp->ptr);
				fprintf (rf, "    :    :    len=%lu\n", hp->len);

				hp = hp->next;
			}
		}
		bp = bp->next;
	}
	fprintf (rf, "\n");
	fflush (rf);
}

