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
	{
		XmlScope scope (xml, "status");
		xml.item ("value", ptr->data.status);
		xml.item ("description", ptr->strings.status);
	}
	xml.item ("location", ptr->data.location);
	{
		XmlScope scope (xml, "terrain");
		xml.item ("value", ptr->data.terrain);
		xml.item ("description", ptr->strings.terrain);
	}
	{
		XmlScope scope (xml, "weather");
		xml.item ("value", ptr->data.weather);
		xml.item ("description", ptr->strings.weather);
	}
	{
		XmlScope scope (xml, "visibility");
		xml.item ("hexes", ptr->data.visibility);
		xml.item ("meters", HEX2M (ptr->data.visibility));
	}

	{
		XmlScope scope(xml, "player1");
		xml.item ("oob", ptr->data.OOB_p1);
		xml.item ("forces", ptr->strings.people_p1);
		{
			XmlScope scope(xml, "mission");
			xml.item ("value", ptr->data.miss_p1);
			xml.item ("description", ptr->strings.miss_p1);
		}
	}
	{
		XmlScope scope(xml, "player2");
		xml.item ("oob", ptr->data.OOB_p2);
		xml.item ("forces", ptr->strings.people_p2);
		{
			XmlScope scope(xml, "mission");
			xml.item ("value", ptr->data.miss_p2);
			xml.item ("description", ptr->strings.miss_p2);
		}
	}
	if (ptr->data.OOB_p3) {
		XmlScope scope(xml, "player3");
		xml.item ("oob", ptr->data.OOB_p3);
		xml.item ("forces", ptr->strings.people_p3);
	}
	if (ptr->data.OOB_p4) {
		XmlScope scope(xml, "player4");
		xml.item ("oob", ptr->data.OOB_p4);
		xml.item ("forces", ptr->strings.people_p4);
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
report_mmas (Xml &xml, char *label, SPWAW_IMMAS *ptr, SPWAW_SNAP_OOB_FORCE *fp, bool unit)
{
	char			smax[256];
	char			smin[256];
	SPWAW_SNAP_OOB_PTR	p;

	if (!ptr || !fp) return;

	XmlScope scope(xml, label);

	memset (smax, 0, sizeof (smax));
	memset (smin, 0, sizeof (smin));
	if (unit) {
		p.up = &(fp->units.list[ptr->max.i]);
		snprintf (smax, sizeof (smax) - 1, "%s: %s (%s %s)", p.up->strings.uid, p.up->data.dname, p.up->strings.rank, p.up->data.lname);
		p.up = &(fp->units.list[ptr->min.i]);
		snprintf (smin, sizeof (smin) - 1, "%s: %s (%s %s)", p.up->strings.uid, p.up->data.dname, p.up->strings.rank, p.up->data.lname);
	} else {
		p.fp = &(fp->formations.list[ptr->max.i]);
		snprintf (smax, sizeof (smax) - 1, "%s: %s %s", p.fp->strings.name, p.fp->strings.fstatus, p.fp->strings.otype);
		p.fp = &(fp->formations.list[ptr->min.i]);
		snprintf (smin, sizeof (smin) - 1, "%s: %s %s", p.fp->strings.name, p.fp->strings.fstatus, p.fp->strings.otype);
	}

	{
		XmlScope scope(xml, "maximum");
		xml.item ("value", ptr->max.v);
		{
			XmlScope scope(xml, unit ? "unit" : "formation");
			xml.item ("index", ptr->max.i);
			xml.item ("description", smax);
		}
	}
	{
		XmlScope scope(xml, "minimum");
		xml.item ("value", ptr->min.v);
		{
			XmlScope scope(xml, unit ? "unit" : "formation");
			xml.item ("index", ptr->min.i);
			xml.item ("description", smin);
		}
	}
	xml.item ("average",	ptr->avg);
	xml.item ("spread",	ptr->spr);
	xml.item ("total",	ptr->tot);
	xml.item ("count",	ptr->cnt);
}

static void
report_mmas (Xml &xml, char *label, SPWAW_FMMAS *ptr, SPWAW_SNAP_OOB_FORCE *fp, bool unit)
{
	char			smax[256];
	char			smin[256];
	SPWAW_SNAP_OOB_PTR	p;

	if (!ptr || !fp) return;

	XmlScope scope(xml, label);

	memset (smax, 0, sizeof (smax));
	memset (smin, 0, sizeof (smin));
	if (unit) {
		p.up = &(fp->units.list[ptr->max.i]);
		snprintf (smax, sizeof (smax) - 1, "%s: %s (%s %s)", p.up->strings.uid, p.up->data.dname, p.up->strings.rank, p.up->data.lname);
		p.up = &(fp->units.list[ptr->min.i]);
		snprintf (smin, sizeof (smin) - 1, "%s: %s (%s %s)", p.up->strings.uid, p.up->data.dname, p.up->strings.rank, p.up->data.lname);
	} else {
		p.fp = &(fp->formations.list[ptr->max.i]);
		snprintf (smax, sizeof (smax) - 1, "%s: %s %s", p.fp->strings.name, p.fp->strings.fstatus, p.fp->strings.otype);
		p.fp = &(fp->formations.list[ptr->min.i]);
		snprintf (smin, sizeof (smin) - 1, "%s: %s %s", p.fp->strings.name, p.fp->strings.fstatus, p.fp->strings.otype);
	}

	{
		XmlScope scope(xml, "maximum");
		xml.item ("value", ptr->max.v);
		{
			XmlScope scope(xml, unit ? "unit" : "formation");
			xml.item ("index", ptr->max.i);
			xml.item ("description", smax);
		}
	}
	{
		XmlScope scope(xml, "minimum");
		xml.item ("value", ptr->min.v);
		{
			XmlScope scope(xml, unit ? "unit" : "formation");
			xml.item ("index", ptr->min.i);
			xml.item ("description", smin);
		}
	}
	xml.item ("average",	ptr->avg);
	xml.item ("spread",	ptr->spr);
	xml.item ("total",	ptr->tot);
	xml.item ("count",	ptr->cnt);
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

		xml.item ("RID", fp->data.RID);
		xml.item ("FID", fp->data.FID);
		xml.item ("name", fp->strings.name);
				
		{
			XmlScope scope (xml, "leader");
			xml.item ("ID", fp->data.leader.up->strings.uid);
			xml.item ("designation", fp->data.leader.up->data.dname);
			xml.item ("rank", fp->data.leader.up->strings.rank);
			xml.item ("name", fp->data.leader.up->data.lname);
		}

		{
			XmlScope scope (xml, "higher_command");
			xml.item ("ID", fp->data.hcmd.up->strings.uid);
			xml.item ("designation", fp->data.hcmd.up->data.dname);
			xml.item ("rank", fp->data.hcmd.up->strings.rank);
			xml.item ("name", fp->data.hcmd.up->data.lname);
		}

		{
			XmlScope scope(xml, "core_status");
			xml.item ("value", fp->data.status);
			xml.item ("description", fp->strings.status);
		}

		{
			XmlScope scope(xml, "status");
			xml.item ("value", fp->data.fstatus);
			xml.item ("description", fp->strings.fstatus);
		}
		
		xml.item ("OOB_record_ID", fp->data.OOBrid);
		xml.item ("type", fp->strings.type);
		xml.item ("organizational_type", fp->strings.otype);

		{
			XmlScope scope(xml, "attributes");
			xml.item ("kills", fp->attr.gen.kills);
			xml.item ("losses", fp->attr.gen.losses);
			xml.item ("readiness", fp->attr.gen.ready * 100.0);
			report_mmas (xml, "kills",	&(fp->attr.mmas_kills), f, true);
			report_mmas (xml, "experience",	&(fp->attr.mmas_exp), f, true);
			report_mmas (xml, "readiness",	&(fp->attr.mmas_ready), f, true);
		}

		{
			XmlScope scope(xml, "unit_counts");
			xml.item ("total", fp->data.ucnt);
			xml.item ("core", fp->data.ucnt_core);
			xml.item ("support", fp->data.ucnt_support);
		}

		{
			XmlScope scope(xml, "units", "count", fp->data.ucnt);
		
			for (j=0; j<fp->data.ucnt; j++) {
				up = fp->data.ulist[j];

				XmlScope scope(xml, "unit");

				xml.item ("RID", up->data.RID);
				xml.item ("ID", up->strings.uid);
				xml.item ("designation", up->data.dname);
				xml.item ("rank", up->strings.rank);
				xml.item ("name", up->data.lname);

				if (up->data.aband == SPWAW_ASTAY) {
					ap = up->data.aunit.up;
	
					if (ap) {
						XmlScope scope(xml, "abandoned", "by", "crew");
						xml.item ("ID", ap->strings.uid);
						xml.item ("designation", ap->data.dname);
						xml.item ("rank", ap->strings.rank);
						xml.item ("name", ap->data.lname);
					} else {
						XmlScope scope(xml, "abandoned", "by", "lost crew");
					}
				}
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

		xml.item ("RID", p->data.RID);
		xml.item ("FRID", p->data.FRID);
		xml.item ("FMID", p->data.FMID);
		xml.item ("FSID", p->data.FSID);
		xml.item ("ID", p->strings.uid);
		xml.item ("designation", p->data.dname);
		xml.item ("unit", p->data.uname);

		xml.item ("leader", p->data.lname);
		{
			XmlScope scope(xml, "rank");
			xml.item ("value", p->data.rank);
			xml.item ("description", p->strings.rank);
		}

		// Prevent warning C4244
		if ((fp = p->data.formation) == NULL) { fp = p->data.aunit.up->data.formation; }
		{
			XmlScope scope(xml, "formation");
			xml.item ("name", fp->strings.name);
			xml.item ("core_status", fp->strings.status);
			xml.item ("status", fp->strings.fstatus);
			xml.item ("organizational_type", fp->strings.otype);
		}

		{
			XmlScope scope(xml, "detected_type");
			xml.item ("value", p->data.dutype);
			xml.item ("description", SPWAW_unittype2str (p->data.dutype));
		}
		{
			XmlScope scope(xml, "OOB");
			xml.item ("value", p->data.OOB);
			xml.item ("description", p->strings.people);
		}
		xml.item ("OOBrid", p->data.OOBrid);
		{
			XmlScope scope(xml, "type");
			xml.item ("value", p->data.utype);
			xml.item ("description", p->strings.utype);
		}
		{
			XmlScope scope(xml, "class");
			xml.item ("value", p->data.uclass);
			xml.item ("description", p->strings.uclass);
		}
		xml.item ("kills", p->data.kills);
		{
			XmlScope scope(xml, "status");
			xml.item ("value", p->data.status);
			xml.item ("description", p->strings.status);
			{
				XmlScope scope(xml, "alive");
				xml.item ("value", p->data.alive);
				xml.item ("description", p->data.alive ? "alive" : "dead");
			}
			{
				XmlScope scope(xml, "spotted");
				xml.item ("value", p->data.spotted);
				xml.item ("description", p->data.spotted ? "spotted" : "hidden");
			}
		}
		{
			XmlScope scope(xml, "entrenchment");
			xml.item ("value", p->data.entr);
			xml.item ("description", p->strings.entr);
		}
		{
			XmlScope scope(xml, "headcount");
			xml.item ("count", p->data.hcnt);
			xml.item ("left", p->data.hcnt_left);
		}
		xml.item ("damage", p->data.damage);
		{
			XmlScope scope(xml, "abandonment");
			xml.item ("value", p->data.aband);
			switch (p->data.aband) {
				case SPWAW_ASTAY:
					if (p->data.aunit.up) {
						XmlScope scope(xml, "abandoned", "by", "crew");
						xml.item ("ID", p->data.aunit.up->strings.uid);
						xml.item ("designation", p->data.aunit.up->data.dname);
						xml.item ("rank", p->data.aunit.up->strings.rank);
						xml.item ("name", p->data.aunit.up->data.lname);
					} else {
						XmlScope scope(xml, "abandoned", "by", "lost crew");
					}
					break;
				case SPWAW_ALEFT:
					{
						XmlScope scope(xml, "abandoned", "from", "unit");
						xml.item ("ID", p->data.aunit.up->strings.uid);
						xml.item ("designation", p->data.aunit.up->data.dname);
						xml.item ("rank", p->data.aunit.up->strings.rank);
						xml.item ("name", p->data.aunit.up->data.lname);
					}
					break;
				default:
					break;
			}
		}
		{
			XmlScope scope(xml, "loaded");
			xml.item ("value", p->data.loaded);
			if (p->data.loaded) {
				XmlScope scope(xml, "loader");
				xml.item ("ID", p->data.loader.up->strings.uid);
				xml.item ("designation", p->data.loader.up->data.dname);
				xml.item ("rank", p->data.loader.up->strings.rank);
				xml.item ("name", p->data.loader.up->data.lname);
			}
		}
		xml.item ("experience", p->data.exp);
		{
			XmlScope scope(xml, "experience_class");
			xml.item ("value", p->data.eclass);
			xml.item ("description", p->strings.exp);
		}
		xml.item ("morale", p->data.mor);
		xml.item ("suppression", p->data.sup);
		xml.item ("rally", p->data.ral);
		xml.item ("inf_skills", p->data.inf);
		xml.item ("art_skills", p->data.art);
		xml.item ("arm_skills", p->data.arm);
		{
			XmlScope scope(xml, "speed");
			xml.item ("value", p->data.speed);
			xml.item ("kph", HEX2KPH (p->data.speed));
		}
		xml.item ("smokedevice", p->data.smkdev);
		{
			XmlScope scope(xml, "contact");
			xml.item ("value", p->data.contact);
			xml.item ("description", p->strings.contact);
		}
		xml.item ("rate_of_fire", p->data.rof);
		xml.item ("targetting", p->data.tgt);
		xml.item ("range_finder", p->data.rf);
		xml.item ("fire_control", p->data.fc);
		xml.item ("infrared", p->data.iv);
		xml.item ("swim", p->data.swim);
		xml.item ("cost", p->data.cost);
		xml.item ("type_group", p->data.UTGidx);
		{
			XmlScope scope(xml, "position");
			xml.item ("x", p->data.posx);
			xml.item ("y", p->data.posy);
		}

		{
			XmlScope scope(xml, "attributes");
			xml.item ("kills", p->attr.gen.kills);
			xml.item ("losses", p->attr.gen.losses);
			xml.item ("readiness", p->attr.gen.ready * 100.0);
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

		xml.item ("RID", p->data.RID);
		xml.item ("FRID", p->data.FRID);
		xml.item ("FMID", p->data.FMID);
		xml.item ("FSID", p->data.FSID);
		xml.item ("ID", p->strings.uid);
		xml.item ("designation", p->data.dname);
		xml.item ("unit", p->data.uname);

		xml.item ("leader", p->data.lname);
		{
			XmlScope scope(xml, "rank");
			xml.item ("value", p->data.rank);
			xml.item ("description", p->strings.rank);
		}

		fp = p->data.aunit.up->data.formation;
		{
			XmlScope scope(xml, "formation");
			xml.item ("name", fp->strings.name);
			xml.item ("core_status", fp->strings.status);
			xml.item ("status", fp->strings.fstatus);
			xml.item ("organizational_type", fp->strings.otype);
		}

		{
			XmlScope scope(xml, "detected_type");
			xml.item ("value", p->data.dutype);
			xml.item ("description", SPWAW_unittype2str (p->data.dutype));
		}
		{
			XmlScope scope(xml, "OOB");
			xml.item ("value", p->data.OOB);
			xml.item ("description", p->strings.people);
		}
		xml.item ("OOBrid", p->data.OOBrid);
		{
			XmlScope scope(xml, "type");
			xml.item ("value", p->data.utype);
			xml.item ("description", p->strings.utype);
		}
		{
			XmlScope scope(xml, "class");
			xml.item ("value", p->data.uclass);
			xml.item ("description", p->strings.uclass);
		}
		xml.item ("kills", p->data.kills);
		{
			XmlScope scope(xml, "status");
			xml.item ("value", p->data.status);
			xml.item ("description", p->strings.status);
			{
				XmlScope scope(xml, "alive");
				xml.item ("value", p->data.alive);
				xml.item ("description", p->data.alive ? "alive" : "dead");
			}
			{
				XmlScope scope(xml, "spotted");
				xml.item ("value", p->data.spotted);
				xml.item ("description", p->data.spotted ? "spotted" : "hidden");
			}
		}
		{
			XmlScope scope(xml, "entrenchment");
			xml.item ("value", p->data.entr);
			xml.item ("description", p->strings.entr);
		}
		{
			XmlScope scope(xml, "headcount");
			xml.item ("count", p->data.hcnt);
			xml.item ("left", p->data.hcnt_left);
		}
		xml.item ("damage", p->data.damage);
		{
			XmlScope scope(xml, "abandonment");
			xml.item ("value", p->data.aband);
			switch (p->data.aband) {
				case SPWAW_ASTAY:
					if (p->data.aunit.up) {
						XmlScope scope(xml, "abandoned", "by", "crew");
						xml.item ("ID", p->data.aunit.up->strings.uid);
						xml.item ("designation", p->data.aunit.up->data.dname);
						xml.item ("rank", p->data.aunit.up->strings.rank);
						xml.item ("name", p->data.aunit.up->data.lname);
					} else {
						XmlScope scope(xml, "abandoned", "by", "lost crew");
					}
					break;
				case SPWAW_ALEFT:
					{
						XmlScope scope(xml, "abandoned", "from", "unit");
						xml.item ("ID", p->data.aunit.up->strings.uid);
						xml.item ("designation", p->data.aunit.up->data.dname);
						xml.item ("rank", p->data.aunit.up->strings.rank);
						xml.item ("name", p->data.aunit.up->data.lname);
					}
					break;
				default:
					break;
			}
		}
		{
			XmlScope scope(xml, "loaded");
			xml.item ("value", p->data.loaded);
			if (p->data.loaded) {
				xml.item ("ID", p->data.loader.up->strings.uid);
				xml.item ("designation", p->data.loader.up->data.dname);
				xml.item ("rank", p->data.loader.up->strings.rank);
				xml.item ("name", p->data.loader.up->data.lname);
			}
		}
		xml.item ("experience", p->data.exp);
		{
			XmlScope scope(xml, "experience_class");
			xml.item ("value", p->data.eclass);
			xml.item ("description", p->strings.exp);
		}
		xml.item ("morale", p->data.mor);
		xml.item ("suppression", p->data.sup);
		xml.item ("rally", p->data.ral);
		xml.item ("inf_skills", p->data.inf);
		xml.item ("art_skills", p->data.art);
		xml.item ("arm_skills", p->data.arm);
		{
			XmlScope scope(xml, "speed");
			xml.item ("value", p->data.speed);
			xml.item ("kph", HEX2KPH (p->data.speed));
		}
		xml.item ("smokedevice", p->data.smkdev);
		{
			XmlScope scope(xml, "contact");
			xml.item ("value", p->data.contact);
			xml.item ("description", p->strings.contact);
		}
		xml.item ("rate_of_fire", p->data.rof);
		xml.item ("targetting", p->data.tgt);
		xml.item ("range_finder", p->data.rf);
		xml.item ("fire_control", p->data.fc);
		xml.item ("infrared", p->data.iv);
		xml.item ("swim", p->data.swim);
		xml.item ("cost", p->data.cost);
		xml.item ("type_group", p->data.UTGidx);
		{
			XmlScope scope(xml, "position");
			xml.item ("x", p->data.posx);
			xml.item ("y", p->data.posy);
		}

		/* No attributes for crewmen! */
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
		report_mmas (xml, "kills",	&(p->mmas_fkills), f, false);
		report_mmas (xml, "losses",	&(p->mmas_flosses), f, false);
		report_mmas (xml, "readiness",	&(p->mmas_fready), f, false);
	}
	{
		XmlScope scope(xml, "unit_attributes");
		report_mmas (xml, "kills",	&(p->mmas_ukills.all), f, true);
		report_mmas (xml, "experience",	&(p->mmas_uexp.all), f, true);
		report_mmas (xml, "readiness",	&(p->mmas_uready.all), f, true);
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
	xml.item ("people", ptr->people);

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
xml_report (SPWAW_SNAPSHOT *ptr, FILE *rf, bool core)
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
