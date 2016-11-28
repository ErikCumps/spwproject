/** \file
 * The SPWaW war cabinet - utility functions - information delta handling.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "../common.h"
#include "util_spwdlt.h"


/* Convenience macro */
#define	DOUBLEPREC	1.0e-6

void
SPWDLT_prep (SPWDLT *dlt, SPWDLT_TYPE type, void *curr, void *base, unsigned int offs)
{
	if (!dlt) return;

	// cppcheck-suppress memsetClassFloat
	memset (dlt, 0, sizeof (*dlt));

	dlt->type = type;
	dlt->curr = curr ? (void *)((char *)curr + offs) : NULL;
	dlt->base = base ? (void *)((char *)base + offs) : NULL;

	if (!dlt->base || !dlt->curr) {
		dlt->type  = SPWDLT_NONE;
		dlt->used  = true;
		dlt->delta = false;
	} else if (dlt->base == dlt->curr) {
		dlt->used  = true;
		dlt->delta = false;
	}
}

void
SPWDLT_set (SPWDLT *dlt)
{
	int	d = 0;

	if (!dlt || dlt->used) return;

	switch (dlt->type) {
		case SPWDLT_BOOL:
			dlt->dlt.b = *((bool *)dlt->curr) - *((bool *)dlt->base);
			dlt->delta = (dlt->dlt.b != 0) ? true : false;
			break;
		case SPWDLT_INT:
			dlt->dlt.i = *((int *)dlt->curr) - *((int *)dlt->base);
			dlt->delta = (dlt->dlt.i != 0) ? true : false;
			break;
		case SPWDLT_DBL:
			dlt->dlt.d = *((double *)dlt->curr) - *((double *)dlt->base);
			if (fabs(dlt->dlt.d) <= DOUBLEPREC) dlt->dlt.d = 0.0;
			dlt->delta = (dlt->dlt.d != 0.0) ? true : false;
			break;
		case SPWDLT_STR:
			d = strcmp (*((char **)dlt->curr), *((char **)dlt->base));
			dlt->delta = (d != 0);
                        //if (dlt->delta) dlt->dlt.s = d > 0;
			break;
		case SPWDLT_CHR:
			d = strcmp ((char *)dlt->curr, (char *)dlt->base);
			dlt->delta = (d != 0);
                        //if (dlt->delta) dlt->dlt.s = d > 0;
			break;
		case SPWDLT_NONE:
			dlt->delta = false;
		default:
			break;
	}
	dlt->used = true;
}

bool
SPWDLT_bool (SPWDLT *dlt)
{
	if (!dlt) return (false);

	if (!dlt->used) SPWDLT_set (dlt);
	return (*((bool *)dlt->curr));
}

int
SPWDLT_int (SPWDLT *dlt)
{
	if (!dlt) return (0);

	if (!dlt->used) SPWDLT_set (dlt);
	return (*((int *)dlt->curr));
}

double
SPWDLT_dbl (SPWDLT *dlt)
{
	if (!dlt) return (0.0);

	if (!dlt->used) SPWDLT_set (dlt);
	return (*((double *)dlt->curr));
}

char *
SPWDLT_str (SPWDLT *dlt)
{
	if (!dlt) return (NULL);

	if (!dlt->used) SPWDLT_set (dlt);
	return (*((char **)dlt->curr));
}

char
SPWDLT_chr (SPWDLT *dlt)
{
	if (!dlt) return ('\0');

	if (!dlt->used) SPWDLT_set (dlt);
	return (*((char *)dlt->curr));
}

bool
SPWDLT_check (SPWDLT *dlt)
{
	if (!dlt) return (false);

	if (!dlt->used) SPWDLT_set (dlt);
	return (dlt->delta);

}

int
SPWDLT_getbool (SPWDLT *dlt)
{
	if (!dlt) return (0);

	if (!dlt->used) SPWDLT_set (dlt);
	return (dlt->dlt.b);
}

int
SPWDLT_getint (SPWDLT *dlt)
{
	if (!dlt) return (0);

	if (!dlt->used) SPWDLT_set (dlt);
	return (dlt->dlt.i);
}

double
SPWDLT_getdbl (SPWDLT *dlt)
{
	if (!dlt) return (0.0);

	if (!dlt->used) SPWDLT_set (dlt);
	return (dlt->dlt.d);
}

bool
SPWDLT_getstr (SPWDLT *dlt)
{
	if (!dlt) return (false);

	if (!dlt->used) SPWDLT_set (dlt);
	return (dlt->delta);
}

bool
SPWDLT_getchr (SPWDLT *dlt)
{
	if (!dlt) return (false);

	if (!dlt->used) SPWDLT_set (dlt);
	return (dlt->delta);
}

int
SPWDLT_summary (SPWDLT *dlt)
{
	int	d = 0;

	if (!dlt) return (0);

	if (!dlt->used) SPWDLT_set (dlt);
	switch (dlt->type) {
		case SPWDLT_BOOL:
			d = (dlt->dlt.b > 0) ? 1 : ((dlt->dlt.b < 0) ? -1 : 0);
			break;
		case SPWDLT_INT:
			d = (dlt->dlt.i > 0) ? 1 : ((dlt->dlt.i < 0) ? -1 : 0);
			break;
		case SPWDLT_DBL:
			d = (dlt->dlt.d > 0.0) ? 1 : ((dlt->dlt.d < 0.0) ? -1 : 0);
			break;
		case SPWDLT_STR:
			d = dlt->dlt.s;
			break;
		case SPWDLT_CHR:
			d = dlt->dlt.s;
			break;
		default:
			d = 0;
			break;
	}
	return (d);
}
