/** \file
 * The SPWaW Library - unit history tracking handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "uht/uht.h"
#include "common/internal.h"

SPWAW_ERROR
UHT_init (SPWAW_UHT *uht, SPWAW_DOSSIER *dossier)
{
	CNULLARG (uht); CNULLARG (dossier);

	clear_ptr (uht);

	uht->dossier = dossier;

	return (SPWERR_OK);
}
