/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "gamefile/gamefile.h"
#include "utils/ud.h"
#include "common/internal.h"
#include "common/types.h"

void
sec37_prepare (STRUCT37 *src)
{
	GAME_DATA	*data;

	if (!src) return;

	data = &(src->u.d.data);
	if (sazslen(data->location) == 0) sazsset (data->location, "unknown location");
}

SPWAW_ERROR
sec37_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab)
{
	GAME_DATA		*data;
	SPWAW_SNAP_BATTLE_RAW	*bp;
	SPWAW_SNAP_CAMPAIGN_RAW	*cp;
	int			i;

	CNULLARG (src); CNULLARG (dst);

	sec37_prepare (&(src->sec37));

	data = &(src->sec37.u.d.data);
	bp   = &(dst->raw.game.battle);
	cp   = &(dst->raw.game.campaign);

	bp->year  = data->Ygame;
	bp->month = data->Mgame;
	//bp->day   = data->Dgame;
	//bp->hour  = data->Hgame;
	// SPWW2 games do not seem to have a battle day and hour!
	// So we assume battles to start at 09h00, the first day of the month.
	bp->day        = 1;
	bp->hour       = 9;
	bp->location   = azstrstab (data->location, stab);
	bp->terrain    = data->terrain;
	// SPWW2 does not record weather, assume it's excellent weather
	bp->weather    = W_BEST;
	bp->visibility = data->vis;
	bp->turn       = data->turn;
	bp->turn_max   = data->turn_max;
	bp->OOB_p1     = data->OOBc1;
	bp->OOB_p2     = data->OOBc2;
	bp->OOB_p3     = data->OOBc3;
	bp->OOB_p4     = data->OOBc4;
	bp->miss_p1    = data->P1mission;
	bp->miss_p2    = data->P2mission;

	cp->campaign    = data->campaign;
	cp->start_year  = data->Ystart;
	cp->start_month = data->Mstart;
	cp->end_year    = data->Yend;
	cp->end_month   = data->Mend;
	cp->battles     = data->battles;
	cp->majvics     = data->DVcount;
	cp->minvics     = data->MVcount;
	// FIXME! Dcount, MDcount, DDcount
	cp->battles_max = data->battles_max;

	cp->P1BLmen     = data->P1BLmen;
	cp->P1BLart     = data->P1BLart;
	cp->P1BLsoft    = data->P1BLsoft;
	cp->P1BLapc     = data->P1BLapc;
	cp->P1BLafv     = data->P1BLafv;
	cp->P1BLgliders = 0;			// FIXME!
	cp->P1BLair     = 0;			// FIXME!
	cp->P2BLmen     = data->P2BLmen;
	cp->P2BLart     = data->P2BLart;
	cp->P2BLsoft    = data->P2BLsoft;
	cp->P2BLapc     = data->P2BLapc;
	cp->P2BLafv     = data->P2BLafv;
	cp->P2BLgliders = 0;			// FIXME!
	cp->P2BLair     = 0;			// FIXME!

	cp->P1TLmen     = data->P1TLmen;
	cp->P1TLart     = data->P1TLart;
	cp->P1TLsoft    = data->P1TLsoft;
	cp->P1TLapc     = data->P1TLapc;
	cp->P1TLafv     = data->P1TLafv;
	cp->P1TLgliders = 0;			// FIXME!
	cp->P1TLair     = 0;			// FIXME!
	cp->P2TLmen     = data->P2TLmen;
	cp->P2TLart     = data->P2TLart;
	cp->P2TLsoft    = data->P2TLsoft;
	cp->P2TLapc     = data->P2TLapc;
	cp->P2TLafv     = data->P2TLafv;
	cp->P2TLgliders = 0;			// FIXME!
	cp->P2TLair     = 0;			// FIXME!

	cp->busy = data->busy;

	cp->P1score = data->P1score;
	cp->P2score = data->P2score;

	for (i=0; i<VHEXCOUNT; i++) {
		bp->vhex[i].x	  = data->vhex.x[i];
		bp->vhex[i].y     = data->vhex.y[i];
		bp->vhex[i].value = data->vhex.amount[i];
		bp->vhex[i].owner = data->vhex.owner[i];
	}

	if (cfg.withUD) {
		SPWAW_UD *UD = &(dst->raw.game.UD);
		UD_init (UD, sizeof (*data));
		UD_ADD (UD, data, __data00);
		UD_ADD (UD, data, __data01);
		UD_ADD (UD, data, __data02);
		UD_ADD (UD, data, __data04);
		UD_ADD (UD, data, __data05);
		UD_ADD (UD, data, __data06);
		UD_ADD (UD, data, __data07);
		UD_ADD (UD, data, __data08);
		UD_ADD (UD, data, __data09);
		UD_ADD (UD, data, __data10);
		UD_ADD (UD, data, __data11);
		UD_ADD (UD, data, __data12);
		UD_ADD (UD, data, __data13);
		UD_ADD (UD, data, __data14);
		UD_ADD (UD, data, __data15);
		UD_ADD (UD, data, __data16);
		UD_ADD (UD, data, __data17);
		UD_ADD (UD, data, __data18);
		UD_ADD (UD, data, __data19);
		UD_ADD (UD, data, __data20);
	}

	return (SPWERR_OK);
}
