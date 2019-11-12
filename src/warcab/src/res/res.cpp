/** \file
 * The SPWaW war cabinet - resource handling.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include <builtin/no_such_icon.xpm>

#include "../common.h"
#include "res/res_private.h"
#include "res/res_arc.h"
#include "res/res_img.h"
#include "res/res_imgcol.h"
#include "res/res_rgb.h"
#include "res/res_rgba.h"
#include "res/res_xpm.h"

QPixmap	*RES_PIXMAP_NONE	= NULL;
QColor	*RES_COLOR_NONE		= NULL;
QString	RES_HTMLCOLOR_NONE	= QString ("#000000");

static void	statereport	(SL_STDBG_INFO_LEVEL level);

/*! Module name */
static const char	*MODULE = "RES";

static SL_BOOL		initialized = SL_false;

static unsigned long	res_count = 0;
static unsigned long	res_list_size = 0;
static RESITEM		*res_list = NULL;

SL_ERROR
RES_init (void)
{
	SL_ERROR	rc;
	unsigned long	i;

	if (initialized) RETURN_OK;

	RES_PIXMAP_NONE	= new QPixmap (no_such_icon_xpm);	DEVASSERT (RES_PIXMAP_NONE != NULL);
	RES_COLOR_NONE	= new QColor (255,255,255);		DEVASSERT (RES_COLOR_NONE != NULL);

	rc = RES_ARC_init (RESOURCE_LIB);	SL_ROE (rc);
	rc = RES_IMG_init ();			SL_ROE (rc);
	rc = RES_IMGCOL_init ();		SL_ROE (rc);
	rc = RES_RGB_init ();			SL_ROE (rc);
	rc = RES_RGBA_init ();			SL_ROE (rc);
	rc = RES_XPM_init ();			SL_ROE (rc);

	res_count = RESOURCE_LIST_SIZE;
	res_list_size = sizeof (RESITEM) * res_count;
	SL_SAFE_CALLOC (res_list, 1, res_list_size);

	for (i=0; i<res_count; i++) {
		res_list[i].info = &(RESOURCE_LIST[i]);

		rc = RES_ARC_item (res_list[i].info->id, &(res_list[i].item)); SL_ROE (rc);

		switch (res_list[i].info->type) {
			case arc_IMG:
				rc = RES_IMG_handle (&(res_list[i]));
				break;
			case arc_IMGCOL:
				rc = RES_IMGCOL_handle (&(res_list[i]));
				break;
			case arc_RGB:
				rc = RES_RGB_handle (&(res_list[i]));
				break;
			case arc_RGBA:
				rc = RES_RGBA_handle (&(res_list[i]));
				break;
			case arc_XPM:
				rc = RES_XPM_handle (&(res_list[i]));
				break;
			default:
				res_list[i].data = NULL;
				break;
		}
		SL_ROE (rc);
	}

	SL_STDBG_add (statereport, MODULE);

	atexit (RES_shutdown);

	initialized = SL_true;
	RETURN_OK;
}

void
RES_shutdown (void)
{
	unsigned long	i;

	if (!initialized) return;

	SL_STDBG_delete (statereport);

	for (i=0; i<res_count; i++) {
		switch (res_list[i].info->type) {
			case arc_IMG:
				RES_IMG_cleanup (&(res_list[i]));
				break;
			case arc_IMGCOL:
				RES_IMGCOL_cleanup (&(res_list[i]));
				break;
			case arc_RGB:
				RES_RGB_cleanup (&(res_list[i]));
				break;
			case arc_RGBA:
				RES_RGBA_cleanup (&(res_list[i]));
				break;
			case arc_XPM:
				RES_XPM_cleanup (&(res_list[i]));
				break;
			default:
				break;
		}
	}
	memset (res_list, 0, res_list_size);
	SL_SAFE_FREE (res_list);

	RES_XPM_shutdown ();
	RES_RGBA_shutdown ();
	RES_RGB_shutdown ();
	RES_IMGCOL_shutdown ();
	RES_IMG_shutdown ();
	RES_ARC_shutdown ();

	delete RES_PIXMAP_NONE;	RES_PIXMAP_NONE = NULL;
	delete RES_COLOR_NONE;	RES_COLOR_NONE = NULL;

	initialized = SL_false;
}

static RESITEM *
find_res_item (unsigned long id)
{
	RESITEM	*p = NULL;

	/* Try a smart guess first */
	if ((id >= 1) && (id <= res_count) && (res_list[id-1].info->id == id)) {
		p = &(res_list[id-1]);
	} else {
		unsigned long	i;

		for (i=0; i<res_count; i++) {
			if (res_list[i].info->id == id) break;
		}
		if (i < res_count) p = &(res_list[i]);
	}
	return (p);
}

QPixmap *
RES_pixmap (unsigned long rid)
{
	RESITEM	*p = NULL;
	QPixmap	*pm;

	p = find_res_item (rid);
	if (!p) return (RES_PIXMAP_NONE);

	switch (p->info->type) {
		case arc_IMG:
			pm = RES_IMG_get (p);
			break;
		case arc_XPM:
			pm = RES_XPM_get (p);
			break;
		default:
			pm = NULL;
			break;
	}

	if (!pm) pm = RES_PIXMAP_NONE;

	return (pm);
}

QPixmap *
RES_pixmap (unsigned long rid, unsigned long sid)
{
	RESITEM	*p = NULL;
	QPixmap	*pm = NULL;

	p = find_res_item (rid);
	if (!p) return (RES_PIXMAP_NONE);

	if ((p->info->type != arc_IMGCOL) || (sid < 1) || (sid > p->info->sub_cnt)) return (RES_PIXMAP_NONE);

	pm = RES_IMGCOL_get (p, p->info->sub_tab[sid-1].row, p->info->sub_tab[sid-1].col);
	if (!pm) pm = RES_PIXMAP_NONE;

	return (pm);
}

QColor *
RES_color (unsigned long rid)
{
	RESITEM	*p = NULL;
	QColor	*c = NULL;

	p = find_res_item (rid);
	if (!p) return (RES_COLOR_NONE);

	switch (p->info->type) {
		default:
			break;
		case arc_RGB:
			c = RES_RGB_get (p);
			break;
		case arc_RGBA:
			c = RES_RGBA_get (p);
			break;
	}
	if (!c) c = RES_COLOR_NONE;

	return (c);
}

QString
RES_htmlcolor (unsigned long rid)
{
	RESITEM	*p = NULL;
	QString	c;

	p = find_res_item (rid);
	if (!p) return (RES_HTMLCOLOR_NONE);

	switch (p->info->type) {
		default:
			c = RES_HTMLCOLOR_NONE;
			break;
		case arc_RGB:
			c = RES_RGB_gethtml (p);
			break;
		case arc_RGBA:
			c = RES_RGBA_gethtml (p);
			break;
	}
	if (c.isNull()) c = RES_HTMLCOLOR_NONE;

	return (c);
}

static void
statereport (SL_STDBG_INFO_LEVEL level)
{
	unsigned long	i;

	SAYSTATE0 ("### Resource state:\n");

	/* basic information */
	if (level >= SL_STDBG_LEVEL_BAS) {
		SAYSTATE1 ("\tinitialized    = %s\n", SL_BOOL_tostr (initialized));
		SAYSTATE1 ("\tresource count = %lu\n", res_count);
	}

	/* extended information */
	if (level >= SL_STDBG_LEVEL_EXT) {
		SAYSTATE1 ("\tresource list = 0x%8.8x\n", res_list);
		for (i = 0; i < res_count; i++) {
			SAYSTATE4 ("\t\t[%03.3lu] id %lu <%s> [%s]\n", i,
				res_list[i].info->id, res_list[i].info->name, arc_type_name (res_list[i].info->type));
			if (level >= SL_STDBG_LEVEL_DEEP) {
				SAYSTATE2 ("\t\t\tarc item=0x%8.8x, data=0x%8.8x\n", res_list[i].item, res_list[i].data);
				switch (res_list[i].info->type) {
					case arc_IMG:
						RES_IMG_statereport (&(res_list[i]));
						break;
					case arc_IMGCOL:
						RES_IMGCOL_statereport (&(res_list[i]));
						break;
					case arc_RGB:
						RES_RGB_statereport (&(res_list[i]));
						break;
					case arc_RGBA:
						RES_RGBA_statereport (&(res_list[i]));
						break;
					case arc_XPM:
						RES_XPM_statereport (&(res_list[i]));
						break;
					default:
						break;
				}
			}
		}
	}

	/* deep probe */
	if (level >= SL_STDBG_LEVEL_DEEP) {
		SAYSTATE1("\tRES_PIXMAP_NONE = 0x%8.8x\n", RES_PIXMAP_NONE);
		SAYSTATE4("\tRES_COLOR_NONE  = 0x%8.8x (R=%d, G=%d, B=%d)\n", RES_COLOR_NONE,
			RES_COLOR_NONE->red(), RES_COLOR_NONE->green(), RES_COLOR_NONE->blue());
	}

	SAYSTATE0 ("\n");
}
