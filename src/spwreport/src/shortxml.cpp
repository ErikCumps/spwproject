/** \file
 * The Steel Panthers World at War report tool.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL V2
 */

#include "spwreport.h"
#include "xml_utils.h"
#include "utils.h"

static void
report_comment (Xml &xml, SPWAW_SNAP_CMT *ptr)
{
	XmlScope scope (xml, "comment");

	xml.item ("title", ptr->title);
}

static void
report_battle (Xml &xml, SPWAW_SNAP_BATTLE *ptr)
{
	if (!ptr) return;

	XmlScope scope(xml, "battle");

	xml.item ("date", ptr->strings.date);
	{
		XmlScope scope(xml, "turn");
		xml.item ("current", ptr->data.tdate.turn);
		xml.item ("max", ptr->data.turn_max);
	}
	xml.item ("status", ptr->strings.status);
	xml.item ("location", ptr->data.location);
	xml.item ("terrain", ptr->strings.terrain);
	xml.item ("weather", ptr->strings.weather);
	{
		XmlScope scope (xml, "visibility");
		xml.item ("hexes", ptr->data.visibility);
		xml.item ("meters", HEX2M (ptr->data.visibility));
	}

	{
		XmlScope scope(xml, "player1");
		xml.item ("oob", ptr->data.OOB_p1);
		xml.item ("nation", ptr->strings.nation_p1);
		xml.item ("mission", ptr->strings.miss_p1);
	}
	{
		XmlScope scope(xml, "player2");
		xml.item ("oob", ptr->data.OOB_p2);
		xml.item ("nation", ptr->strings.nation_p2);
		xml.item ("mission", ptr->strings.miss_p2);
	}
	if (ptr->data.OOB_p3) {
		XmlScope scope(xml, "player3");
		xml.item ("oob", ptr->data.OOB_p3);
		xml.item ("nation", ptr->strings.nation_p3);
	}
	if (ptr->data.OOB_p4) {
		XmlScope scope(xml, "player4");
		xml.item ("oob", ptr->data.OOB_p4);
		xml.item ("nation", ptr->strings.nation_p4);
	}

	{
		XmlScope scope(xml, "victory_hex_stats", "count", ARRAYCOUNT(ptr->stats.vhex_stats));

		for (int i=0; i<ARRAYCOUNT(ptr->stats.vhex_stats)-1; i++) {
			XmlScope scope(xml, "victory_hex_stat");
			xml.item ("owner", SPWAW_vhstatus2str((SPWAW_VHSTATUS)i));
			xml.item ("count", ptr->stats.vhex_stats[i].count);
			xml.item ("points", ptr->stats.vhex_stats[i].value);
		}
	}
}

static void
report_campaign (Xml &xml, SPWAW_SNAP_CAMPAIGN *ptr, SPWAW_SNAP_GAME *game)
{
	if (!ptr) return;

	XmlScope scope(xml, "campaign");

	xml.item ("flag", ptr->data.campaign);

	if (ptr->data.campaign) {
		{
			XmlScope scope(xml, "dates");
			xml.item ("start", ptr->strings.start);
			xml.item ("end", ptr->strings.end);
		}
		{
			XmlScope scope(xml, "battles");
			xml.item ("fought", ptr->data.battles);
			xml.item ("max", ptr->data.battles_max);
		}
		{
			XmlScope scope(xml, "results");
			xml.item ("decisive_victories", ptr->data.majvics);
			xml.item ("marginal_victories", ptr->data.minvics);
			xml.item ("losses", ptr->data.losses);
		}

		xml.item ("battle_index", game->btlidx);
		{
			XmlScope scope(xml, "battle_result");
			{
				XmlScope scope(xml, "player1");
				xml.item ("score", ptr->data.P1score);
				xml.item ("result", ptr->strings.P1result);
			}
			{
				XmlScope scope(xml, "player2");
				xml.item ("score", ptr->data.P2score);
				xml.item ("result", ptr->strings.P2result);
			}
		}

		{
			XmlScope scope(xml, "losses");
			{
				XmlScope scope(xml, "battle");
				{
					XmlScope scope(xml, "player1");
					xml.item ("men",	   ptr->data.P1BL.men);
					xml.item ("artillery",	   ptr->data.P1BL.art);
					xml.item ("soft_vehicles", ptr->data.P1BL.soft);
					xml.item ("apc",	   ptr->data.P1BL.apc);
					xml.item ("afv",	   ptr->data.P1BL.afv);
					xml.item ("aircraft",	   ptr->data.P1BL.air);
					xml.item ("gliders",	   ptr->data.P1BL.gliders);
				}
				{
					XmlScope scope(xml, "player2");
					xml.item ("men",	   ptr->data.P2BL.men);
					xml.item ("artillery",	   ptr->data.P2BL.art);
					xml.item ("soft_vehicles", ptr->data.P2BL.soft);
					xml.item ("apc",	   ptr->data.P2BL.apc);
					xml.item ("afv",	   ptr->data.P2BL.afv);
					xml.item ("aircraft",	   ptr->data.P2BL.air);
					xml.item ("gliders",	   ptr->data.P2BL.gliders);
				}
			}
			{
				XmlScope scope(xml, "total");
				{
					XmlScope scope(xml, "player1");
					xml.item ("men",	   ptr->data.P1TL.men);
					xml.item ("artillery",	   ptr->data.P1TL.art);
					xml.item ("soft_vehicles", ptr->data.P1TL.soft);
					xml.item ("apc",	   ptr->data.P1TL.apc);
					xml.item ("afv",	   ptr->data.P1TL.afv);
					xml.item ("aircraft",	   ptr->data.P1TL.air);
					xml.item ("gliders",	   ptr->data.P1TL.gliders);
				}
				{
					XmlScope scope(xml, "player2");
					xml.item ("men",	   ptr->data.P2TL.men);
					xml.item ("artillery",	   ptr->data.P2TL.art);
					xml.item ("soft_vehicles", ptr->data.P2TL.soft);
					xml.item ("apc",	   ptr->data.P2TL.apc);
					xml.item ("afv",	   ptr->data.P2TL.afv);
					xml.item ("aircraft",	   ptr->data.P2TL.air);
					xml.item ("gliders",	   ptr->data.P2TL.gliders);
				}
			}
		}
	}
}

static void
report_map (Xml &xml, SPWAW_SNAP_MAP *ptr)
{
	if (!ptr) return;

	XmlScope scope(xml, "map");

	{
		XmlScope scope(xml, "size");
		{
			XmlScope scope(xml,"hexes");
			xml.item ("width", ptr->width);
			xml.item ("height", ptr->height);
		}
		{
			XmlScope scope(xml,"kilometers");
			xml.item ("width", HEX2K (ptr->width));
			xml.item ("height", HEX2K (ptr->height));
		}
	}
}

static void
report_game (Xml &xml, SPWAW_SNAP_GAME *ptr)
{
	if (!ptr) return;

	XmlScope scope(xml, "game");

	switch (ptr->battle.data.status) {
		case SPWAW_BTDEPLOY:
			xml.comment ("contains updated campaign info");
			break;
		case SPWAW_BTSCORE:
			xml.comment ("contains updated battle statistics");
			break;
		default:
			break;
	}

	report_comment	(xml, &(ptr->cmt));
	report_battle	(xml, &(ptr->battle));
	report_map	(xml, &(ptr->map));
	report_campaign	(xml, &(ptr->campaign), ptr);
}

static void
report_mmas (Xml &xml, char *label, SPWAW_IMMAS *ptr, SPWAW_SNAP_OOB_FORCE *fp)
{
	if (!ptr || !fp) return;

	XmlScope scope(xml, label);

	xml.item ("maximum",	ptr->max.v);
	xml.item ("minimum",	ptr->min.v);
	xml.item ("average",	ptr->avg);
}

static void
report_mmas (Xml &xml, char *label, SPWAW_FMMAS *ptr, SPWAW_SNAP_OOB_FORCE *fp)
{
	if (!ptr || !fp) return;

	XmlScope scope(xml, label);

	xml.item ("maximum",	ptr->max.v);
	xml.item ("minimum",	ptr->min.v);
	xml.item ("average",	ptr->avg);
}

static void
report_formations (Xml &xml, SPWAW_SNAP_OOB_FORCE *f)
{
	SPWAW_SNAP_OOB_F	*ptr;
	DWORD			i, j;
	SPWAW_SNAP_OOB_FEL	*fp;
	SPWAW_SNAP_OOB_UEL	*up, *ap;

	if (!f) return; ptr = &(f->formations);
	if (!ptr->cnt || !ptr->list) return;

	XmlScope scope(xml, "formations", "count", ptr->cnt);

	for (i=0; i<ptr->cnt; i++) {
		fp = &(ptr->list[i]);

		XmlScope scope(xml, "formation", "core", (fp->data.fstatus == SPWAW_FCORE) ? "true" : "false");

		xml.item ("name", fp->strings.name);
		xml.item ("leader", fp->data.leader.up->strings.uid);
		xml.item ("higher_command", fp->data.hcmd.up->strings.uid);
		xml.item ("core_status", fp->strings.status);
		xml.item ("status", fp->strings.fstatus);
		xml.item ("OOB_record_ID", fp->data.OOBrid);
		xml.item ("type", fp->strings.type);
		xml.item ("organizational_type", fp->strings.otype);
		xml.item ("kills", fp->attr.gen.kills);
		xml.item ("losses", fp->attr.gen.losses);
		xml.item ("readiness", fp->attr.gen.ready * 100.0);

		{
			XmlScope scope(xml, "units", "count", fp->data.ucnt);
		
			for (j=0; j<fp->data.ucnt; j++) {
				up = fp->data.ulist[j];

				XmlScope scope(xml, "unit");

				xml.item ("ID", up->strings.uid);
				xml.item ("designation", up->data.dname);
				xml.item ("rank", up->strings.rank);
				xml.item ("name", up->data.lname);
				xml.item ("status", up->strings.status);
				xml.item ("formation_leader", (up->data.RID == up->data.formation->data.leader.up->data.RID) ? "yes" : "no");
				if (up->data.aband == SPWAW_ASTAY) {
					ap = up->data.aunit.up;
					if (ap) {
						XmlScope scope(xml, "abandoned", "by", "crew");
						xml.item ("ID", ap->strings.uid);
					} else {
						XmlScope scope(xml, "abandoned", "by", "lost crew");
					}
				}
				xml.item ("kills", up->attr.gen.kills);
				xml.item ("losses", up->attr.gen.losses);
				xml.item ("readiness", up->attr.gen.ready * 100.0);
				xml.item ("headcount", up->data.hcnt);
			}
		}
	}
}

static void
report_units (Xml &xml, SPWAW_SNAP_OOB_FORCE *f)
{
	SPWAW_SNAP_OOB_U	*ptr;
	DWORD			i;
	SPWAW_SNAP_OOB_UEL	*p;

	if (!f) return; ptr = &(f->units);
	if (!ptr->cnt || !ptr->list) return;

	XmlScope scope(xml, "units", "count", ptr->cnt);

	for (i=0; i<ptr->cnt; i++) {
		SPWAW_SNAP_OOB_FEL	*fp;
		p = &(ptr->list[i]);

		XmlScope scope(xml, "unit", "core", p->core ? "true" : "false");

		xml.item ("ID", p->strings.uid);
		xml.item ("designation", p->data.dname);
		xml.item ("unit", p->data.uname);
		xml.item ("leader", p->data.lname);
		xml.item ("rank", p->strings.rank);

		// Prevent warning C4244
		if ((fp = p->data.formation) == NULL) { fp = p->data.aunit.up->data.formation; }
		xml.item ("formation", fp->strings.name);

		xml.item ("OOB", p->strings.people);
		xml.item ("OOBrid", p->data.OOBrid);
		xml.item ("type", p->strings.utype);
		xml.item ("class", p->strings.uclass);
		xml.item ("kills", p->data.kills);
		xml.item ("status", p->strings.status);
		xml.item ("alive", p->data.alive ? "alive" : "dead");
		xml.item ("spotted", p->data.spotted ? "spotted" : "hidden");
		xml.item ("entrenchment", p->strings.entr);
		{
			XmlScope scope(xml, "headcount");
			xml.item ("count", p->data.hcnt);
			xml.item ("left", p->data.hcnt_left);
		}
		xml.item ("damage", p->data.damage);
		{
			XmlScope scope(xml, "abandoned", "status", (p->data.aband == SPWAW_ASTAY) ? "yes" : "no");
			if (p->data.aband == SPWAW_ASTAY) {
				if (p->data.aunit.up) {
					xml.item ("by", p->data.aunit.up->strings.uid);
				} else {
					xml.item ("by", "lost crew");
				}
			}
		}
		{
			XmlScope scope(xml, "loaded", "status", p->data.loaded ? "yes" : "no");
			if (p->data.loaded) {
				xml.item ("loader", p->data.loader.up->strings.uid);
			}
		}
		xml.item ("experience", p->data.exp);
		xml.item ("experience_class", p->strings.exp);
		xml.item ("morale", p->data.mor);
		xml.item ("suppression", p->data.sup);
		xml.item ("rally", p->data.ral);
		xml.item ("inf_skills", p->data.inf);
		xml.item ("art_skills", p->data.art);
		xml.item ("arm_skills", p->data.arm);
		xml.item ("smokedevice", p->data.smkdev);
		xml.item ("contact", p->strings.contact);
		xml.item ("kills", p->attr.gen.kills);
		xml.item ("losses", p->attr.gen.losses);
		xml.item ("readiness", p->attr.gen.ready * 100.0);
		{
			XmlScope scope(xml, "position");
			xml.item ("x", p->data.posx);
			xml.item ("y", p->data.posy);
		}
	}
}

static void
report_crews (Xml &xml, SPWAW_SNAP_OOB_FORCE *f)
{
	SPWAW_SNAP_OOB_U	*ptr;
	DWORD			i;
	SPWAW_SNAP_OOB_UEL	*p;

	if (!f) return; ptr = &(f->crews);
	if (!ptr->cnt || !ptr->list) return;

	XmlScope scope(xml, "crews", "count", ptr->cnt);

	for (i=0; i<ptr->cnt; i++) {
		SPWAW_SNAP_OOB_FEL	*fp;
		p = &(ptr->list[i]);

		XmlScope scope(xml, "crew", "core", p->core ? "true" : "false");

		xml.item ("ID", p->strings.uid);
		xml.item ("designation", p->data.dname);
		xml.item ("unit", p->data.uname);
		xml.item ("leader", p->data.lname);
		xml.item ("rank", p->strings.rank);

		fp = p->data.aunit.up->data.formation;
		xml.item ("formation", fp->strings.name);

		xml.item ("OOB", p->strings.people);
		xml.item ("OOBrid", p->data.OOBrid);
		xml.item ("type", p->strings.utype);
		xml.item ("class", p->strings.uclass);
		xml.item ("kills", p->data.kills);
		xml.item ("status", p->strings.status);
		xml.item ("alive", p->data.alive ? "alive" : "dead");
		xml.item ("spotted", p->data.spotted ? "spotted" : "hidden");
		xml.item ("entrenchment", p->strings.entr);
		{
			XmlScope scope(xml, "headcount");
			xml.item ("count", p->data.hcnt);
			xml.item ("left", p->data.hcnt_left);
		}
		xml.item ("damage", p->data.damage);
		{
			XmlScope scope(xml, "abandoned", "status", (p->data.aband == SPWAW_ALEFT) ? "yes" : "no");
			if (p->data.aband == SPWAW_ALEFT) {
				if (p->data.aunit.up) {
					xml.item ("who", p->data.aunit.up->strings.uid);
				} else {
					xml.item ("who", "lost unit");
				}
			}
		}
		{
			XmlScope scope(xml, "loaded", "status", p->data.loaded ? "yes" : "no");
			if (p->data.loaded) {
				xml.item ("loader", p->data.loader.up->strings.uid);
			}
		}
		xml.item ("experience", p->data.exp);
		xml.item ("experience_class", p->strings.exp);
		xml.item ("morale", p->data.mor);
		xml.item ("suppression", p->data.sup);
		xml.item ("rally", p->data.ral);
		xml.item ("inf_skills", p->data.inf);
		xml.item ("art_skills", p->data.art);
		xml.item ("arm_skills", p->data.arm);
		xml.item ("smokedevice", p->data.smkdev);
		xml.item ("contact", p->strings.contact);
		{
			XmlScope scope(xml, "position");
			xml.item ("x", p->data.posx);
			xml.item ("y", p->data.posy);
		}
	}
}

static void
report_attrs (Xml &xml, SPWAW_SNAP_OOB_FORCE *f)
{
	SPWAW_SNAP_OOB_GATTR	*p;

	if (!f) return;
	p = &(f->attr);

	XmlScope scope(xml, "attributes");

	{
		XmlScope scope(xml, "totals");
		xml.item ("kills", p->gen.kills);
		xml.item ("losses", p->gen.losses);
		xml.item ("readiness", p->gen.ready * 100.0);
	}
	{
		XmlScope scope(xml, "formation_attributes");
		report_mmas (xml, "kills",	&(p->mmas_fkills), f);
		report_mmas (xml, "losses",	&(p->mmas_flosses), f);
		report_mmas (xml, "readiness",	&(p->mmas_fready), f);
	}
	{
		XmlScope scope(xml, "unit_attributes");
		report_mmas (xml, "kills",	&(p->mmas_ukills.all), f);
		report_mmas (xml, "experience",	&(p->mmas_uexp.all), f);
		report_mmas (xml, "readiness",	&(p->mmas_uready.all), f);
	}
}

static void
report_unit_stats (Xml &xml, SPWAW_SNAP_OOB_FORCE *f)
{
	SPWAW_SNAP_OOB_USTATS	*p;

	if (!f) return;
	p = &(f->stats.ustats);

	XmlScope scope(xml, "units");

	xml.item ("count", p->cnt);
	xml.item ("left", p->cnt_left);
}

static void
report_formation_stats (Xml &xml, SPWAW_SNAP_OOB_FORCE *f)
{
	SPWAW_SNAP_OOB_FSTATS	*p;

	if (!f) return;
	p = &(f->stats.fstats);

	XmlScope scope(xml, "formations");

	xml.item ("count", p->cnt);
	xml.item ("left", p->cnt_left);
}

static void
report_stats (Xml &xml, SPWAW_SNAP_OOB_FORCE *f)
{
	if (!f) return;

	XmlScope scope(xml, "statistics");
	report_formation_stats	(xml, f);
	report_unit_stats	(xml, f);
}

static void
report_oob (Xml &xml, SPWAW_SNAP_OOB *ptr, bool core)
{
	SPWAW_SNAP_OOB_FORCE	*p;

	if (!ptr) return;

	XmlScope scope (xml, "OOB", "player", (ptr->side == 0) ? 1 : 2);

	xml.item ("side", ptr->side);
	xml.item ("OOB", ptr->OOB);
	xml.item ("nation", ptr->nation);

	p = core ? &(ptr->core) : &(ptr->battle);

	if (!p->formations.cnt) {
		xml.comment ("no force present in OOB");
		return;
	}

	{
		XmlScope scope(xml, "leader");
		xml.item ("ID", p->leader->strings.uid);
		xml.item ("designation", p->leader->data.dname);
		xml.item ("rank", p->leader->strings.rank);
		xml.item ("name", p->leader->data.lname);
	}

	report_attrs		(xml, p);
	report_stats		(xml, p);
	report_formations	(xml, p);
	report_units		(xml, p);
	report_crews		(xml, p);
}

void
shortxml_report (SPWAW_SNAPSHOT *ptr, FILE *rf, bool core)
{
	if (!ptr || !rf) return;

	Xml	xml (rf);

	{
		XmlScope scope(xml, "type");
		xml.item ("game", SPWAW_gametype2str (ptr->gametype));
		xml.item ("battle", SPWAW_battletype2str (ptr->type));
	}

	report_game	(xml, &(ptr->game));
	report_oob	(xml, &(ptr->OOBp1), core);	fprintf (rf, "\n\n");
	report_oob	(xml, &(ptr->OOBp2), core);	fprintf (rf, "\n\n");

	return;
}
