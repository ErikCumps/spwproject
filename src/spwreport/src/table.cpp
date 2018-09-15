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
report_comment (FILE *rf, SPWAW_SNAP_CMT *ptr)
{
	fprintf (rf, "Title: \"%s\"\n", ptr->title);
	fprintf (rf, "\n");
}

static void
report_battle (FILE *rf, SPWAW_SNAP_BATTLE *ptr)
{
	if (!ptr) return;

	smart_title (rf, '-', "Table battle report:\n");

	fprintf (rf, "Battle date      : %s, turn %u of %u\n", ptr->strings.date, ptr->data.turn, ptr->data.turn_max);
	fprintf (rf, "Battle location  : %s, %s terrain\n", ptr->data.location, ptr->strings.terrain);
	fprintf (rf, "Battle conditions: %s, %u meters visibility\n", ptr->strings.weather, HEX2M (ptr->data.visibility));
	fprintf (rf, "Player1          : %s forces, %s mission\n", ptr->strings.people_p1, ptr->strings.miss_p1);
	fprintf (rf, "Player2          : %s forces, %s mission\n", ptr->strings.people_p2, ptr->strings.miss_p2);
	fprintf (rf, "Purchase credit  : %u\n", ptr->data.credit);

	fprintf (rf, "\n");
}

static void
report_campaign (FILE *rf, SPWAW_SNAP_CAMPAIGN *ptr)
{
	if (!ptr) return;

	smart_title (rf, '-', "Table campaign report:\n");

	fprintf (rf, "Campaign flag?       : %u\n", ptr->data.campaign);

	if (ptr->data.campaign) {
		fprintf (rf, "Battles fought       : %u, %d MV, %d TV, %d L\n", ptr->data.battles, ptr->data.majvics, ptr->data.minvics, ptr->data.losses);
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
	}

	fprintf (rf, "\n");
}

static void
report_map (FILE *rf, SPWAW_SNAP_MAP *ptr)
{
	if (!ptr) return;

	smart_title (rf, '-', "Table map report:\n");

	fprintf (rf, "Map size: %.3f x %.3f km\n",
		HEX2K (ptr->width), HEX2K (ptr->height));


	fprintf (rf, "\n");
}

static void
report_game (FILE *rf, SPWAW_SNAP_GAME *ptr)
{
	if (!ptr) return;

	smart_title (rf, '=', "Table game data report:\n");

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
	DWORD			i, j;
	SPWAW_SNAP_OOB_FEL	*p;
	SPWAW_SNAP_OOB_UEL	*q;
	char			stat[4];

	if (!ptr) return;
	if (!ptr->cnt || !ptr->list) return;

	smart_title (rf, '-', "Table formations report:\n");

	fprintf (rf, "%-4s: %-3s %-32s %-16s %-16s %-4s %-5s %-6s %-8s %-3s\n", "#", "CoA", "Status", "Type", "Ldr", "Cmd", "Kills", "Losses", "Ready", "Cnt");
	fprintf (rf, "%-4s: %-3s %-32s %-16s %-16s %-4s %-5s %-6s %-8s %-3s\n", "--", "---", "------", "----", "---", "---", "-----", "------", "-----", "---");

	memset (stat, 0, sizeof (stat));
	for (i=0; i<ptr->cnt; i++) {
		p = &(ptr->list[i]);

		snprintf (stat, sizeof (stat) - 1, "%s", p->strings.status);
		fprintf (rf, "%-4s: %-3s %-32s %-16s %-16s %-4s %5u %6u %6.2f %% %3u\n",
			p->strings.name, stat, p->strings.fstatus, p->strings.type,
			p->data.leader.up->strings.uid, p->data.hcmd.up->strings.uid,
			p->attr.gen.kills, p->attr.gen.losses, p->attr.gen.ready * 100.0, p->data.ucnt);

		for (j=0; j<p->data.ucnt; j++) {
			q = p->data.ulist[j];
			fprintf (rf, "%-4s: %-3s %-32s %-16s %-16s %-4s %5u %6u %6.2f %% %3u\n",
				q->strings.uid, "", q->strings.status, q->data.type, q->data.name,
				(q->data.RID == q->data.formation->data.leader.up->data.RID) ? "*" : "",
				q->attr.gen.kills, q->attr.gen.losses, q->attr.gen.ready * 100.0, q->data.hcnt);
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

	smart_title (rf, '-', "Table units report:\n");

	fprintf (rf, "%s%s%s%s%s%-4s %-16s %-4s %-16s %4s %3s %3s %3s %3s %3s %3s %3s %3s %4s %4s %3s %3s %3s %8s %10s\n",
		"+", "S", "D", "A", "L", "#", "Unit", "Rank", "Leader", "RID", "UTG", "EXP", "MOR", "SUP", "RAL", "INF", "ART", "ARM", "Kill", "Loss", "Men", "KIA", "Dmg", "Ready", "Position");
	fprintf (rf, "%s%s%s%s%s%-4s %-16s %-4s %-16s %4s %3s %3s %3s %3s %3s %3s %3s %3s %4s %4s %3s %3s %3s %8s %10s\n",
		"-", "-", "-", "-", "-", "----", "----", "----", "------", "--", "---", "---", "---", "---", "---", "---", "---", "---", "----", "----", "---", "---", "---", "-----", "----------");

	for (i=0; i<ptr->cnt; i++) {
		SPWAW_SNAP_OOB_UEL *p = &(ptr->list[i]);

		memset (posb, 0, sizeof (posb));
		if (p->data.posx < 0) {
			snprintf (posb, sizeof (posb) - 1, "(  none  )");
		} else {
			snprintf (posb, sizeof (posb) - 1, "(%3d, %3d)", p->data.posx, p->data.posy);
		}

		fprintf (rf, "%s%s%s%s%s%-4s %-16s %-4s %-16s %4u %3u %3u %3u %3u %3u %3u %3u %3u %4u %4u %3u %3u %3u %6.2f %% %10s\n",
			//p->data.lost?"+":" ",
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

	smart_title (rf, '-', "Table crews report:\n");

	fprintf (rf, "%s%s%s%s%s%-4s %-16s %-4s %-16s %4s %3s %3s %3s %3s %3s %3s %3s %3s %4s %3s %3s %3s %8s %10s\n",
		"+", "S", "D", "A", "L", "#", "Unit", "Rank", "Leader", "RID", "UTG", "EXP", "MOR", "SUP", "RAL", "INF", "ART", "ARM", "Kill", "Men", "KIA", "Dmg", "Ready", "Position");
	fprintf (rf, "%s%s%s%s%s%-4s %-16s %-4s %-16s %4s %3s %3s %3s %3s %3s %3s %3s %3s %4s %3s %3s %3s %8s %10s\n",
		"-", "-", "-", "-", "-", "----", "----", "----", "------", "--", "---", "---", "---", "---", "---", "---", "---", "---", "----", "---", "---", "---", "-----", "----------");

	for (i=0; i<ptr->cnt; i++) {
		SPWAW_SNAP_OOB_UEL *p = &(ptr->list[i]);

		memset (posb, 0, sizeof (posb));
		if (p->data.posx < 0) {
			snprintf (posb, sizeof (posb) - 1, "(  none  )");
		} else {
			snprintf (posb, sizeof (posb) - 1, "(%3d, %3d)", p->data.posx, p->data.posy);
		}

		fprintf (rf, "%s%s%s%s%s%-4s %-16s %-4s %-16s %4u %3u %3u %3u %3u %3u %3u %3u %3u %4u %3u %3u %3u %6.2f %% %10s\n",
			//p->data.lost?"+":" ",
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

	smart_title (rf, '=', "Table %s forces order of battle report:\n", ptr->people);

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

	fprintf (rf, "%20s %5s %6s %8s\n", "item", "kills", "losses", "readiness");
	fprintf (rf, "%20s %5s %6s %8s\n", "----", "-----", "------", "---------");
	fprintf (rf, "%20s %5d %6d %6.2f %%\n", "Total", p->attr.gen.kills, p->attr.gen.losses, p->attr.gen.ready * 100.0);
	fprintf (rf, "%20s %5d %6d %6.2f %%\n", "Formations max", p->attr.mmas_fkills.max.v, p->attr.mmas_flosses.max.v, p->attr.mmas_fready.max.v);
	fprintf (rf, "%20s %5d %6d %6.2f %%\n", "Formations min", p->attr.mmas_fkills.min.v, p->attr.mmas_flosses.min.v, p->attr.mmas_fready.min.v);
	fprintf (rf, "%20s %5.2f %6.2f %6.2f %%\n", "Formations avg", p->attr.mmas_fkills.avg, p->attr.mmas_flosses.avg, p->attr.mmas_fready.avg);
	fprintf (rf, "%20s %5d %6s %6.2f %%\n", "Units max", p->attr.mmas_ukills.all.max.v, "-", p->attr.mmas_uready.all.max.v);
	fprintf (rf, "%20s %5d %6s %6.2f %%\n", "Units min", p->attr.mmas_ukills.all.min.v, "-", p->attr.mmas_uready.all.min.v);
	fprintf (rf, "%20s %5.2f %6s %6.2f %%\n", "Units avg", p->attr.mmas_ukills.all.avg, "-", p->attr.mmas_uready.all.avg);
	fprintf (rf, "\n");

	report_formations	(rf, &(p->formations));
	report_units		(rf, &(p->units));
	report_crews		(rf, &(p->crews));

	fprintf (rf, "\n");
}

void
table_report (SPWAW_SNAPSHOT *ptr, FILE *rf, bool core)
{
	if (!ptr || !rf) return;

	fprintf (rf, "Battle type: %s\n", SPWAW_battletype2str (ptr->type));
	fprintf (rf, "\n");

	report_game	(rf, &(ptr->game));		fprintf (rf, "\n\n");
	report_oob	(rf, &(ptr->OOBp1), core);	fprintf (rf, "\n\n");
	report_oob	(rf, &(ptr->OOBp2), core);	fprintf (rf, "\n\n");

	return;
}
