/** \file
 * Resource Archive - buildres tool - config file parsing.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include <resarchv.h>
#include <system/independence.h>

#include "idhash.h"
#include "idnamehash.h"
#include "autoid.h"
#include "parse_cfg.h"
#include "util.h"

static RESOURCE *find_in_list (RESLIST *list, unsigned long id);

/* fix backslashes in filename */
static void
fix_name (char **name)
{
	char	*p;
	char	*new, *q;

	if (name == NULL) abort();

	/* look for backslashes */
	if (strchr (*name, '\\') == NULL) return;

	SIZEMALLOC (new, char, strlen (*name));
	p = *name; q = new;
	while (*p != '\0') {
		if (*p == '\\' && *(p+1) == '\\') {
			p++; *p = '/';
		}
		*q++ = *p++;
	}
	free (*name); *name = new;
}

#define	MAXSPLITCNT	8

typedef struct {
	unsigned long	lc;
	char		*line;
	int		cnt;
	char		*lst[MAXSPLITCNT];
} SPLIT;

static int
split_line (char *line, unsigned long lc, char *p, SPLIT *split)
{
	int	idx = 0;

	if (!p || !split) return (0);
	memset (split, 0, sizeof (*split));

	split->line = line;
	split->lc = lc;

	while (*p && (idx < MAXSPLITCNT)) {
		split->lst[idx] = p; idx++;
		while (*p && !isspace (*p)) p++; if (!*p) break; *p++ = '\0';
		while (*p && isspace (*p)) p++;
	}

	split->cnt = idx;
	return (split->cnt);
}

static int
prep_line (char *line, char **ptr)
{
	char	*p = line;
	char	*q;

	if ((line == NULL) || (ptr == NULL)) abort();
	*ptr = NULL;

	/* Skip leading whitespace */
	while (isspace (*p)) p++;

	/* Skip comments */
	if (*p == '#') return (0);

	/* make copy */
	SAFE_STRDUP (p, ptr);

	/* Remove trailing newline */
	q = strchr (*ptr, '\n');
	if (q != NULL) *q = '\0';

	return (1);
}

typedef struct {
	char		*name;
	arc_type_t	type;
} TYPEMAP;

static TYPEMAP map[] = {
	{ "IMG",	arc_IMG		},
	{ "IMGCOL",	arc_IMGCOL	},
	{ "RGB",	arc_RGB		},
	{ "RGBA",	arc_RGBA	},
	{ "XPM",	arc_XPM		},
	{ NULL,		arc_BIN		}
};

static int
parse_type (char *str, arc_type_t *rtype)
{
	int	i;

	if ((str == NULL) || (rtype == NULL)) abort();

	i = 0;
	while (map[i].name) {
		if (strcmp (map[i].name, str) == 0) break;
		i++;
	}
	*rtype = map[i].type;

	return (1);
}

static int
parse_id (char *str, unsigned long *rid, char **ridname)
{
	char		*p;
	unsigned long	id;
	char		*idname = NULL;

	if ((str == NULL) || (rid == NULL) || (ridname == NULL)) abort();
	*rid = 0; *ridname = NULL;

	/* Determine line type */
	if (isdigit (*str)) {
		errno = 0; id = strtoul (str, &p, 0);
		if (errno || (p == NULL) || !*p) return (0);

		if (ih_find (id) != NULL) {
			printf ("*** Error: duplicate ID (%lu) detected!", id);
			return (0);
		}
	} else {
		/* Extract ID name */
		SAFE_STRDUP (str, &idname);

		if (nh_find (idname) != NULL) {
			printf ("*** Error: duplicate ID name \"%s\" detected!", idname);
			free (idname);
			return (0);
		}

		id = autoid (idname);
	}

	/* Register id and idname */
	ih_register (id);
	if (idname) nh_register (idname, id);

	*rid = id; *ridname = idname;
	return (1);
}

static int
parse_fname (char *str, RESDATA_FILE *dst)
{
	char	*p = str;
	char	*q;
	int	quoted = 0;

	if ((str == NULL) || (dst == NULL)) abort();
	dst->fname = dst->fpath = NULL;

	/* Look for opening quote */
	if (*p == '"') { quoted = 1; p++; if (p == NULL) return (0); }

	/* extract filename */
	q = p;
	if (quoted) {
		/* look for closing quote */
		while ((*q != '\0') && (*q != '"')) q++;
		if (*q != '"') return (0);
	} else {
		/* look for end of string */
		q = strchr (q, '\0');
		/* remove trailing whitespace */
		while ((q > p) && isspace(*(q-1))) q--;
		if (q == p) return (0);
	}
	*q = '\0';

	SAFE_STRDUP (p, &(dst->fname));
	return (1);
}

static int
parse_dimensions (char *str, unsigned long *rw, unsigned long *rh)
{
	char		*p, *q;
	unsigned long	w, h;

	if ((str == NULL) || (rw == NULL) || (rh == NULL)) abort();
	*rw = *rh = 0;

	/* Discover first and second number */
	p = strchr (str, 'x'); if (!p) return (0);
	*p++ = '\0';

	errno = 0; w = strtoul (str, &q, 0);
	if (errno || (q == NULL) || *q) return (0);

	errno = 0; h = strtoul (p, &q, 0);
	if (errno || (q == NULL) || *q) return (0);

	*rw = w; *rh = h;
	return (1);
}

static int
parse_rgb (char *str, RESOURCE *dst, RESLIST *list)
{
	char		*p = str;
	char		*q, *e;
	unsigned long	id;
	RESOURCE	*l;

	if ((str == NULL) || (dst == NULL)) abort();
	memset (dst->data.rgb.rgb, 0xFF, sizeof (dst->data.rgb.rgb));

	if (isdigit (p[0])) {
		/* extract RGB triplet */
		q = p;
		while ((*q != '\0') && (*q != ',')) q++; if (*q != ',') return (0); *q++ = '\0';
		errno = 0; dst->data.rgb.rgb[0] = strtoul (p, &e, 0); if (errno || (e == NULL) || *e) return (0);

		p = q;
		while ((*q != '\0') && (*q != ',')) q++; if (*q != ',') return (0); *q++ = '\0';
		errno = 0; dst->data.rgb.rgb[1] = strtoul (p, &e, 0); if (errno || (e == NULL) || *e) return (0);

		p = q;
		while ((*q != '\0') && (*q != ',')) q++; if (*q != '\0') return (0);
		errno = 0; dst->data.rgb.rgb[2] = strtoul (p, &e, 0); if (errno || (e == NULL) || *e) return (0);
	} else {
		id = (unsigned long) nh_find (str);
		if (id == 0) {
			printf ("*** Error: invalid RGB link detected: \"%s\" does not exist!", str);
			return (0);
		}
		dst->link = find_in_list (list, id);
		while (dst->link->link != NULL) { l = dst->link->link; dst->link = l; }
	}

	return (1);
}

static int
parse_rgba (char *str, RESOURCE *dst, RESLIST *list)
{
	char		*p = str;
	char		*q, *e;
	unsigned long	id;
	RESOURCE	*l;

	if ((str == NULL) || (dst == NULL)) abort();
	memset (dst->data.rgba.rgba, 0xFF, sizeof (dst->data.rgba.rgba));

	if (isdigit (p[0])) {
		/* extract RGBA quadlet */
		q = p;
		while ((*q != '\0') && (*q != ',')) q++; if (*q != ',') return (0); *q++ = '\0';
		errno = 0; dst->data.rgba.rgba[0] = strtoul (p, &e, 0); if (errno || (e == NULL) || *e) return (0);

		p = q;
		while ((*q != '\0') && (*q != ',')) q++; if (*q != ',') return (0); *q++ = '\0';
		errno = 0; dst->data.rgba.rgba[1] = strtoul (p, &e, 0); if (errno || (e == NULL) || *e) return (0);

		p = q;
		while ((*q != '\0') && (*q != ',')) q++; if (*q != ',') return (0); *q++ = '\0';
		errno = 0; dst->data.rgba.rgba[2] = strtoul (p, &e, 0); if (errno || (e == NULL) || *e) return (0);

		p = q;
		while ((*q != '\0') && (*q != ',')) q++; if (*q != '\0') return (0);
		errno = 0; dst->data.rgba.rgba[3] = strtoul (p, &e, 0); if (errno || (e == NULL) || *e) return (0);
	} else {
		/* extract RGB link and alpha value */
		q = p;
		while ((*q != '\0') && (*q != ',')) q++; if (*q != ',') return (0); *q++ = '\0';
		id = (unsigned long) nh_find (p);
		if (id == 0) {
			printf ("*** Error: invalid RGBA link detected: \"%s\" does not exist!", p);
			return (0);
		}
		l = find_in_list (list, id);
		if (!l) {
			printf ("*** Error: invalid RGBA link detected: \"%s\" does not exist!", p);
			return (0);
		}
		while (l->link != NULL) { l = l->link; }

		if (*q == '\0') {
			dst->link = l;
		} else {
			if (l->type != arc_RGB) {
				printf ("*** Error: invalid RGBA RGB link detected: \"%s\" is not an RGB resource!", p);
				return (0);
			}
			dst->data.rgba.rgba[0] = l->data.rgb.rgb[0];
			dst->data.rgba.rgba[1] = l->data.rgb.rgb[1];
			dst->data.rgba.rgba[2] = l->data.rgb.rgb[2];

			p = q;
			while ((*q != '\0') && (*q != ',')) q++; if (*q != '\0') return (0);
			errno = 0; dst->data.rgba.rgba[3] = strtoul (p, &e, 0); if (errno || (e == NULL) || *e) return (0);
		}
	}

	return (1);
}

static int
parse_xpm_filename (char *str, RESDATA_XPM *dst)
{
	char	*p = str;
	char	*q;
	int	quoted = 0;

	if ((str == NULL) || (dst == NULL)) abort();
	dst->fname = NULL;

	/* Look for opening quote */
	if (*p == '"') { quoted = 1; p++; if (p == NULL) return (0); }

	/* extract filename */
	q = p;
	if (quoted) {
		/* look for closing quote */
		while ((*q != '\0') && (*q != '"')) q++;
		if (*q != '"') return (0);
	} else {
		/* look for end of string */
		q = strchr (q, '\0');
		/* remove trailing whitespace */
		while ((q > p) && isspace(*(q-1))) q--;
		if (q == p) return (0);
	}
	*q = '\0';

	SAFE_STRDUP (p, &(dst->fname));
	return (1);
}

typedef struct {
	unsigned long	len;
	unsigned long	cnt;
	unsigned long	*ssize;
	char		**sdata;
} STRINGLIST;

static void
STRINGLIST_init (STRINGLIST *list)
{
	if (!list) return;

	list->len = 8; list->cnt = 0;
	SIZEMALLOC (list->ssize, unsigned long, list->len * sizeof (unsigned long));
	SIZEMALLOC (list->sdata, char*, list->len * sizeof (char*));
}

static void
STRINGLIST_add (STRINGLIST *list, char *p)
{
	unsigned long	*size = NULL;
	char		**data = NULL;
	unsigned long	i, idx;

	if (!list || !p) return;

	if (list->cnt == list->len) {
		SIZEMALLOC (size, unsigned long, 2 * list->len * sizeof (unsigned long));
		SIZEMALLOC (data, char*, 2 * list->len * sizeof (char*));
		list->len *= 2;
		for (i=0; i<list->cnt; i++) {
			size[i] = list->ssize[i];
			data[i] = list->sdata[i];
		}
		free (list->ssize); list->ssize = size;
		free (list->sdata); list->sdata = data;
	}
	idx = list->cnt++;
	list->ssize[idx] = strlen (p);
	SAFE_STRDUP (p, &(list->sdata[idx]));
}

static void
STRINGLIST_cleanup (STRINGLIST *list)
{
	unsigned long	i;

	if (!list || !list->cnt) return;

	for (i=0; i<list->cnt; i++) free (list->sdata[i]);
	free (list->ssize); free (list->sdata);
	list->len = list->cnt = 0;
	list->ssize = NULL; list->sdata = NULL;
}

static int
parse_xpm_strings (RESDATA_XPM *dst)
{
	FILE		*f = NULL;
	char		buf[4096];
	int		s = 0;
	char		*p, *q;
	STRINGLIST	list;
	unsigned long	i;
	unsigned long	*xpm_cnt, xpm_cnt_size;
	unsigned long	*xpm_lst, xpm_lst_size;

	if ((dst == NULL) || (dst->fname == NULL)) abort();
	dst->cnt = 0; dst->data = NULL;

	f = fopen (dst->fname, "r");
	if (!f) return (0);

	STRINGLIST_init (&list);
	while ((s != 2) && (fgets (buf, sizeof (buf), f) != NULL)) {
		switch (s) {
			case 0:
				p = strchr (buf, '{');
				if (p) s = 1;
				break;
			case 1:
				p = strchr (buf, '"'); q = NULL;
				if (p) {
					p++, q = strchr (p, '"');
					if (q) {
						*q = 0;
						STRINGLIST_add (&list, p);
					}
				} else {
					if (strchr (buf, '}')) s = 2;
				}

				break;
			default:
				break;
		}
	}

	xpm_cnt_size = sizeof (unsigned long);
	xpm_lst_size = list.cnt * sizeof (unsigned long);
	dst->len = xpm_cnt_size + xpm_lst_size;
	for (i=0; i<list.cnt; i++) dst->len += list.ssize[i] + 1;

	SIZEMALLOC (dst->xpm, char, dst->len);

	xpm_cnt = (unsigned long *)(dst->xpm);
	xpm_lst = (unsigned long *)(dst->xpm + xpm_cnt_size);
	p = q = dst->xpm + xpm_cnt_size + xpm_lst_size;

	*xpm_cnt = list.cnt;
	for (i=0; i<*xpm_cnt; i++) {
		xpm_lst[i] = (unsigned long)(p - q);
		memcpy (p, list.sdata[i], list.ssize[i]);
		p += list.ssize[i] + 1;
	}

	STRINGLIST_cleanup (&list);
	fclose (f);
	return (1);
}

static int
parse_xpm (char *str, RESDATA_XPM *dst)
{
	if ((str == NULL) || (dst == NULL)) abort();
	dst->len = 0; dst->xpm = NULL;

	if (!parse_xpm_filename (str, dst)) return (0);
	if (!parse_xpm_strings (dst)) return (0);

	return (1);
}

static int
parse_row (char *str, unsigned long *rr)
{
	char		*p;
	unsigned long	r;

	if ((str == NULL) || (rr == NULL)) abort();
	*rr = 0;

	errno = 0; r = strtoul (str, &p, 10);
	if (errno || (p == NULL) || *p) return (0);

	*rr = r;
	return (1);
}

static int
parse_col (char *str, unsigned long *rc)
{
	char		*p;
	unsigned long	c;

	if ((str == NULL) || (rc == NULL)) abort();
	*rc = 0;

	errno = 0; c = strtoul (str, &p, 10);
	if (errno || (p == NULL) || *p) return (0);

	*rc = c;
	return (1);
}

static int
parse_sid (char *str, char *base, char **rsidname)
{
	unsigned long	len;
	char		*sidname = NULL;

	if ((str == NULL) || (base == NULL) || (rsidname == NULL)) abort();
	*rsidname = NULL;

	/* Create subID name */
	len = strlen (str) + 1 + strlen (base) + 1;
	SIZEMALLOC (sidname, char, len);
	snprintf (sidname, len - 1, "%s_%s", base, str);

	if (nh_find (sidname) != NULL) {
		printf ("*** Error: duplicate subID name \"%s\" detected!\n", sidname);
		free (sidname);
		return (0);
	}

	/* Register sidname */
	nh_register (sidname, 1);

	*rsidname = sidname;
	return (1);
}

static void
add_to_list (RESLIST *list, RESOURCE *item)
{
	if ((list == NULL) || (item == NULL)) abort();

	item->next = NULL;
	if (list->tail == NULL) {
		list->list = list->tail = item;
	} else {
		list->tail->next = item;
		list->tail = item;
	}
	item->parent = list;
	item->parent->count++;
}

static RESOURCE *
find_in_list (RESLIST *list, unsigned long id)
{
	RESOURCE	*p = NULL;

	if (list == NULL) abort();
	if (id == 0) return (NULL);

	p = list->list;
	while (p != NULL) {
		if (p->id == id) break;
		p = p->next;
	}
	return (p);
}


static void
handle_resource (SPLIT *split, RESLIST *list)
{
	arc_type_t	type = arc_BIN;
	unsigned long	id = 0;
	char		*idname = NULL;
	RESOURCE	*res = NULL;

	if ((split == NULL) || (list == NULL)) abort();

	if (split->cnt < 3) {
		printf ("*** Error: line %lu: invalid resource syntax\n", split->lc);
		goto handle_error;
	}

	if (!parse_type (split->lst[0], &type)) {
		printf ("*** Error: line %lu: failed to parse resource type\n", split->lc);
		goto handle_error;
	}
	if (!parse_id (split->lst[1], &id, &idname)) {
		printf ("*** Error: line %lu: failed to parse resource ID\n", split->lc);
		goto handle_error;
	}

	TYPEMALLOC (res, RESOURCE);
	res->type    = type;
	res->id      = id;
	res->name    = idname;

	switch (type) {
		case arc_IMG:
			if (!parse_fname (split->lst[2], &(res->data.file))) {
				printf ("*** Error: line %lu: failed to parse IMG file name and path\n", split->lc);
				goto handle_error;
			}
			break;
		case arc_IMGCOL:
			if (!parse_fname (split->lst[2], &(res->data.file))) {
				printf ("*** Error: line %lu: failed to parse IMGCOL file name and path\n", split->lc);
				goto handle_error;
			}
			if (split->cnt >= 4) {
				if (!parse_dimensions (split->lst[3], &(res->meta[0]), &(res->meta[1]))) {
					printf ("*** Error: line %lu: failed to parse IMGCOL resource dimensions\n", split->lc);
					goto handle_error;
				}
			}
			break;
		case arc_RGB:
			if (!parse_rgb (split->lst[2], res, list)) {
				printf ("*** Error: line %lu: failed to parse RGB triplet\n", split->lc);
				goto handle_error;
			}
			break;
		case arc_RGBA:
			if (!parse_rgba (split->lst[2], res, list)) {
				printf ("*** Error: line %lu: failed to parse RGBA quadlet\n", split->lc);
				goto handle_error;
			}
			break;
		case arc_XPM:
			if (!parse_xpm (split->lst[2], &(res->data.xpm))) {
				printf ("*** Error: line %lu: failed to parse XPM resource\n", split->lc);
				goto handle_error;
			}
			break;
		default:
			if (!parse_fname (split->lst[2], &(res->data.file))) {
				printf ("*** Error: line %lu: failed to parse resource file name and path\n", split->lc);
				goto handle_error;
			}
			break;
	}

	add_to_list (list, res);
	return;

handle_error:
	if (res) free (res);
	return;
}

static void
add_to_sublist (RESOURCE *base, SUBRESOURCE *item)
{
	if ((base == NULL) || (item == NULL)) abort();

	item->next = NULL;
	if (base->sub_tail == NULL) {
		base->sub_list = base->sub_tail = item;
	} else {
		base->sub_tail->next = item;
		base->sub_tail = item;
	}
	item->parent = base;
	item->parent->sub_cnt++;
	item->parent->parent->sub_count++;
}

static void
handle_sub_resource (SPLIT *split, RESOURCE *base)
{
	unsigned long	row, col;
	char		*sidname = NULL;
	SUBRESOURCE	*sub;

	if ((split == NULL) || (base == NULL)) abort ();

	if (!parse_row (split->lst[1], &row)) {
		printf ("*** Error: line %lu: failed to parse subresource row idx\n", split->lc);
		return;
	}
	if (!parse_col (split->lst[2], &col)) {
		printf ("*** Error: line %lu: failed to parse subresource column idx\n", split->lc);
		return;
	}
	if (!parse_sid (split->lst[3], base->name, &sidname)) {
		printf ("*** Error: line %lu: failed to parse subresource ID\n", split->lc);
		return;
	}

	TYPEMALLOC (sub, SUBRESOURCE);
	sub->sid  = base->sub_cnt + 1;
	sub->name = sidname;
	sub->row  = row;
	sub->col  = col;

	add_to_sublist (base, sub);
}

static void
parse_line (char *line, unsigned long lc, RESLIST *list)
{
	char	*p;
	SPLIT	split;

	if ((line == NULL) || (list == NULL)) abort();

	if (!prep_line (line, &p)) return;
	if (!split_line (line, lc, p, &split)) return;

	if ((split.cnt >= 3) && (strcmp (split.lst[0], ":") == 0))
		handle_sub_resource (&split, list->tail);
	else
		handle_resource (&split, list);

	free (p);
}

RESLIST *
parse_cfg (char *file)
{
	RESLIST		*list = NULL;
	FILE		*cfg;
	unsigned long	lc;
	char		buffer[256];

	TYPEMALLOC (list, RESLIST);

	cfg = fopen (file, "r");
	if (cfg == NULL) {
		printf ("*** Error: failed to open file \"%s\" for reading!\n", file);
		return (list);
	}

	lc = 0;
	while (fgets (buffer, sizeof (buffer), cfg) != NULL) {
		lc++; parse_line (buffer, lc, list);
	} fclose (cfg);

	return (list);
}

