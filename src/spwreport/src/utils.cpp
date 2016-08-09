/** \file
 * The Steel Panthers World at War report tool.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL V2
 */

#include "utils.h"

static void
byte_list (BYTE *src, DWORD slen, char *dst, DWORD dlen)
{
	DWORD	todo, left;
	BYTE	*s;
	char	*d;
	int	done;

	if (!src || !dst || !dlen) return;
	memset (dst, 0, dlen);

	if (!slen) return;

	s = src; todo = slen;
	d = dst; left = dlen - 1;
	while (todo && (left >= 5)) {
		done = snprintf (d, 6, "0x%2.2x ", *s);
		if (done < 0) break;
		s++; todo--;
		d += done; left -= done;
	}
}

void
mkstr_bytedata (MKSTR_DST *dst, MKSTR_SRC *src, MKSTR_OPT *opt)
{
	if (!dst || !src || !opt) return;

	switch (opt->type) {
		case MKSTR_LIST:
			byte_list (src->ptr, src->len, dst->ptr, dst->len);
			break;
		case MKSTR_ARRAY:
			break;
		default:
			break;
	}
}

void
UD_report (SPWAW_UD *ud, char *title, FILE *file)
{
	char		head[64], buf[256];
	int		len;
	MKSTR_SRC	mksrc;
	MKSTR_DST	mkdst;
	MKSTR_OPT	mkopt;

	if (ud->cnt == 0) return;

	mkdst.ptr = buf;
	mkdst.len = sizeof (buf) - 1;

	memset (head, 0, sizeof (head));
	if (ud->cnt == 1) {
		if (title)	snprintf (head, sizeof (head) - 1, "Unknown %s data: ", title);
		else		snprintf (head, sizeof (head) - 1, "Unknown data: ");

		mksrc.ptr  = ud->head->data;
		mksrc.len  = ud->head->size;
		mksrc.addr = ud->head->addr;
		mkopt.type = MKSTR_LIST;
		mkstr_bytedata (&mkdst, &mksrc, &mkopt);

		fprintf (file, "    %s#%02.2u (%3.3u) 0x%8.8x: %s\n", head, 0, ud->head->size, ud->head->addr, buf);
	} else {
		SPWAW_UDEL	*u = ud->head;
		DWORD		i = 0;

		if (title)	len = snprintf (head, sizeof (head) - 1, "Unknown %s data report:\n", title);
		else		len = snprintf (head, sizeof (head) - 1, "Unknown data report:\n");
		if (len < 0) return;

		fprintf (file, head);
		memset (head, '-', len-1);
		fprintf (file, head);
		while ((i<ud->cnt) && u) {
			mksrc.ptr  = u->data;
			mksrc.len  = u->size;
			mksrc.addr = u->addr;
			mkopt.type = MKSTR_LIST;
			mkstr_bytedata (&mkdst, &mksrc, &mkopt);

			fprintf (file, "\t#%02.2u (%3.3u) 0x%8.8x: %s\n", i, u->size, u->addr, buf);
			i++; u = u->next;
		}
	}
	fprintf (file, "\n");
}

void
UD_block_report (SPWAW_UD *ud, int width, FILE *file, char *prefix)
{
	SPWAW_UDEL	*p;
	DWORD		size;
	char		pf[256];
	char		head[512], *sp, *hp;
	DWORD		addr, endaddr;
	int		i, uidx;

	/* Early bailout if nothing to do */
	if (ud->cnt == 0) return;

	/* Calculate block range */
	p = ud->head; size = 0;
	while (p != NULL) {
		size += p->size;
		p = p->next;
	}

	memset (pf, 0 ,sizeof (pf));
	if (prefix) snprintf (pf, sizeof (pf) - 1, "%s", prefix);

	if (sizeof (head) <= (12 + (3*width) + 1)) {
		error ("not enough room to create UD block report header");
		return;
	}

	memset (head, 0, sizeof (head));
	snprintf (head, sizeof (head) - 1, "%s%12.12s", pf, "");
	sp = head + strlen(pf) + 12;
	hp = sp;
	for (i=0; i<width; i++) {
		snprintf (hp, 4, "%2.2X ", i);
		hp += 3;
	}
	*hp = '\n';
	fprintf (file, head);

	hp = sp;
	memset (hp, '_', strlen (head) - (sp - head) - 1);
	fprintf (file, head);

	endaddr = ud->size - 1;	addr = 0;
	p = ud->head; uidx = -1;
	while (addr <= endaddr) {
		fprintf (file, "%s  %8.8X: ", pf, addr);
		for (i=0; i<width; i++) {
			if ((addr+i) > endaddr) break;
			if (!p) {
				fprintf (file, ".. ");
				continue;
			}
			if (uidx > 0) {
				if ((DWORD)uidx == p->size) {
					uidx = -1;
					p = p->next;
					if (p && ((addr+i) == p->addr)) {
						fprintf (file, "%2.2X ", p->data[0]);
						uidx = 1;
					} else {
						fprintf (file, ".. ");
					}
				} else {
					fprintf (file, "%2.2X ", p->data[uidx]);
					uidx++;
				}
			} else {
				if ((addr+i) < p->addr) {
					fprintf (file, ".. ");
				} else {
					fprintf (file, "%2.2X ", p->data[0]);
					uidx = 1;
				}
			}
		}
		fprintf (file, "\n");
		addr += width;
	}

	fprintf (file, "\n");
}

void
UD_block_report (SPWAW_UD *ud, char *title, int width, FILE *file, char *prefix)
{
	SPWAW_UDEL	*p;
	DWORD		size;
	char		pf[256];
	char		head[512];

	/* Early bailout if nothing to do */
	if (ud->cnt == 0) return;

	/* Calculate block range */
	p = ud->head; size = 0;
	while (p != NULL) {
		size += p->size;
		p = p->next;
	}

	memset (pf, 0 ,sizeof (pf));
	if (prefix) snprintf (pf, sizeof (pf) - 1, "%s", prefix);

	memset (head, 0 ,sizeof (head));
	if (title)	snprintf (head, sizeof (head) - 1, "%sUnknown %s data report: (%lu out of %lu bytes)\n", pf, title, size, ud->size);
	else		snprintf (head, sizeof (head) - 1, "%sUnknown data report: (%lu out of %lu bytes)\n", pf, size, ud->size);

	smart_title (file, '-', head);

	UD_block_report (ud,width, file, prefix);
}

void
UD_block_report (SPWAW_UD *ud, int width, FILE *file)
{
	UD_block_report (ud, width, file, NULL);
}

void
UD_block_report (SPWAW_UD *ud, char *title, int width, FILE *file)
{
	UD_block_report (ud, title, width, file, NULL);
}

void
error (char *fmt, ...)
{
	va_list	AP;
	char	buf[1024];

	memset (buf, 0, sizeof (buf));
	va_start (AP, fmt);
	vsnprintf (buf, sizeof (buf) - 1, fmt, AP);
	va_end (AP);
	fprintf (stderr, "*** Error: %s!\n", buf);

	exit (2);
}

void
smart_title (FILE *rf, char ulc, char *fmt, ...)
{
	va_list	AP;
	int	len;
	char	buf[256];

	va_start (AP, fmt);
	memset (buf, 0, sizeof (buf));
	len = vsnprintf (buf, sizeof (buf) - 1, fmt, AP);
	va_end (AP);
	if (len < 0) return;

	fprintf (rf, buf);
	memset (buf, ulc, len-1);
	fprintf (rf, buf);
	fprintf (rf, "\n");
}

void
report_mmas (FILE *rf, char *msg, SPWAW_IMMAS *ptr, SPWAW_SNAP_OOB_FORCE *fp, bool unit)
{
	char			smax[256];
	char			smin[256];
	SPWAW_SNAP_OOB_PTR	p;

	if (!ptr || !fp) return;

	memset (smax, 0, sizeof (smax));
	memset (smin, 0, sizeof (smin));
	if (unit) {
		p.up = &(fp->units.list[ptr->max.i]);
		snprintf (smax, sizeof (smax) - 1, "%s: %s (%s %s)", p.up->strings.uid, p.up->data.type, p.up->strings.rank, p.up->data.name);
		p.up = &(fp->units.list[ptr->min.i]);
		snprintf (smin, sizeof (smin) - 1, "%s: %s (%s %s)", p.up->strings.uid, p.up->data.type, p.up->strings.rank, p.up->data.name);
	} else {
		p.fp = &(fp->formations.list[ptr->max.i]);
		snprintf (smax, sizeof (smax) - 1, "%s: %s %s", p.fp->strings.name, p.fp->strings.fstatus, p.fp->strings.type);
		p.fp = &(fp->formations.list[ptr->min.i]);
		snprintf (smin, sizeof (smin) - 1, "%s: %s %s", p.fp->strings.name, p.fp->strings.fstatus, p.fp->strings.type);
	}

	fprintf (rf, "%s: max: %u (%3u: %s)\n", msg, ptr->max.v, ptr->max.i, smax);
	fprintf (rf, "%s: min: %u (%3u: %s)\n", msg, ptr->min.v, ptr->min.i, smin);
	fprintf (rf, "%s: avg: %.2f\n", msg, ptr->avg);
	fprintf (rf, "%s: spr: %u\n", msg, ptr->spr);
	fprintf (rf, "%s: tot: %u\n", msg, ptr->tot);
	fprintf (rf, "%s: cnt: %u\n", msg, ptr->cnt);
}

void
report_mmas (FILE *rf, char *msg, SPWAW_FMMAS *ptr, SPWAW_SNAP_OOB_FORCE *fp, bool unit)
{
	char			smax[256];
	char			smin[256];
	SPWAW_SNAP_OOB_PTR	p;

	if (!ptr || !fp) return;

	memset (smax, 0, sizeof (smax));
	memset (smin, 0, sizeof (smin));
	if (unit) {
		p.up = &(fp->units.list[ptr->max.i]);
		snprintf (smax, sizeof (smax) - 1, "%s: %s (%s %s)", p.up->strings.uid, p.up->data.type, p.up->strings.rank, p.up->data.name);
		p.up = &(fp->units.list[ptr->min.i]);
		snprintf (smin, sizeof (smin) - 1, "%s: %s (%s %s)", p.up->strings.uid, p.up->data.type, p.up->strings.rank, p.up->data.name);
	} else {
		p.fp = &(fp->formations.list[ptr->max.i]);
		snprintf (smax, sizeof (smax) - 1, "%s: %s %s", p.fp->strings.name, p.fp->strings.fstatus, p.fp->strings.type);
		p.fp = &(fp->formations.list[ptr->min.i]);
		snprintf (smin, sizeof (smin) - 1, "%s: %s %s", p.fp->strings.name, p.fp->strings.fstatus, p.fp->strings.type);
	}

	fprintf (rf, "%s: max: %.2f (%3u: %s)\n", msg, ptr->max.v, ptr->max.i, smax);
	fprintf (rf, "%s: min: %.2f (%3u: %s)\n", msg, ptr->min.v, ptr->min.i, smin);
	fprintf (rf, "%s: avg: %.2f\n", msg, ptr->avg);
	fprintf (rf, "%s: spr: %.2f\n", msg, ptr->spr);
	fprintf (rf, "%s: tot: %.2f\n", msg, ptr->tot);
	fprintf (rf, "%s: cnt: %u\n", msg, ptr->cnt);
}
