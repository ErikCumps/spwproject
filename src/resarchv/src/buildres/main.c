/** \file
 * Resource Archive - buildres tool.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include <stdio.h>
#include <stdlib.h>

#include <resarchv.h>
#include <system/independence.h>

#include "config.h"
#include "idhash.h"
#include "idnamehash.h"
#include "parse_cfg.h"
#include "util.h"

/* Global variables */
static char *signature = NULL;

/* Statistics */
static int STATS_items = 0;

void
app_init (void)
{
	ih_init ();
	nh_init ();
}

void
app_shutdown (void)
{
	ih_shutdown ();
	nh_shutdown ();
}
int
mkhdr (RESLIST *list, unsigned long count)
{
	RESOURCE	*p;
	SUBRESOURCE	*q;
	FILE		*hdr;
	char		*name;
	unsigned int	i;

	hdr = fopen (cfg.outhdr, "w");
	if (!hdr) return (0);

	SAFE_STRDUP (cfg.cfg, &name);
	for (i=0; i<strlen (name); i++) if (!isalnum(name[i])) name[i] = '_';

	fprintf (hdr,
		"/* *** CAUTION: GENERATED FILE! ***\n"
		" *\n"
		" * ResArchv header file for %s\n"
		" *\n"
		" */\n"
		"\n",
		cfg.cfg);

	fprintf (hdr,
		"#ifndef\tRESARCHV_%s_H\n"
		"#define\tRESARCHV_%s_H\t1\n"
		"\n"
		"#include <resarchv.h>\n"
		"\n"
		"#ifdef\t__cplusplus\n"
		"extern \"C\" {\n"
		"#endif\t/* __cplusplus */\n"
		"\n\n",
		name, name);

	fprintf (hdr,
		"/* Resource library filename */\n"
		"#define\tRESOURCE_LIB\t\"%s\"\n"
		"\n",
		cfg.outlib);

	fprintf (hdr, "/* Resource IDs */\n");
	p = list->list;
	while (p) {
		fprintf (hdr, "#define\tRID_%-32s\t%lu\t/* %s type */\n", p->name, p->id, arc_type_lookup (p->type));
		p = p->next;
	}
	fprintf (hdr, "\n");

	fprintf (hdr, "/* Subresource IDs */\n");
	p = list->list;
	while (p) {
		if (p->sub_cnt) {
			q = p->sub_list;
			while (q) {
				fprintf (hdr, "#define\tSID_%-32s\t%lu\n", q->name, q->sid);
				q = q->next;
			}
		}
		p = p->next;
	}
	fprintf (hdr, "\n");

	fprintf (hdr, "/* Aggregate subresource IDs */\n");
	p = list->list;
	while (p) {
		if (p->sub_cnt) {
			q = p->sub_list;
			while (q) {
				fprintf (hdr, "#define\tRID_%-32s\tRID_%s,SID_%s\n", q->name, p->name, q->name);
				q = q->next;
			}
		}
		p = p->next;
	}
	fprintf (hdr, "\n");

	fprintf (hdr,
		"/* Subresource Identifier */\n"
		"typedef struct {\n"
		"\tunsigned long\t\tsid;\t\t/* subresource ID */\n"
		"\tconst char\t\t*name;\t\t/* subresource ID name */\n"
		"\tunsigned long\t\trow;\t\t/* optional subresource row ID */\n"
		"\tunsigned long\t\tcol;\t\t/* optional subresource column ID */\n"
		"} SUBRESOURCE;\n"
		"\n"
		);

	fprintf (hdr,
		"/* Resource list element */\n"
                "typedef struct {\n"
		"\tunsigned long\t\tid;\t\t/* resource ID */\n"
		"\tconst char\t\t*name;\t\t/* resource ID name */\n"
		"\tarc_type_t\t\ttype;\t\t/* resource type */\n"
		"\tunsigned long\t\tmeta[2];\t/* additional type-specific resource data */\n"
		"\tconst SUBRESOURCE\t*sub_tab;\t/* subresource ID table */\n"
		"\tunsigned long\t\tsub_cnt;\t/* subresource ID table size */\n"
		"} RESOURCE_LIST_ELEMENT;\n"
		"\n"
		);

	fprintf (hdr,
		"/* Resource list size */\n"
		"#define\tRESOURCE_LIST_SIZE\t%lu\n"
		"\n",
		count);

	fprintf (hdr,
		"/* Resource list array */\n"
		"extern const RESOURCE_LIST_ELEMENT\tRESOURCE_LIST[RESOURCE_LIST_SIZE];\n"
		"\n"
		);


	fprintf (hdr,
		"\n\n"
		"#ifdef\t__cplusplus\n"
		"}\n"
		"#endif\t/* __cplusplus */\n"
		"#endif\t/* RESARCHV_%s_H */\n"
		"\n",
		name);

	fclose (hdr);

	return (1);
}

static char *
sid_quoted (char *str)
{
	const char	*sid = "SID_";
	unsigned long	len;
	char		*p = NULL;

	if (!str) return (NULL);

	len = strlen (str) + strlen (sid) + 4;
	SIZEMALLOC (p, char, len);
	snprintf (p, len - 1, "\"%s%s\",", sid, str);
	return (p);
}

int
mksrc (RESLIST *list, unsigned long count)
{
	RESOURCE	*p;
	SUBRESOURCE	*q;
	FILE		*src;
	char		*s;

	UNREFERENCED (count);

	src = fopen (cfg.outsrc, "w");
	if (!src) return (0);

	fprintf (src, "#include \"%s\"\n\n", cfg.outhdr);
	fprintf (src, "#include <stdlib.h>\n\n");

	fprintf (src,
		"/* Convenience macros */\n"
		"#define BIN(id_)\t\t\t{ id_, #id_, arc_BIN, { 0, 0 }, NULL, 0 }\n"
		"#define IMG(id_)\t\t\t{ id_, #id_, arc_IMG, { 0, 0 }, NULL, 0 }\n"
		"#define IMGCOL(id_,w_,h_,sp_,sc_)\t{ id_, #id_, arc_IMGCOL, { w_, h_ }, SUBRESOURCE_LIST_##sp_, sc_ }\n"
		"#define RGB(id_)\t\t\t{ id_, #id_, arc_RGB, { 0, 0 }, NULL, 0 }\n"
		"#define RGBA(id_)\t\t\t{ id_, #id_, arc_RGBA, { 0, 0 }, NULL, 0 }\n"
		"#define XPM(id_)\t\t\t{ id_, #id_, arc_XPM, { 0, 0 }, NULL, 0 }\n"
		"#define SID(sid_,n_,r_,c_)\t\t{ sid_, n_, r_, c_ }\n"
		"\n"
		);

	p = list->list;
	while (p) {
		if (p->sub_cnt) {
			fprintf (src,
				"/* Subresource list */\n"
				"const SUBRESOURCE\tSUBRESOURCE_LIST_%s[%lu] = {\n", p->name, p->sub_cnt);

			q = p->sub_list;
			while (q) {
				s = sid_quoted (q->name);
				fprintf (src, "\tSID (%3lu, %-40s\t%lu, %lu),\n", q->sid, s, q->row, q->col);
				free (s);
				q = q->next;
			}

			fprintf (src, "};\n\n");
		}
		p = p->next;
	}

	fprintf (src,
		"/* Resource list */\n"
		"const RESOURCE_LIST_ELEMENT\tRESOURCE_LIST[RESOURCE_LIST_SIZE] = {\n");

	p = list->list;
	while (p) {
		switch (p->type) {
			case arc_IMG:
				fprintf (src, "\tIMG (RID_%s),\n", p->name);
				break;
			case arc_IMGCOL:
				fprintf (src, "\tIMGCOL (RID_%s, %lu, %lu, %s, %lu),\n", p->name, p->meta[0], p->meta[1], p->name, p->sub_cnt);
				break;
			case arc_RGB:
				fprintf (src, "\tRGB (RID_%s),\n", p->name);
				break;
			case arc_RGBA:
				fprintf (src, "\tRGBA (RID_%s),\n", p->name);
				break;
			case arc_XPM:
				fprintf (src, "\tXPM (RID_%s),\n", p->name);
				break;
			case arc_BIN:
			default:
				fprintf (src, "\tBIN (RID_%s),\n", p->name);
				break;
		}
		p = p->next;
	}

	fprintf (src, "};\n\n");

	fclose (src);

	return (1);
}

int
main (int argc, char *argv[])
{
	unsigned long	count = 0;
	int		rc;
	RESLIST		*list;
	arc_t		*arc;
	RESOURCE	*p;

	MEMTRACE_INIT ("./debug_memtrace.log");

	app_init ();
	parse_args (argc, argv);

	list = parse_cfg (cfg.cfg);
	printf ("Resource configuration file \"%s\" specifies:\n\t%lu resources\n\t%lu subresources\n\n",
		cfg.cfg, list->count, list->sub_count);

	arc = arc_new (cfg.outlib, (arc_sig_t *)signature);
	if (arc == NULL) {
		printf ("*** Error: failed to create empty archive!\n");
		exit (42);
	}

	p = list->list;
	while (p) {
		rc = 0;
		switch (p->type) {
			case arc_BIN:
			case arc_IMG:
			case arc_IMGCOL:
			default:
				rc = arc_add_file (arc, p->type, p->id, p->name, p->data.file.fname);
				break;
			case arc_RGB:
				if (p->link) {
					rc = arc_add_link (arc, p->type, p->id, p->name, p->link->id);
				} else {
					rc = arc_add_data (arc, p->type, p->id, p->name, p->data.rgb.rgb, sizeof (p->data.rgb.rgb));
				}
				break;
			case arc_RGBA:
				if (p->link) {
					rc = arc_add_link (arc, p->type, p->id, p->name, p->link->id);
				} else {
					rc = arc_add_data (arc, p->type, p->id, p->name, p->data.rgba.rgba, sizeof (p->data.rgba.rgba));
				}
				break;
			case arc_XPM:
				rc = arc_add_data (arc, p->type, p->id, p->name, p->data.xpm.xpm, p->data.xpm.len);
				break;

		}
		if (rc) count++;

		p = p->next;
	}
	printf ("Stored %lu valid resources in resource archive.\n", count);

	if (count != list->count) {
		printf ("*** Error: failed to store all specified resources in archive!\n");
		exit (42);
	}

	if (!arc_store (arc)) {
		printf ("*** Error: failed to save resource archive!\n");
		exit (42);
	}

	if (!mkhdr (list, count)) {
		printf ("*** Error: failed to create header file!\n");
		exit (42);
	}

	if (!mksrc (list, count)) {
		printf ("*** Error: failed to create source file!\n");
		exit (42);
	}

	arc_close (arc);
	app_shutdown();

	return (0);
}

