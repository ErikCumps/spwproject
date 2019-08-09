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
report_game (FILE *rf, SPWAW_SNAP_GAME *ptr)
{
	if (!ptr) return;

	smart_title (rf, '-', "Status report for \"%s\"\n", ptr->cmt.title);

	fprintf (rf, "Battle at %s, %s %s against %s %s.\n",
		ptr->battle.data.location,
		ptr->battle.strings.people_p1, ptr->battle.strings.miss_p1,
		ptr->battle.strings.people_p2, ptr->battle.strings.miss_p2);
	fprintf (rf, "\n");

	fprintf (rf, "%s, turn %u of %u\n",
		ptr->battle.strings.date, ptr->battle.data.tdate.turn, ptr->battle.data.turn_max);
	fprintf (rf, "%.3f x %.3f km %s terrain, %s with %u meters visibility.\n",
		HEX2K (ptr->map.width), HEX2K (ptr->map.height),
		ptr->battle.strings.terrain, ptr->battle.strings.weather, ptr->battle.data.visibility * HEXSIZE);
	fprintf (rf, "\n");

	if (ptr->battle.data.OOB_p3 || ptr->battle.data.OOB_p4) {
		if (ptr->battle.data.OOB_p3 && ptr->battle.data.OOB_p4) {
			fprintf (rf, "%s and %s forces may also be present on the battlefield.\n",
				ptr->battle.strings.people_p3, ptr->battle.strings.people_p4);
		} else {
			char *p = ptr->battle.data.OOB_p3 ? ptr->battle.strings.people_p3 : ptr->battle.strings.people_p4;

			fprintf (rf, "%s forces may also be present on the battlefield.\n", p);
		}
		fprintf (rf, "\n");
	}

	fprintf (rf, "%s forces occupy %u victory hexes (worth %u points).\n",
		ptr->battle.strings.people_p1, ptr->battle.stats.vhex_stats[SPWAW_VHP1].count, ptr->battle.stats.vhex_stats[SPWAW_VHP1].value); 
	fprintf (rf, "%s forces occupy %u victory hexes (worth %u points).\n",
		ptr->battle.strings.people_p2, ptr->battle.stats.vhex_stats[SPWAW_VHP2].count, ptr->battle.stats.vhex_stats[SPWAW_VHP2].value); 
	fprintf (rf, "%u remaining victory hexes (worth %u points) are still neutral.\n",
		ptr->battle.stats.vhex_stats[SPWAW_VHN].count, ptr->battle.stats.vhex_stats[SPWAW_VHN].value); 
}

static void
report_oob (FILE *rf, SPWAW_SNAP_OOB *ptr, bool core)
{
	SPWAW_SNAP_OOB_FORCE	*p;
	DWORD			i, j;
	int			omor, emor;
	int			hc, hl;

	if (!ptr) return;

	p = core ? &(ptr->core) : &(ptr->battle);

	if (!p->formations.cnt) {
		fprintf (rf, "No %s forces present in OOB.\n\n", ptr->people);
		return;
	}

	smart_title (rf, '-', "%s forces lead by %s %s.\n", ptr->people, p->leader->strings.rank, p->leader->data.lname);

	fprintf (rf, "Force achieved %u kills with %u losses.\n", p->attr.gen.kills, p->attr.gen.losses);

	fprintf (rf, "Overall force readiness is %3.0f %%.\n", p->attr.gen.ready * 100.0);
	fprintf (rf, "\n");

	fprintf (rf, "Force contains %u formations, %u units and %u crews.\n", p->formations.cnt, p->units.cnt, p->crews.cnt);
	fprintf (rf, "\n");

	for (i=0; i<p->formations.cnt; i++) {

		fprintf (rf, "Formation %s: %s %s %s (%s).\n",
			p->formations.list[i].strings.name, p->formations.list[i].strings.status,
			p->formations.list[i].strings.fstatus, p->formations.list[i].strings.otype,
			p->formations.list[i].strings.type);

		fprintf (rf, "It is lead by %s %s.\n",
			p->formations.list[i].data.leader.up->strings.uid, p->formations.list[i].data.leader.up->data.dname);
		fprintf (rf, "Its higher command is %s %s.\n",
			p->formations.list[i].data.hcmd.up->strings.uid, p->formations.list[i].data.hcmd.up->data.dname);
		fprintf (rf, "It has killed %u enemy units.\n", p->formations.list[i].attr.gen.kills);
		//fprintf (rf, "It has %u recorded kills.\n", p->formations.list[i].attr.gen.kills);
		fprintf (rf, "It has lost %u units.\n", p->formations.list[i].attr.gen.losses);
		//fprintf (rf, "It has %u losses.\n", p->formations.list[i].attr.gen.losses);
		fprintf (rf, "It is %3.0f %% ready.\n", p->formations.list[i].attr.gen.ready * 100.0);
		fprintf (rf, "This formation contains %u units:\n", p->formations.list[i].data.ucnt);
		for (j=0; j<p->formations.list[i].data.ucnt; j++) {
			fprintf (rf, "\tUnit %s: %s (%s/%s)\n",
			p->formations.list[i].data.ulist[j]->strings.uid,
			p->formations.list[i].data.ulist[j]->data.dname,
			p->formations.list[i].data.ulist[j]->strings.utype,
			p->formations.list[i].data.ulist[j]->strings.uclass);

			fprintf (rf, "\t\tIt is lead by %s %s.\n",
				p->formations.list[i].data.ulist[j]->strings.rank, p->formations.list[i].data.ulist[j]->data.lname);
			fprintf (rf, "\t\tIt is an %s unit, and it is %s and %s.\n",
				p->formations.list[i].data.ulist[j]->strings.exp,
				p->formations.list[i].data.ulist[j]->data.spotted ? "spotted" : "hidden",
				//p->formations.list[i].data.ulist[j]->data.lost ? "lost" : "available");
				p->formations.list[i].data.ulist[j]->data.alive ? "available" : "lost");
			fprintf (rf, "\t\tIt has killed %u enemy units.\n", p->formations.list[i].data.ulist[j]->attr.gen.kills);
			if (p->formations.list[i].data.ulist[j]->attr.gen.losses) {
				fprintf (rf, "\t\tIt is lost. (%u losses)\n", p->formations.list[i].data.ulist[j]->attr.gen.losses);
			}
			omor = p->formations.list[i].data.ulist[j]->data.mor;
			emor = omor - p->formations.list[i].data.ulist[j]->data.sup; if (emor < 0) emor = 0;
			fprintf (rf, "\t\tIt is %s and %s, with %u suppression and effective morale %d (%6.2f %%)\n",
				p->formations.list[i].data.ulist[j]->strings.status,
				p->formations.list[i].data.ulist[j]->strings.entr,
				p->formations.list[i].data.ulist[j]->data.sup,
				emor, 100.0 * (double)emor / (double)omor);
			fprintf (rf, "\t\tIt has %s with higher command.\n", p->formations.list[i].data.ulist[j]->strings.contact);
			hc = p->formations.list[i].data.ulist[j]->data.hcnt;
			hl = p->formations.list[i].data.ulist[j]->data.hcnt_left;
			fprintf (rf, "\t\tIt has %d of %d effectives available (%6.2f %%)\n",
				hl, hc, 100.0 * (double)hl / (double)hc);
			fprintf (rf, "\t\tIt has sustained %d of %d damage.\n",
				p->formations.list[i].data.ulist[j]->data.damage,
				p->formations.list[i].data.ulist[j]->data.cost);
			fprintf (rf, "\t\tIt is %3.0f %% ready.\n", p->formations.list[i].data.ulist[j]->attr.gen.ready * 100.0);
			if (p->formations.list[i].data.ulist[j]->data.aband != SPWAW_ANONE) {
				if (p->formations.list[i].data.ulist[j]->data.aunit.up) {
					fprintf (rf, "\t\tIt is abandoned by %s %s.\n",
						p->formations.list[i].data.ulist[j]->data.aunit.up->strings.uid,
						p->formations.list[i].data.ulist[j]->data.aunit.up->data.dname);
				} else {
					fprintf (rf, "\t\tIt is abandoned by a lost crew.\n");
				}
			}
			fprintf (rf, "\t\tIt is situated at (%d, %d).\n",
				p->formations.list[i].data.ulist[j]->data.posx,
				p->formations.list[i].data.ulist[j]->data.posy);
		}
		fprintf (rf, "\n");
	}

	fprintf (rf, "\n");
}

void
narrative_report (SPWAW_SNAPSHOT *ptr, FILE *rf, bool core)
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
