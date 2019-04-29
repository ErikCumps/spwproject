/** \file
 * The Steel Panthers World at War report tool.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL V2
 */

#include "spwreport.h"
#include "utils.h"

static void
report_comment (FILE *rf, SPWAW_SNAP_CMT *ptr)
{
	fprintf (rf, "Title: \"%s\"\n", ptr->title);
	fprintf (rf, "\n");
}

static void
report_battle (FILE *rf, SPWAW_SNAP_BATTLE *ptr)
{
	if (!ptr) return;

	smart_title (rf, '-', "Short battle report:\n");

	fprintf (rf, "Battle date    : %s, turn %u of %u\n", ptr->strings.date, ptr->data.turn, ptr->data.turn_max);
	fprintf (rf, "Battle location: %s\n", ptr->data.location);
	fprintf (rf, "Terrain        : %s terrain\n", ptr->strings.terrain);
	fprintf (rf, "Weather        : %s\n", ptr->strings.weather);
	fprintf (rf, "Visibility     : %u meters\n", HEX2M (ptr->data.visibility));
	fprintf (rf, "Player1        : %s forces\n", ptr->strings.people_p1);
	fprintf (rf, "Player1        : %s mission\n", ptr->strings.miss_p1);
	fprintf (rf, "Player2        : %s forces\n", ptr->strings.people_p2);
	fprintf (rf, "Player2        : %s mission\n", ptr->strings.miss_p2);
	if (ptr->data.OOB_p3) {
		fprintf (rf, "Player3        : %s forces\n", ptr->strings.people_p3);
	}
	if (ptr->data.OOB_p4) {
		fprintf (rf, "Player4        : %s forces\n", ptr->strings.people_p4);
	}
	fprintf (rf, "\n");

	for (int i=0; i<ARRAYCOUNT(ptr->stats.vhex_stats)-1; i++) {
		fprintf (rf, "VHEX: %-9s: %2u (%u points)\n",
			SPWAW_vhstatus2str((SPWAW_VHSTATUS)i), ptr->stats.vhex_stats[i].count, ptr->stats.vhex_stats[i].value);
	}
	fprintf (rf, "\n");
}

static void
report_campaign (FILE *rf, SPWAW_SNAP_CAMPAIGN *ptr)
{
	if (!ptr) return;

	smart_title (rf, '-', "Short campaign report:\n");

	fprintf (rf, "Campaign flag?       : %u\n", ptr->data.campaign);

	if (ptr->data.campaign) {
		fprintf (rf, "Campaign start date  : %s\n", ptr->strings.start);
		fprintf (rf, "Campaign end date    : %s\n", ptr->strings.end);
		fprintf (rf, "Battles fought       : %u of %u\n", ptr->data.battles, ptr->data.battles_max);
		fprintf (rf, "Major victories      : %d\n", ptr->data.majvics);
		fprintf (rf, "Minor victories      : %d\n", ptr->data.minvics);
		fprintf (rf, "Losses               : %d\n", ptr->data.losses);
		fprintf (rf, "\n");

		fprintf (rf, "                       %5.5s %5.5s %5.5s %5.5s %5.5s %5.5s %5.5s\n",
			"MEN", "ART", "SFT", "APC", "AFV", "AIR", "GLD");
		fprintf (rf, "Player1 battle losses: %5u %5u %5u %5u %5u %5u %5u\n",
			ptr->data.P1BL.men, ptr->data.P1BL.art, ptr->data.P1BL.soft, ptr->data.P1BL.apc, ptr->data.P1BL.afv,
			ptr->data.P1BL.air, ptr->data.P1BL.gliders);
		fprintf (rf, "Player1 total losses : %5u %5u %5u %5u %5u %5u %5u\n",
			ptr->data.P1TL.men, ptr->data.P1TL.art, ptr->data.P1TL.soft, ptr->data.P1TL.apc, ptr->data.P1TL.afv,
			ptr->data.P1TL.air, ptr->data.P1TL.gliders);
		fprintf (rf, "Player1 score        : %d\n", ptr->data.P1score);
		fprintf (rf, "Player1 result       : %s\n", ptr->strings.P1result);
		fprintf (rf, "\n");

		fprintf (rf, "                       %5.5s %5.5s %5.5s %5.5s %5.5s %5.5s %5.5s\n",
			"MEN", "ART", "SFT", "APC", "AFV", "AIR", "GLD");
		fprintf (rf, "Player2 battle losses: %5u %5u %5u %5u %5u %5u %5u\n",
			ptr->data.P2BL.men, ptr->data.P2BL.art, ptr->data.P2BL.soft, ptr->data.P2BL.apc, ptr->data.P2BL.afv,
			ptr->data.P2BL.air, ptr->data.P2BL.gliders);
		fprintf (rf, "Player2 total losses : %5u %5u %5u %5u %5u %5u %5u\n",
			ptr->data.P2TL.men, ptr->data.P2TL.art, ptr->data.P2TL.soft, ptr->data.P2TL.apc, ptr->data.P2TL.afv,
			ptr->data.P2TL.air, ptr->data.P2TL.gliders);
		fprintf (rf, "Player2 score        : %d\n", ptr->data.P2score);
		fprintf (rf, "Player2 result       : %s\n", ptr->strings.P2result);
	}

	fprintf (rf, "\n");
}

static void
report_map (FILE *rf, SPWAW_SNAP_MAP *ptr)
{
	if (!ptr) return;

	smart_title (rf, '-', "Short map report:\n");

	fprintf (rf, "Map size: %.3f x %.3f km\n",
		HEX2K (ptr->width), HEX2K (ptr->height));


	fprintf (rf, "\n");
}

static void
report_game (FILE *rf, SPWAW_SNAP_GAME *ptr)
{
	if (!ptr) return;

	smart_title (rf, '=', "Short game data report:\n");

	report_comment	(rf, &(ptr->cmt));
	report_battle	(rf, &(ptr->battle));
	report_campaign	(rf, &(ptr->campaign));
	report_map	(rf, &(ptr->map));

	fprintf (rf, "Battle turn status: %s\n", ptr->battle.strings.status);
	switch (ptr->battle.data.status) {
		case SPWAW_BTDEPLOY:
			fprintf (rf, "Snapshot contains updated campaign info.\n");
			break;
		case SPWAW_BTSCORE:
			fprintf (rf, "Snapshot contains updated battle statistics.\n");
			break;
		default:
			break;
	}
}

static void
report_formations (FILE *rf, SPWAW_SNAP_OOB_F *ptr)
{
	DWORD	i, j;

	if (!ptr) return;
	if (!ptr->cnt || !ptr->list) return;

	smart_title (rf, '-', "Short formations report:\n");

	for (i=0; i<ptr->cnt; i++) {
		SPWAW_SNAP_OOB_FEL *p = &(ptr->list[i]);

		fprintf (rf, "%-2s: %s %s %s\n", p->strings.name, p->strings.status, p->strings.fstatus, p->strings.type);
		fprintf (rf, "\tleader    : %s %s (%s %s)\n",
			p->data.leader.up->strings.uid, p->data.leader.up->data.type,
			p->data.leader.up->strings.rank, p->data.leader.up->data.name);
		fprintf (rf, "\thigher cmd: %s %s (%s %s)\n",
			p->data.hcmd.up->strings.uid, p->data.hcmd.up->data.type,
			p->data.hcmd.up->strings.rank, p->data.hcmd.up->data.name);

		fprintf (rf, "\tkills     : %u\n", p->attr.gen.kills);
		fprintf (rf, "\tlosses    : %u\n", p->attr.gen.losses);
		fprintf (rf, "\treadiness : %6.2f %%\n", p->attr.gen.ready * 100.0);
		fprintf (rf, "\tunit count: %u\n", p->data.ucnt);
		fprintf (rf, "\tunit list :\n");
		for (j=0; j<p->data.ucnt; j++) {
			if (p->data.ulist[j]->data.aband == SPWAW_ANONE) {
				fprintf (rf, "\t\t%s %s\n", p->data.ulist[j]->strings.uid, p->data.ulist[j]->data.type);
			} else {
				if (p->data.ulist[j]->data.aunit.up) {
					fprintf (rf, "\t\t%s %s abandoned by %s %s\n",
						p->data.ulist[j]->strings.uid, p->data.ulist[j]->data.type,
						p->data.ulist[j]->data.aunit.up->strings.uid, p->data.ulist[j]->data.aunit.up->data.type);
				} else {
					fprintf (rf, "\t\t%s %s abandoned by lost crew\n",
						p->data.ulist[j]->strings.uid, p->data.ulist[j]->data.type);
				}
			}
		}

		fprintf (rf, "\n");
	}

	fprintf (rf, "\n");
}

static void
report_units (FILE *rf, SPWAW_SNAP_OOB_U *ptr)
{
	DWORD	i;
	char	posb[11];

	if (!ptr) return;
	if (!ptr->cnt || !ptr->list) return;

	smart_title (rf, '-', "Short units report:\n");

	for (i=0; i<ptr->cnt; i++) {
		SPWAW_SNAP_OOB_UEL *p = &(ptr->list[i]);

		memset (posb, 0, sizeof (posb));
		if (p->data.posx < 0) {
			snprintf (posb, sizeof (posb) - 1, "(  none  )");
		} else {
			snprintf (posb, sizeof (posb) - 1, "(%3d, %3d)", p->data.posx, p->data.posy);
		}

		fprintf (rf, "%s%s%s%s%s%-4s: %-16s (%-3s %-16s) RID %4.4u UTG %3.3u EXP %3.3u MOR %3.3u SUP %3.3u RAL %3.3u INF %3.3u ART %3.3u ARM %3.3u KILL %3.3u LOSS %3.3u MEN %3.3u KIA %3.3u DMG %3.3u RDY %6.2f POS %10s\n",
			p->data.alive?" ":"+",
			p->data.spotted?" ":"#",
			p->data.damage?"-":" ",
			p->data.aband?"A":" ",
			p->data.loaded?"L":" ",
			p->strings.uid, p->data.type, p->strings.rank, p->data.name,
			p->data.RID, p->data.UTGidx,
			p->data.exp, p->data.mor, p->data.sup, p->data.ral, p->data.inf, p->data.art, p->data.arm,
			p->attr.gen.kills, p->attr.gen.losses, p->data.hcnt, p->data.hcnt - p->data.hcnt_left, p->data.damage,
			p->attr.gen.ready * 100.0,
			posb);
	}

	fprintf (rf, "\n");
}

static void
report_crews (FILE *rf, SPWAW_SNAP_OOB_U *ptr)
{
	DWORD	i;
	char	posb[11];

	if (!ptr) return;
	if (!ptr->cnt || !ptr->list) return;

	smart_title (rf, '-', "Short crews report:\n");

	for (i=0; i<ptr->cnt; i++) {
		SPWAW_SNAP_OOB_UEL *p = &(ptr->list[i]);

		memset (posb, 0, sizeof (posb));
		if (p->data.posx < 0) {
			snprintf (posb, sizeof (posb) - 1, "(  none  )");
		} else {
			snprintf (posb, sizeof (posb) - 1, "(%3d, %3d)", p->data.posx, p->data.posy);
		}

		fprintf (rf, "%s%s%s%s%s%-4s: %-16s (%-3s %-16s) RID %4.4u UTG %3.3u EXP %3.3u MOR %3.3u SUP %3.3u RAL %3.3u INF %3.3u ART %3.3u ARM %3.3u KILL %3.3u MEN %3.3u KIA %3.3u DMG %3.3u RDY %6.2f POS %10s\n",
			p->data.alive?" ":"+",
			p->data.spotted?" ":"#",
			p->data.damage?"-":" ",
			p->data.aband?"A":" ",
			p->data.loaded?"L":" ",
			p->strings.uid, p->data.type, p->strings.rank, p->data.name,
			p->data.RID, p->data.UTGidx,
			p->data.exp, p->data.mor, p->data.sup, p->data.ral, p->data.inf, p->data.art, p->data.arm,
			p->attr.gen.kills, p->data.hcnt, p->data.hcnt - p->data.hcnt_left, p->data.damage,
			p->attr.gen.ready * 100.0,
			posb);
	}

	fprintf (rf, "\n");
}

static void
report_oob (FILE *rf, SPWAW_SNAP_OOB *ptr, bool core)
{
	SPWAW_SNAP_OOB_FORCE	*p;

	if (!ptr) return;

	smart_title (rf, '=', "Short %s forces order of battle report:\n", ptr->people);

	p = core ? &(ptr->core) : &(ptr->battle);

	if (!p->formations.cnt) {
		fprintf (rf, "No forces present in OOB.\n\n");
		return;
	}

	fprintf (rf, "Leader    : %s %s (%s %s)\n",
		p->leader->strings.uid, p->leader->data.type,
		p->leader->strings.rank, p->leader->data.name);

	fprintf (rf, "Formations: %u\n", p->formations.cnt);
	fprintf (rf, "Units     : %u\n", p->units.cnt);
	fprintf (rf, "Crews     : %u\n", p->crews.cnt);
	fprintf (rf, "\n");

	fprintf (rf, "Total kills        : %u\n", p->attr.gen.kills);
	fprintf (rf, "Total losses       : %u\n", p->attr.gen.losses);
	fprintf (rf, "Total readiness    : %6.2f %%\n", p->attr.gen.ready * 100.0);
	report_mmas (rf, "Unit experience    ", &(p->attr.mmas_uexp.all), p, true);
	report_mmas (rf, "Unit readiness     ", &(p->attr.mmas_uready.all), p, true);
	report_mmas (rf, "Unit kills         ", &(p->attr.mmas_ukills.all), p, true);
	report_mmas (rf, "Formation readiness", &(p->attr.mmas_fready), p, false);
	report_mmas (rf, "Formation kills    ", &(p->attr.mmas_fkills), p, false);
	report_mmas (rf, "Formation losses   ", &(p->attr.mmas_flosses), p, false);
	fprintf (rf, "\n");

	report_formations	(rf, &(p->formations));
	report_units		(rf, &(p->units));
	report_crews		(rf, &(p->crews));

	fprintf (rf, "\n");
}

void
short_report (SPWAW_SNAPSHOT *ptr, FILE *rf, bool core)
{
	if (!ptr || !rf) return;

	fprintf (rf, "Game type: %s\n", SPWAW_gametype2str (ptr->gametype));
	fprintf (rf, "Battle type: %s\n", SPWAW_battletype2str (ptr->type));
	fprintf (rf, "\n");

	report_game	(rf, &(ptr->game));		fprintf (rf, "\n\n");
	report_oob	(rf, &(ptr->OOBp1), core);	fprintf (rf, "\n\n");
	report_oob	(rf, &(ptr->OOBp2), core);	fprintf (rf, "\n\n");

	return;
}
