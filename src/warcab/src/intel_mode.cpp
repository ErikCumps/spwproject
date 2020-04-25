/** \file
 * The SPWaW war cabinet - intel mode definition.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "intel_mode.h"

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