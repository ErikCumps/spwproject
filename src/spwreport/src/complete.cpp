/** \file
 * The Steel Panthers World at War report tool.
 *
 * Copyright (C) 2007-2018 Erik Cumps <erik.cumps@gmail.com>
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

	smart_title (rf, '-', "Complete battle report:\n");

	fprintf (rf, "Date           : %s\n", ptr->strings.date);
	fprintf (rf, "Turn           : %u of %u\n", ptr->data.turn, ptr->data.turn_max);
	fprintf (rf, "Status         : %u (%s)\n", ptr->data.status, ptr->strings.status);
	fprintf (rf, "Location       : %s\n", ptr->data.location);
	fprintf (rf, "Terrain        : %u (%s terrain)\n", ptr->data.terrain, ptr->strings.terrain);
	fprintf (rf, "Weather        : %u (%s)\n", ptr->data.weather, ptr->strings.weather);
	fprintf (rf, "Visibility     : %u (%u meters)\n", ptr->data.visibility, HEX2M (ptr->data.visibility));
	fprintf (rf, "Player1 OOB    : %u (%s forces)\n", ptr->data.OOB_p1, ptr->strings.people_p1);
	fprintf (rf, "Player1 mission: %u (%s mission)\n", ptr->data.miss_p1, ptr->strings.miss_p1);
	fprintf (rf, "Player2 OOB    : %u (%s forces)\n", ptr->data.OOB_p2, ptr->strings.people_p2);
	fprintf (rf, "Player2 mission: %u (%s mission)\n", ptr->data.miss_p2, ptr->strings.miss_p2);
	if (ptr->data.OOB_p3) {
		fprintf (rf, "Player3 OOB    : %u (%s forces)\n", ptr->data.OOB_p3, ptr->strings.people_p3);
	}
	if (ptr->data.OOB_p4) {
		fprintf (rf, "Player4 OOB    : %u (%s forces)\n", ptr->data.OOB_p4, ptr->strings.people_p4);
	}

	fprintf (rf, "\n");
}

static void
report_campaign (FILE *rf, SPWAW_SNAP_CAMPAIGN *ptr)
{
	if (!ptr) return;

	smart_title (rf, '-', "Complete campaign report:\n");

	fprintf (rf, "Campaign flag?       : %u\n", ptr->data.campaign);

	if (ptr->data.campaign) {
		fprintf (rf, "Start date           : %s\n", ptr->strings.start);
		fprintf (rf, "End date             : %s\n", ptr->strings.end);
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

	smart_title (rf, '-', "Complete map report:\n");

	fprintf (rf, "Map size: %u x %u\n", ptr->width, ptr->height);
	fprintf (rf, "        : %.3f x %.3f km\n", HEX2K (ptr->width), HEX2K (ptr->height));

	fprintf (rf, "\n");
}

static void
report_game (FILE *rf, SPWAW_SNAP_GAME *ptr)
{
	if (!ptr) return;

	smart_title (rf, '=', "Complete game data report:\n");

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
report_formations (FILE *rf, SPWAW_SNAP_OOB_FORCE *f)
{
	SPWAW_SNAP_OOB_F	*ptr;
	DWORD			i, j;
	SPWAW_SNAP_OOB_FEL	*fp;
	SPWAW_SNAP_OOB_UEL	*up, *ap;

	if (!f) return; ptr = &(f->formations);
	if (!ptr->cnt || !ptr->list) return;

	smart_title (rf, '-', "Complete formations report:\n");

	fprintf (rf, "Formation count: %u\n", ptr->cnt);
	fprintf (rf, "\n");

	for (i=0; i<ptr->cnt; i++) {
		fp = &(ptr->list[i]);

		fprintf (rf, "#%5.5u %s (FID %5.5u)\n", fp->data.RID, fp->strings.name, fp->data.FID);
		fprintf (rf, "\tleader      : %s %s (%s %s)\n",
			fp->data.leader.up->strings.uid, fp->data.leader.up->data.type,
			fp->data.leader.up->strings.rank, fp->data.leader.up->data.name);
		fprintf (rf, "\thigher cmd  : %s %s (%s %s)\n",
			fp->data.hcmd.up->strings.uid, fp->data.hcmd.up->data.type,
			fp->data.hcmd.up->strings.rank, fp->data.hcmd.up->data.name);

		fprintf (rf, "\tstatus      : %u (%s)\n", fp->data.status, fp->strings.status);
		fprintf (rf, "\tforce status: %u (%s)\n", fp->data.fstatus, fp->strings.fstatus);
		fprintf (rf, "\ttype        : %s\n", fp->strings.type);
		fprintf (rf, "\tunit type   : %s\n", fp->strings.utype);
		fprintf (rf, "\tunit count  : %u\n", fp->data.ucnt);
		fprintf (rf, "\tunit list   :\n");
		for (j=0; j<fp->data.ucnt; j++) {
			up = fp->data.ulist[j];
			if (up->data.aband == SPWAW_ANONE) {
				fprintf (rf, "\t\t%s %s (%s %s)\n",
					up->strings.uid, up->data.type,
					up->strings.rank, up->data.name);
			} else {
				ap = up->data.aunit.up;

				if (ap) {
					fprintf (rf, "\t\t%s %s (%s %s) abandoned by %s %s (%s %s)\n",
						up->strings.uid, up->data.type,
						up->strings.rank, up->data.name,
						ap->strings.uid, ap->data.type,
						ap->strings.rank, ap->data.name);
				} else {
					fprintf (rf, "\t\t%s %s (%s %s) abandoned by lost crew\n",
						up->strings.uid, up->data.type,
						up->strings.rank, up->data.name);
				}
			}

		}
		fprintf (rf, "\n");

		fprintf (rf, "\tkills      : %u\n", fp->attr.gen.kills);
		fprintf (rf, "\tlosses     : %u\n", fp->attr.gen.losses);
		fprintf (rf, "\treadiness  : %6.2f %%\n", fp->attr.gen.ready * 100.0);
		report_mmas (rf, "\tunit experience", &(fp->attr.mmas_exp), f, true);
		report_mmas (rf, "\tunit readiness ", &(fp->attr.mmas_ready), f, true);
		report_mmas (rf, "\tunit kills     ", &(fp->attr.mmas_kills), f, true);

		fprintf (rf, "\n");
	}

	fprintf (rf, "\n");
}

static void
report_units (FILE *rf, SPWAW_SNAP_OOB_FORCE *f)
{
	SPWAW_SNAP_OOB_U	*ptr;
	DWORD			i;
	SPWAW_SNAP_OOB_UEL	*p;

	if (!f) return; ptr = &(f->units);
	if (!ptr->cnt || !ptr->list) return;

	smart_title (rf, '-', "Complete units report:\n");

	fprintf (rf, "Unit count: %u\n", ptr->cnt);
	fprintf (rf, "\n");

	for (i=0; i<ptr->cnt; i++) {
		SPWAW_SNAP_OOB_FEL	*fp;

		p = &(ptr->list[i]);

		fprintf (rf, "#%5.5u (%5.5u:%5.5u,%3.3u) %-4s %s\n",
			p->data.RID, p->data.FRID, p->data.FMID, p->data.FSID, p->strings.uid, p->data.type);

		fprintf (rf, "\tleader      : %s, rank %u (%s)\n", p->data.name, p->data.rank, p->strings.rank);
		// Prevent warning C4244
		if ((fp = p->data.formation) == NULL) { fp = p->data.aunit.up->data.formation; }
		fprintf (rf, "\tformation   : %s: %s %s %s\n",
			fp->strings.name, fp->strings.status, fp->strings.fstatus, fp->strings.type);
		fprintf (rf, "\tOOB         : %u (%10.10s)\n", p->data.OOB, p->strings.people);
		fprintf (rf, "\tunit type   : %u (%s)\n", p->data.utype, p->strings.utype);
		fprintf (rf, "\tunit class  : %u (%s)\n", p->data.uclass, p->strings.uclass);
		fprintf (rf, "\tkills       : %u\n", p->data.kills);
		//fprintf (rf, "\tstatus      : %u (%s) [%d:%s] [%d:%s] [%d:%s]\n", p->data.status, p->strings.status,
		//	p->data.alive, p->data.alive ? "alive" : "DEAD",
		//	p->data.lost, p->data.lost ? "LOST" : "available",
		//	p->data.spotted, p->data.spotted ? "SPOTTED" : "hidden");
		fprintf (rf, "\tstatus      : %u (%s) [%d:%s] [%d:%s]\n", p->data.status, p->strings.status,
			p->data.alive, p->data.alive ? "alive" : "DEAD",
			p->data.spotted, p->data.spotted ? "SPOTTED" : "hidden");
		fprintf (rf, "\tentrenchment: %u (%s)\n", p->data.entr, p->strings.entr);
		fprintf (rf, "\thead count  : %u (%u left)\n", p->data.hcnt, p->data.hcnt_left);
		fprintf (rf, "\tdamage      : %u\n", p->data.damage);
		fprintf (rf, "\tabandoned   : %u\n", p->data.aband);
		switch (p->data.aband) {
			case SPWAW_ASTAY:
				if (p->data.aunit.up) {
					fprintf (rf, "\tabandoned   : unit left by crew %s %s (%s %s)\n",
						p->data.aunit.up->strings.uid, p->data.aunit.up->data.type,
						p->data.aunit.up->strings.rank, p->data.aunit.up->data.name);
				} else {
					fprintf (rf, "\tabandoned   : unit left by crew\n");

				}
				break;
			case SPWAW_ALEFT:
				fprintf (rf, "\tabandoned   : crew left unit %s %s (%s %s)\n",
					p->data.aunit.up->strings.uid, p->data.aunit.up->data.type,
					p->data.aunit.up->strings.rank, p->data.aunit.up->data.name);
				break;
			default:
				break;
		}
		fprintf (rf, "\tloaded      : %u\n", p->data.loaded);
		if (p->data.loaded) {
			fprintf (rf, "\tloaded      : on or by %s %s (%s %s)\n",
						p->data.loader.up->strings.uid, p->data.loader.up->data.type,
						p->data.loader.up->strings.rank, p->data.loader.up->data.name);
		}
		fprintf (rf, "\texperience  : %u, class %u (%s)\n", p->data.exp, p->data.eclass, p->strings.exp);
		fprintf (rf, "\tmorale      : %u\n", p->data.mor);
		fprintf (rf, "\tsuppression : %u\n", p->data.sup);
		fprintf (rf, "\trally       : %u\n", p->data.ral);
		fprintf (rf, "\tinf skills  : %u\n", p->data.inf);
		fprintf (rf, "\tart skills  : %u\n", p->data.art);
		fprintf (rf, "\tarm skills  : %u\n", p->data.arm);
		fprintf (rf, "\tspeed       : %u (%.3f km/h)\n", p->data.speed, HEX2KPH (p->data.speed));
		fprintf (rf, "\tsmokedevice : %u\n", p->data.smkdev);
		fprintf (rf, "\tcontact     : %u (%s)\n", p->data.contact, p->strings.contact);
		fprintf (rf, "\trate of fire: %u\n", p->data.rof);
		fprintf (rf, "\ttargetting  : %u\n", p->data.tgt);
		fprintf (rf, "\trange finder: %u\n", p->data.rf);
		fprintf (rf, "\tfire control: %u\n", p->data.fc);
		fprintf (rf, "\tinfrared    : %u\n", p->data.iv);
		fprintf (rf, "\tswim        : %u\n", p->data.swim);
		fprintf (rf, "\ttype group  : %u\n", p->data.UTGidx);
		fprintf (rf, "\tposition    : (%d, %d)\n", p->data.posx, p->data.posy);
		fprintf (rf, "\n");

		fprintf (rf, "\tkills       : %u\n", p->attr.gen.kills);
		fprintf (rf, "\tlosses      : %u\n", p->attr.gen.losses);
		fprintf (rf, "\treadiness   : %6.2f %%\n", p->attr.gen.ready * 100.0);
		fprintf (rf, "\n");
	}

	fprintf (rf, "\n");
}

static void
report_crews (FILE *rf, SPWAW_SNAP_OOB_FORCE *f)
{
	SPWAW_SNAP_OOB_U	*ptr;
	DWORD			i;
	SPWAW_SNAP_OOB_UEL	*p;

	if (!f) return; ptr = &(f->crews);
	if (!ptr->cnt || !ptr->list) return;

	smart_title (rf, '-', "Complete crews report:\n");

	fprintf (rf, "Unit count: %u\n", ptr->cnt);
	fprintf (rf, "\n");

	for (i=0; i<ptr->cnt; i++) {
		SPWAW_SNAP_OOB_FEL	*fp;

		p = &(ptr->list[i]);

		fprintf (rf, "#%5.5u (%5.5u:%5.5u,%3.3u) %-4s %s\n",
			p->data.RID, p->data.FRID, p->data.FMID, p->data.FSID, p->strings.uid, p->data.type);

		fprintf (rf, "\tleader      : %s, rank %u (%s)\n", p->data.name, p->data.rank, p->strings.rank);
		fp = p->data.aunit.up->data.formation;
		fprintf (rf, "\tformation   : %s: %s %s %s\n",
			fp->strings.name, fp->strings.status, fp->strings.fstatus, fp->strings.type);
		fprintf (rf, "\tOOB         : %u (%10.10s)\n", p->data.OOB, p->strings.people);
		fprintf (rf, "\tunit type   : %u (%s)\n", p->data.utype, p->strings.utype);
		fprintf (rf, "\tunit class  : %u (%s)\n", p->data.uclass, p->strings.uclass);
		fprintf (rf, "\tkills       : %u\n", p->data.kills);
		//fprintf (rf, "\tstatus      : %u (%s) [%d:%s] [%d:%s] [%d:%s]\n", p->data.status, p->strings.status,
		//	p->data.alive, p->data.alive ? "alive" : "DEAD",
		//	p->data.lost, p->data.lost ? "LOST" : "available",
		//	p->data.spotted, p->data.spotted ? "SPOTTED" : "hidden");
		fprintf (rf, "\tstatus      : %u (%s) [%d:%s] [%d:%s]\n", p->data.status, p->strings.status,
			p->data.alive, p->data.alive ? "alive" : "DEAD",
			p->data.spotted, p->data.spotted ? "SPOTTED" : "hidden");
		fprintf (rf, "\thead count  : %u (%u left)\n", p->data.hcnt, p->data.hcnt_left);
		fprintf (rf, "\tdamage      : %u\n", p->data.damage);
		fprintf (rf, "\tabandoned   : %u\n", p->data.aband);
		switch (p->data.aband) {
			case SPWAW_ASTAY:
				fprintf (rf, "\tabandoned   : unit left by crew %s %s (%s %s)\n",
					p->data.aunit.up->strings.uid, p->data.aunit.up->data.type,
					p->data.aunit.up->strings.rank, p->data.aunit.up->data.name);
				break;
			case SPWAW_ALEFT:
				fprintf (rf, "\tabandoned   : crew left unit %s %s (%s %s)\n",
					p->data.aunit.up->strings.uid, p->data.aunit.up->data.type,
					p->data.aunit.up->strings.rank, p->data.aunit.up->data.name);
				break;
			default:
				break;
		}
		fprintf (rf, "\tloaded      : %u\n", p->data.loaded);
		if (p->data.loaded) {
			fprintf (rf, "\tloaded      : on or by %s %s (%s %s)\n",
						p->data.loader.up->strings.uid, p->data.loader.up->data.type,
						p->data.loader.up->strings.rank, p->data.loader.up->data.name);
		}
		fprintf (rf, "\texperience  : %u, class %u (%s)\n", p->data.exp, p->data.eclass, p->strings.exp);
		fprintf (rf, "\tmorale      : %u\n", p->data.mor);
		fprintf (rf, "\tsuppression : %u\n", p->data.sup);
		fprintf (rf, "\trally       : %u\n", p->data.ral);
		fprintf (rf, "\tinf skills  : %u\n", p->data.inf);
		fprintf (rf, "\tart skills  : %u\n", p->data.art);
		fprintf (rf, "\tarm skills  : %u\n", p->data.arm);
		fprintf (rf, "\tspeed       : %u (%.3f km/h)\n", p->data.speed, HEX2KPH (p->data.speed));
		fprintf (rf, "\tsmokedevice : %u\n", p->data.smkdev);
		fprintf (rf, "\tcontact      : %u\n", p->data.contact);
		fprintf (rf, "\trate of fire: %u\n", p->data.rof);
		fprintf (rf, "\ttargetting  : %u\n", p->data.tgt);
		fprintf (rf, "\trange finder: %u\n", p->data.rf);
		fprintf (rf, "\tfire control: %u\n", p->data.fc);
		fprintf (rf, "\tinfrared    : %u\n", p->data.iv);
		fprintf (rf, "\tswim        : %u\n", p->data.swim);
		fprintf (rf, "\ttype group  : %u\n", p->data.UTGidx);
		fprintf (rf, "\n");

		/* No attributes for crewmen! */
		//fprintf (rf, "\tkills       : %u\n", p->attr.gen.kills);
		//fprintf (rf, "\tlost        : %u\n", p->attr.gen.losses);
		//fprintf (rf, "\treadiness   : %6.2f %%\n", p->attr.gen.ready * 100.0);
		//fprintf (rf, "\n");
	}

	fprintf (rf, "\n");
}

static void
report_attrs (FILE *rf, SPWAW_SNAP_OOB_FORCE *f)
{
	SPWAW_SNAP_OOB_GATTR	*p;

	if (!f) return;
	p = &(f->attr);

	fprintf (rf, "Total kills        : %u\n", p->gen.kills);
	fprintf (rf, "Total losses       : %u\n", p->gen.losses);
	fprintf (rf, "Total readiness    : %6.2f %%\n", p->gen.ready * 100.0);
	report_mmas (rf, "Unit experience    ", &(p->mmas_uexp.all), f, true);
	report_mmas (rf, "Unit readiness     ", &(p->mmas_uready.all), f, true);
	report_mmas (rf, "Unit kills         ", &(p->mmas_ukills.all), f, true);
	report_mmas (rf, "Formation readiness", &(p->mmas_fready), f, false);
	report_mmas (rf, "Formation kills    ", &(p->mmas_fkills), f, false);
	report_mmas (rf, "Formation losses   ", &(p->mmas_flosses), f, false);

	fprintf (rf, "\n");
}

static void
report_unit_stats (FILE *rf, SPWAW_SNAP_OOB_FORCE *f, char *fmt)
{
	SPWAW_SNAP_OOB_USTATS	*p;

	if (!f) return;
	p = &(f->stats.ustats);

	smart_title (rf, '-', fmt);

	fprintf (rf, "Unit count: %u\n", p->cnt);
	fprintf (rf, "Units left: %u\n", p->cnt_left);

	fprintf (rf, "\n");
}

static void
report_formation_stats (FILE *rf, SPWAW_SNAP_OOB_FORCE *f, char *fmt)
{
	SPWAW_SNAP_OOB_FSTATS	*p;

	if (!f) return;
	p = &(f->stats.fstats);

	smart_title (rf, '-', fmt);

	fprintf (rf, "Formation count: %u\n", p->cnt);
	fprintf (rf, "Formations left: %u\n", p->cnt_left);

	fprintf (rf, "\n");
}

static void
report_stats (FILE *rf, SPWAW_SNAP_OOB_FORCE *f)
{
	if (!f) return;

	report_unit_stats	(rf, f, "Complete unit statistics report:\n");
	report_formation_stats	(rf, f, "Complete formation statistics report:\n");

	fprintf (rf, "\n");
}

static void
report_oob (FILE *rf, SPWAW_SNAP_OOB *ptr, bool core)
{
	SPWAW_SNAP_OOB_FORCE	*p;

	if (!ptr) return;

	smart_title (rf, '=', "Complete player %u OOB report:\n", (ptr->side == 0) ? 1 : 2);

	fprintf (rf, "Side  : %u\n", ptr->side);
	fprintf (rf, "OOB   : %u\n", ptr->OOB);
	fprintf (rf, "Nation: %s\n", ptr->nation);
	fprintf (rf, "People: %s\n", ptr->people);
	fprintf (rf, "\n");

	p = core ? &(ptr->core) : &(ptr->battle);

	if (!p->formations.cnt) {
		fprintf (rf, "No forces present in OOB.\n\n");
		return;
	}

	fprintf (rf, "Formations: %u\n", p->formations.cnt);
	fprintf (rf, "Units     : %u\n", p->units.cnt);
	fprintf (rf, "Crews     : %u\n", p->crews.cnt);
	fprintf (rf, "Leader    : %s %s (%s %s)\n", p->leader->strings.uid, p->leader->data.type, p->leader->strings.rank, p->leader->data.name);
	fprintf (rf, "\n");

	report_attrs		(rf, p);
	report_formations	(rf, p);
	report_units		(rf, p);
	report_crews		(rf, p);
	//report_attrs		(rf, p);
	report_stats		(rf, p);

	fprintf (rf, "\n");
}

void
complete_report (SPWAW_SNAPSHOT *ptr, FILE *rf, bool core)
{

	if (!ptr || !rf) return;

	fprintf (rf, "Battle type: %s\n", SPWAW_battletype2str (ptr->type));
	fprintf (rf, "\n");

	report_game	(rf, &(ptr->game));		fprintf (rf, "\n\n");
	report_oob	(rf, &(ptr->OOBp1), core);	fprintf (rf, "\n\n");
	report_oob	(rf, &(ptr->OOBp2), core);	fprintf (rf, "\n\n");

	return;
}
