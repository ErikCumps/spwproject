/** \file
 * The SPWaW war cabinet - strategic map - data definitions.
 *
 * Copyright (C) 2012-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "smap_hexdata.h"

int	SMAP_HRD_mask[SMAP_HRD_CNT]	= { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20 };
int	SMAP_HRD_180mask[SMAP_HRD_CNT]	= { 0x08, 0x10, 0x20, 0x01, 0x02, 0x04 };
