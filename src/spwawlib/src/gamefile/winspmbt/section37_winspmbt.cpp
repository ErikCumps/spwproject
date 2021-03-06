/** \file
 * The SPWaW Library - gamefile handling - winSPMBT game data.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "gamefile/gamedata.h"
#include "gamefile/winspmbt/gamedata_winspmbt.h"
#include "utils/ud.h"
#include "common/internal.h"
#include "common/types.h"

void
section37_winspmbt_prepare (WINSPMBT_SECTION37 *src)
{
	WINSPMBT_GAME_DATA	*data;

	if (!src) return;

	data = &(src->u.d.data);
	if (sazslen(data->location) == 0) sazsset (data->location, "unknown location");
}

SPWAW_ERROR
section37_winspmbt_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab)
{
	WINSPMBT_GAME_DATA	*data;
	SPWAW_SNAP_BATTLE_RAW	*bp;
	SPWAW_SNAP_CAMPAIGN_RAW	*cp;
	int			i;

	CNULLARG (src); CNULLARG (dst);

	section37_winspmbt_prepare (&(GDWINSPMBT(src)->sec37));

	data = &(GDWINSPMBT(src)->sec37.u.d.data);
	bp   = &(dst->raw.game.battle);
	cp   = &(dst->raw.game.campaign);

	// winSPMBT games do not have a battle day and hour!
	bp->year  = data->Ygame;
	bp->month = data->Mgame;
	bp->day   = 0;
	bp->hour  = 0;

	bp->location   = azstrstab (data->location, stab);
	bp->terrain    = data->terrain;
	// winSPMBT does not record weather, assume it's excellent weather
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
	cp->P1BLgliders = data->P1BLairtpt;	// FIXME: winSPMBT records glider losses as aircraft losses!
	cp->P1BLair     = data->P1BLaircraft;
	cp->P2BLmen     = data->P2BLmen;
	cp->P2BLart     = data->P2BLart;
	cp->P2BLsoft    = data->P2BLsoft;
	cp->P2BLapc     = data->P2BLapc;
	cp->P2BLafv     = data->P2BLafv;
	cp->P2BLgliders = data->P2BLairtpt;	// FIXME: winSPMBT records glider losses as aircraft losses!
	cp->P2BLair     = data->P2BLaircraft;

	cp->P1TLmen     = data->P1TLmen;
	cp->P1TLart     = data->P1TLart;
	cp->P1TLsoft    = data->P1TLsoft;
	cp->P1TLapc     = data->P1TLapc;
	cp->P1TLafv     = data->P1TLafv;
	cp->P1TLgliders = data->P1TLairtpt;	// FIXME: winSPMBT records glider losses as aircraft losses!
	cp->P1TLair     = data->P1TLaircraft;
	cp->P2TLmen     = data->P2TLmen;
	cp->P2TLart     = data->P2TLart;
	cp->P2TLsoft    = data->P2TLsoft;
	cp->P2TLapc     = data->P2TLapc;
	cp->P2TLafv     = data->P2TLafv;
	cp->P2TLgliders = data->P2TLairtpt;	// FIXME: winSPMBT records glider losses as aircraft losses!
	cp->P2TLair     = data->P2TLaircraft;

	cp->busy = data->busy;

	cp->P1score = data->P1score;
	cp->P2score = data->P2score;

	cp->P1result = data->P1result;
	cp->P2result = data->P2result;

	for (i=0; i<WINSPMBT_VHEXCOUNT; i++) {
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
		UD_ADD (UD, data, __data03);
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
