/** \file
 * The Steel Panthers World at War report tool.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL V2
 */

#include "spwreport.h"
#include "utils.h"

static void
report_comment (FILE *rf, SPWAW_SNAP_CMT_RAW *ptr)
{
	if (!ptr) return;

	smart_title (rf, '-', "RAW game comment report:\n");

	fprintf (rf, "title : %s\n", ptr->title);
	fprintf (rf, "mapsrc: %s\n", ptr->mapsrc);

	fprintf (rf, "\n");
}

static void
report_battle (FILE *rf, SPWAW_SNAP_BATTLE_RAW *ptr)
{
	int	i;

	if (!ptr) return;

	smart_title (rf, '-', "RAW game battle report:\n");

	fprintf (rf, "{date     } year=%u, month=%u, day=%u, hour=%u\n", ptr->year, ptr->month, ptr->day, ptr->hour);
	fprintf (rf, "location  : %s\n", ptr->location);
	fprintf (rf, "terrain   : %u\n", ptr->terrain);
	fprintf (rf, "weather   : %u\n", ptr->weather);
	fprintf (rf, "visibility: %u\n", ptr->visibility);
	fprintf (rf, "turn      : %u\n", ptr->turn);
	fprintf (rf, "turn_max  : %u\n", ptr->turn_max);
	fprintf (rf, "OOB_p1    : %u\n", ptr->OOB_p1);
	fprintf (rf, "OOB_p2    : %u\n", ptr->OOB_p2);
	fprintf (rf, "OOB_p3    : %u\n", ptr->OOB_p3);
	fprintf (rf, "OOB_p4    : %u\n", ptr->OOB_p4);
	fprintf (rf, "miss_p1   : %u\n", ptr->miss_p1);
	fprintf (rf, "miss_p2   : %u\n", ptr->miss_p2);
	fprintf (rf, "credit    : %u\n", ptr->credit);
	fprintf (rf, "\n");

	fprintf (rf, "victory hexes: %u\n", SPWAW_VHEXCNT);
	for (i=0; i<SPWAW_VHEXCNT; i++) {
		fprintf (rf, "\t{%4u} x    : %d\n", i, ptr->vhex[i].x);
		fprintf (rf, "\t{%4u} y    : %d\n", i, ptr->vhex[i].y);
		fprintf (rf, "\t{%4u} value: %d\n", i, ptr->vhex[i].value);
		fprintf (rf, "\t{%4u} owner: %d\n", i, ptr->vhex[i].owner);
		fprintf (rf, "\n");
	}

	fprintf (rf, "\n");
}

static void
report_campaign (FILE *rf, SPWAW_SNAP_CAMPAIGN_RAW *ptr)
{
	if (!ptr) return;

	smart_title (rf, '-', "RAW game campaign report:\n");

	fprintf (rf, "campaign   : %u\n", ptr->campaign);
	fprintf (rf, "{start     } year=%u, month=%u\n", ptr->start_year, ptr->start_month);
	fprintf (rf, "{end       } year=%u, month=%u\n", ptr->end_year, ptr->end_month);
	fprintf (rf, "battles    : %u\n", ptr->battles);
	fprintf (rf, "majvics    : %u\n", ptr->majvics);
	fprintf (rf, "minvics    : %u\n", ptr->minvics);
	fprintf (rf, "battles_max: %u\n", ptr->battles_max);
	fprintf (rf, "Busy       : %u\n", ptr->busy);
	fprintf (rf, "{P1BL      } men=%u, art=%u, soft=%u, apc=%u, afv=%u, gliders=%u, air=%u\n",
		ptr->P1BLmen, ptr->P1BLart, ptr->P1BLsoft, ptr->P1BLapc, ptr->P1BLafv, ptr->P1BLgliders, ptr->P1BLair);
	fprintf (rf, "{P2BL      } men=%u, art=%u, soft=%u, apc=%u, afv=%u, gliders=%u, air=%u\n",
		ptr->P2BLmen, ptr->P2BLart, ptr->P2BLsoft, ptr->P2BLapc, ptr->P2BLafv, ptr->P2BLgliders, ptr->P2BLair);
	fprintf (rf, "{P1TL      } men=%u, art=%u, soft=%u, apc=%u, afv=%u, gliders=%u, air=%u\n",
		ptr->P1TLmen, ptr->P1TLart, ptr->P1TLsoft, ptr->P1TLapc, ptr->P1TLafv, ptr->P1TLgliders, ptr->P1TLair);
	fprintf (rf, "{P2TL      } men=%u, art=%u, soft=%u, apc=%u, afv=%u, gliders=%u, air=%u\n",
		ptr->P2TLmen, ptr->P2TLart, ptr->P2TLsoft, ptr->P2TLapc, ptr->P2TLafv, ptr->P2TLgliders, ptr->P2TLair);
	fprintf (rf, "P1score    : %u\n", ptr->P1score);
	fprintf (rf, "P2score    : %u\n", ptr->P2score);
	fprintf (rf, "\n");
}

static void
report_map (FILE *rf, SPWAW_SNAP_MAP_RAW *ptr)
{
	DWORD	x, y;

	if (!ptr) return;

	smart_title (rf, '-', "RAW game map report:\n");

	fprintf (rf, "width : %u\n", ptr->width);
	fprintf (rf, "height: %u\n", ptr->height);
	fprintf (rf, "\n");

	fprintf (rf, "map info:\n");
	for (y=0; y<ptr->height; y++) {
		fprintf (rf, "\trow: %u\n", y);
		for (x=0; x<ptr->width; x++) {
			fprintf (rf, "\t\t(%4u,%4u} height=%2.2d"
				" has_T1=0x%2.2x has_T2=0x%2.2x has_T3=0x%2.2x has_T4=0x%2.2x"
				" conn_road1=0x%2.2x conn_road2=0x%2.2x conn_rail=0x%2.2x"
				"\n",
				x, y, (signed char)ptr->data[y*ptr->width+x].height,
				ptr->data[y*ptr->width+x].has_T1, ptr->data[y*ptr->width+x].has_T2,
				ptr->data[y*ptr->width+x].has_T3, ptr->data[y*ptr->width+x].has_T4,
				ptr->data[y*ptr->width+x].conn_road1, ptr->data[y*ptr->width+x].conn_road2,
				ptr->data[y*ptr->width+x].conn_rail);
		}
	}
	fprintf (rf, "\n");
}

static void
report_game (FILE *rf, SPWAW_SNAP_GAME_RAW *ptr)
{
	if (!ptr) return;

	smart_title (rf, '=', "RAW game data report:\n");

	report_comment (rf, &(ptr->cmt));
	report_battle (rf, &(ptr->battle));
	report_campaign (rf, &(ptr->campaign));
	report_map (rf, &(ptr->map));
	UD_block_report (&(ptr->UD), "game", 16, rf);

	fprintf (rf, "\n");
}

static void
report_formations (FILE *rf, SPWAW_SNAP_OOB_FRAW *ptr, SPWAW_SNAP_OOB_F *iptr)
{
	DWORD			i;
	SPWAW_SNAP_OOB_FELRAW	*p;
	SPWAW_RINDEX		*r;
	char			pf[16];

	if (!ptr) return;

	smart_title (rf, '-', "RAW formations report:\n");

	fprintf (rf, "count: %u\n", ptr->cnt);
	fprintf (rf, "start: %u\n", ptr->start);
	fprintf (rf, "\n");

	if (!ptr->cnt || !ptr->raw || !ptr->ridx) return;
	memset (pf, 0, sizeof (pf));

	for (i=0; i<ptr->cnt; i++) {
		p = &(ptr->raw[i]);

		fprintf (rf, "{%4u} RID       : %4.4x\n", i, p->RID);
		fprintf (rf, "{%4u} ID        : %4.4x\n", i, p->RID);
		fprintf (rf, "{%4u} FID       : %2.2x\n", i, p->FID);
		fprintf (rf, "{%4u} name      : %s\n", i, p->name);
		fprintf (rf, "{%4u} leader    : %4.4x\n", i, p->leader);
		fprintf (rf, "{%4u} higher cmd: %4.4x\n", i, p->hcmd);
		if (p->OOBrid) {
			fprintf (rf, "{%4u} OOBrid    : %u\n", i, p->OOBrid);
		} else {
			fprintf (rf, "{%4u} OOBrid    : %u (via higher cmd link)\n", i, iptr->list[i].data.OOBrid);
		}
		fprintf (rf, "{%4u} status    : %u\n", i, p->status);
		fprintf (rf, "{%4u} player    : %u\n", i, p->player);

		snprintf (pf, sizeof (pf) - 1, "{%4u} ", i);
		fprintf (rf, "%s\n", pf);
		UD_block_report (&(p->UD), 16, rf, pf);
		fprintf (rf, "\n");
	}

	for (i=0; i<ptr->cnt; i++) {
		r = &(ptr->ridx[i]);

		fprintf (rf, "ridx[%4u] rid=%4.4x, idx=%4.4x\n", i, r->rid, r->idx);
	}
	fprintf (rf, "\n");
}

static void
report_units (FILE *rf, SPWAW_SNAP_OOB_URAW *ptr)
{
	DWORD			i, j;
	SPWAW_SNAP_OOB_UELRAW	*p;
	SPWAW_RINDEX		*r;
	char			pf[16];

	if (!ptr) return;

	smart_title (rf, '-', "RAW units report:\n");

	fprintf (rf, "count: %u\n", ptr->cnt);
	fprintf (rf, "\n");

	if (!ptr->cnt || !ptr->raw || !ptr->ridx) return;
	memset (pf, 0, sizeof (pf));

	for (i=0; i<ptr->cnt; i++) {
		p = &(ptr->raw[i]);

		fprintf (rf, "{%4u} RID       : %4.4x\n", i, p->RID);
		fprintf (rf, "{%4u} FRID      : %2.2x\n", i, p->FRID);
		fprintf (rf, "{%4u} FMID      : %2.2x\n", i, p->FMID);
		fprintf (rf, "{%4u} FSID      : %2.2x\n", i, p->FSID);
		fprintf (rf, "{%4u} name      : %s\n", i, p->name);
		fprintf (rf, "{%4u} classID   : %u\n", i, p->classID);
		fprintf (rf, "{%4u} OOB       : %u\n", i, p->OOB);
		fprintf (rf, "{%4u} OOBrid    : %u\n", i, p->OOBrid);
		fprintf (rf, "{%4u} size      : %u\n", i, p->size);
		fprintf (rf, "{%4u} cost      : %u\n", i, p->cost);
		fprintf (rf, "{%4u} survive   : %u\n", i, p->survive);
		fprintf (rf, "{%4u} leader    : %4.4x\n", i, p->leader);
		fprintf (rf, "{%4u} exp       : %u\n", i, p->exp);
		fprintf (rf, "{%4u} mor       : %u\n", i, p->mor);
		fprintf (rf, "{%4u} sup       : %u\n", i, p->sup);
		fprintf (rf, "{%4u} status    : %u\n", i, p->status);
		fprintf (rf, "{%4u} smkdev    : %u\n", i, p->smkdev);
		fprintf (rf, "{%4u} smkammo   : %u\n", i, p->smkammo);
		fprintf (rf, "{%4u} crew      : %4.4x\n", i, p->crew);
		fprintf (rf, "{%4u} range     : %u\n", i, p->range);
		fprintf (rf, "{%4u} stance_x  : %u\n", i, p->stance_x);
		fprintf (rf, "{%4u} stance_y  : %u\n", i, p->stance_y);
		fprintf (rf, "{%4u} loader    : %4.4x\n", i, p->loader);
		fprintf (rf, "{%4u} load_cap  : %u\n", i, p->load_cap);
		fprintf (rf, "{%4u} load_cost : %u\n", i, p->load_cost);
		fprintf (rf, "{%4u} radio     : %u\n", i, p->radio);
		fprintf (rf, "{%4u} rof       : %u\n", i, p->rof);
		fprintf (rf, "{%4u} tgt       : %u\n", i, p->tgt);
		fprintf (rf, "{%4u} rf        : %u\n", i, p->rf);
		fprintf (rf, "{%4u} fc        : %u\n", i, p->fc);
		fprintf (rf, "{%4u} iv        : %u\n", i, p->iv);
		fprintf (rf, "{%4u} swim      : %u\n", i, p->swim);
		fprintf (rf, "{%4u} men       : %u\n", i, p->men);
		fprintf (rf, "{%4u} men_ori   : %u\n", i, p->men_ori);
		fprintf (rf, "{%4u} speed     : %u\n", i, p->speed);
		fprintf (rf, "{%4u} moves     : %u\n", i, p->moves);
		fprintf (rf, "{%4u} damage    : %u\n", i, p->damage);
		fprintf (rf, "{%4u} movdir    : %u\n", i, p->movdir);
		fprintf (rf, "{%4u} shtdir    : %u\n", i, p->shtdir);
		fprintf (rf, "{%4u} target    : %u\n", i, p->target);
		fprintf (rf, "{%4u} UTGidx    : %u\n", i, p->UTGidx);
		fprintf (rf, "{%4u} SPECIAL_OU: %4.4x\n", i, p->SPECIAL_OU);
		fprintf (rf, "{%4u} SPECIAL   :", i);
		for (j=0; j<5; j++) fprintf (rf, " [%u]=%4.4x", j, p->SPECIAL[j]);
		fprintf (rf, "\n");

		memset (pf, 0, sizeof (pf));
		snprintf (pf, sizeof (pf) - 1, "{%4u} ", i);
		fprintf (rf, "%s\n", pf);
		UD_block_report (&(p->UD), 16, rf, pf);
		fprintf (rf, "\n");
	}

	for (i=0; i<ptr->cnt; i++) {
		r = &(ptr->ridx[i]);

		fprintf (rf, "ridx[%4u] rid=%4.4x, idx=%4.4x\n", i, r->rid, r->idx);
	}
	fprintf (rf, "\n");
}

static void
report_leaders (FILE *rf, SPWAW_SNAP_OOB_LRAW *ptr)
{
	DWORD			i;
	SPWAW_SNAP_OOB_LELRAW	*p;
	SPWAW_RINDEX		*r;
	char			pf[16];

	if (!ptr) return;

	smart_title (rf, '-', "RAW leaders report:\n");

	fprintf (rf, "count: %u\n", ptr->cnt);
	fprintf (rf, "\n");

	if (!ptr->cnt || !ptr->raw || !ptr->ridx) return;
	memset (pf, 0, sizeof (pf));

	for (i=0; i<ptr->cnt; i++) {
		p = &(ptr->raw[i]);

		fprintf (rf, "{%4u} RID   : %4.4x\n", i, p->RID);
		fprintf (rf, "{%4u} URID  : %4.4x\n", i, p->URID);
		fprintf (rf, "{%4u} name  : %s\n", i, p->name);
		fprintf (rf, "{%4u} rank  : %u\n", i, p->rank);
		fprintf (rf, "{%4u} ral   : %u\n", i, p->ral);
		fprintf (rf, "{%4u} inf   : %u\n", i, p->inf);
		fprintf (rf, "{%4u} art   : %u\n", i, p->art);
		fprintf (rf, "{%4u} arm   : %u\n", i, p->arm);
		fprintf (rf, "{%4u} kills : %u\n", i, p->kills);
		fprintf (rf, "{%4u} status: %u\n", i, p->status);

		memset (pf, 0, sizeof (pf));
		snprintf (pf, sizeof (pf) - 1, "{%4u} ", i);
		fprintf (rf, "%s\n", pf);
		UD_block_report (&(p->UD), 16, rf, pf);
		fprintf (rf, "\n");
	}

	for (i=0; i<ptr->cnt; i++) {
		r = &(ptr->ridx[i]);

		fprintf (rf, "ridx[%4u] rid=%4.4x, idx=%4.4x\n", i, r->rid, r->idx);
	}
	fprintf (rf, "\n");
}

static void
report_positions (FILE *rf, SPWAW_SNAP_OOB_PRAW *ptr)
{
	DWORD			i;
	SPWAW_SNAP_OOB_PELRAW	*p;
	SPWAW_RINDEX		*r;
	char			pf[16];

	if (!ptr) return;

	smart_title (rf, '-', "RAW unit positions report:\n");

	fprintf (rf, "count: %u\n", ptr->cnt);
	fprintf (rf, "\n");

	if (!ptr->cnt || !ptr->raw || !ptr->ridx) return;
	memset (pf, 0, sizeof (pf));

	for (i=0; i<ptr->cnt; i++) {
		p = &(ptr->raw[i]);

		fprintf (rf, "{%4u} RID : %4.4x\n", i, p->RID);
		fprintf (rf, "{%4u} URID: %4.4x\n", i, p->URID);
		fprintf (rf, "{%4u} x   : %u\n", i, p->x);
		fprintf (rf, "{%4u} y   : %u\n", i, p->y);
		fprintf (rf, "{%4u} seen: %u\n", i, p->seen);

		snprintf (pf, sizeof (pf) - 1, "{%4u} ", i);
		fprintf (rf, "%s\n", pf);
		UD_block_report (&(p->UD), 16, rf, pf);
		fprintf (rf, "\n");
	}

	for (i=0; i<ptr->cnt; i++) {
		r = &(ptr->ridx[i]);

		fprintf (rf, "ridx[%4u] rid=%4.4x, idx=%4.4x\n", i, r->rid, r->idx);
	}
	fprintf (rf, "\n");
}

static void
report_oob (FILE *rf, SPWAW_SNAP_OOB_RAW *ptr, SPWAW_SNAP_OOB_FORCE *iptr, int player)
{
	if (!ptr) return;

	smart_title (rf, '=', "RAW player %u OOB report:\n", player);

	report_formations (rf, &(ptr->formations), &(iptr->formations));
	report_units (rf, &(ptr->units));
	report_leaders (rf, &(ptr->leaders));
	report_positions (rf, &(ptr->positions));

	fprintf (rf, "\n");
}

void
raw_report (SPWAW_SNAPSHOT *ptr, FILE *rf)
{
	if (!ptr || !rf) return;

	report_game	(rf, &(ptr->raw.game));
	report_oob	(rf, &(ptr->raw.OOBp1), &(ptr->OOBp1.battle), 1);
	report_oob	(rf, &(ptr->raw.OOBp2), &(ptr->OOBp2.battle), 2);

	return;
}
