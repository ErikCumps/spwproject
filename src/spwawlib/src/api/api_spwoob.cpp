/** \file
 * The SPWaW Library - SPWaW OOB API implementation.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_spwoob.h>
#include "common/internal.h"
#include "spwoob/spwoob.h"

SPWAWLIB_API SPWAW_ERROR
SPWAW_oob_data	(SPWOOB *oob, BYTE id, SPWOOB_DATA **data)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	CSPWINIT;
	CNULLARG (oob); CNULLARG (data);

	*data = spwoob_data (oob, id);
	if (!*data) rc = SPWERR_BADOOB;

	return (rc);
}

SPWAWLIB_API const char *
SPWAW_oob_nation (BYTE id)
{
	return (spwoob_id2nation (id & 0xFF));
}

SPWAWLIB_API const char *
SPWAW_oob_people (BYTE id)
{
	return (spwoob_id2people (id & 0xFF));
}

SPWAWLIB_API const char *
SPWAW_oob_prefix (BYTE id)
{
	return (spwoob_id2prefix (id & 0xFF));
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_oob_dump (SPWOOB *oob, char *base)
{
	CSPWINIT;
	CNULLARG (oob); CNULLARG (base);

	spwoob_dump (oob, base);

	return (SPWERR_OK);
}

SPWAWLIB_API const char *
SPWAW_oob_uclass (SPWOOB_UCLASS e)
{
	if ((e < SPWOOB_UCLASS_START) || (e > SPWOOB_UCLASS_LIMIT)) return ("???");
	return (SPWOOB_UCLASS_lookup(e));
}
