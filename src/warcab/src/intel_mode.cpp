/** \file
 * The SPWaW war cabinet - intel mode definition.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "intel_mode.h"
#include "res/res.h"
#include "resource/resource.h"

const char *
intelmode2str (INTEL_MODE mode)
{
	switch (mode) {
		case INTEL_MODE_FULL:
		default:
			return ("full");
			break;
		case INTEL_MODE_LMTD:
			return ("limited");
			break;
		case INTEL_MODE_NONE:
			return ("none");
			break;
	}
	return ("???");
}

bool
intelmode2html (INTEL_MODE mode, UtilStrbuf &str, const char *style)
{
	str.clear();
	switch (mode) {
		default:
			return (false);
		case INTEL_MODE_FULL:
		case INTEL_MODE_LMTD:
		case INTEL_MODE_NONE:
			break;
	}

	if (style) str.printf ("<%s>", style);
	switch (mode) {
		case INTEL_MODE_FULL:
			str.printf ("<font color=%s>", qPrintable(RES_htmlcolor (RID_RGB_INTEL_FULL)));
			str.printf ("Full intel available!");
			str.printf ("</font>");
			break;
		case INTEL_MODE_LMTD:
			str.printf ("<font color=%s>", qPrintable(RES_htmlcolor (RID_RGB_INTEL_LMTD)));
			str.printf ("Limited intel available!");
			str.printf ("</font>");
			break;
		case INTEL_MODE_NONE:
			str.printf ("<font color=%s>", qPrintable(RES_htmlcolor (RID_RGB_INTEL_NONE)));
			str.printf ("No intel available!");
			str.printf ("</font>");
			break;
	}
	if (style) str.printf ("</%s>", style);
	str.printf ("\n");

	return (true);
}

void
intelmode2label (INTEL_MODE mode, QLabel *label)
{
	char		buf[256];	UtilStrbuf	str(buf, sizeof (buf), true, true);

	intelmode2html (mode, str, "h2");
	label->setText(buf); label->show();
}

void
intelmode2label (INTEL_MODE mode, bool pflag, QLabel *label)
{
	if (!pflag) {
		intelmode2label (mode, label);
	} else {
		label->clear(); label->hide();
	}
}

INTEL_MODE
raw2intelmode (int mode)
{
	switch (mode) {
		case 0:
		case 1:
		case 2:
			return ((INTEL_MODE)mode);
			break;
		default:
			return (INTEL_MODE_FULL);
			break;
	}
}

int
intelmode2raw (INTEL_MODE mode)
{
	switch (mode) {
		case INTEL_MODE_FULL:
		case INTEL_MODE_LMTD:
		case INTEL_MODE_NONE:
			return ((int)mode);
			break;
		default:
			return (0);
			break;
	}
}