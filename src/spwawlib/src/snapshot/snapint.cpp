/** \file
 * The SPWaW Library - snapshot handling.
 *
 * Copyright (C) 2007-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "snapshot/index.h"
#include "snapshot/translate.h"
#include "snapshot/snapshot.h"
#include "common/internal.h"

static SPWAW_ERROR
snapint_game_battle (SPWAW_SNAPSHOT *ptr)
{
	SPWAW_ERROR			rc = SPWERR_OK;
	SPWAW_SNAP_BATTLE_RAW		*raw;
	SPWAW_SNAP_BATTLE_DATA		*dst;
	SPWAW_SNAP_BATTLE_STRINGS	*str;
	SPWAW_PERIOD			add;
	int				i;

	CNULLARG (ptr);

	raw = &(ptr->raw.game.battle);
	dst = &(ptr->game.battle.data);
	str = &(ptr->game.battle.strings);

	dst->start.year  = raw->year + SPWAW_STARTYEAR;
	dst->start.month = raw->month;
	dst->start.day   = raw->day;
	dst->start.hour  = raw->hour;

	add.stamp = raw->turn * SPWAW_MINSPERTURN;
	rc = SPWAW_date_add (&(dst->start), &add, &(dst->date)); ROE ("SPWAW_date_add(turn time)");

	dst->location   = raw->location;
	dst->visibility = raw->visibility;
	dst->terrain    = raw2terrain (raw->terrain);
	dst->weather    = raw2weather (raw->weather);
	dst->turn       = raw->turn;
	dst->turn_max   = raw->turn_max;

	if (dst->turn == 0) {
		dst->status = SPWAW_BTDEPLOY;
	} else {
		//dst->status = (!ptr->raw.game.campaign.busy) ? SPWAW_BTBUSY : SPWAW_BTSCORE;
		dst->status = (ptr->raw.game.campaign.busy == 1) ? SPWAW_BTSCORE : SPWAW_BTBUSY;
	}

	dst->OOB_p1  = raw->OOB_p1;
	dst->OOB_p2  = raw->OOB_p2;
	dst->OOB_p3  = raw->OOB_p3;
	dst->OOB_p4  = raw->OOB_p4;
	dst->miss_p1 = raw2mission (raw->miss_p1);
	dst->miss_p2 = raw2mission (raw->miss_p2);
	dst->meeting = dst->miss_p1 == dst->miss_p2;
	dst->credit  = raw->credit;

	rc = SPWAW_date2str (&(dst->date), str->date, sizeof (str->date)); ROE ("SPWAW_date2str(turn date)");
	str->terrain   = terrain2str (dst->terrain);
	str->weather   = weather2str (dst->terrain, dst->weather);
	str->status    = btstatus2str (dst->status);
	str->nation_p1 = (char *)SPWAW_oob_nation (raw->OOB_p1);
	str->people_p1 = (char *)SPWAW_oob_people (raw->OOB_p1);
	str->prefix_p1 = (char *)SPWAW_oob_prefix (raw->OOB_p1);
	str->nation_p2 = (char *)SPWAW_oob_nation (raw->OOB_p2);
	str->people_p2 = (char *)SPWAW_oob_people (raw->OOB_p2);
	str->prefix_p2 = (char *)SPWAW_oob_prefix (raw->OOB_p2);
	str->nation_p3 = (char *)SPWAW_oob_nation (raw->OOB_p3);
	str->people_p3 = (char *)SPWAW_oob_people (raw->OOB_p3);
	str->prefix_p3 = (char *)SPWAW_oob_prefix (raw->OOB_p3);
	str->nation_p4 = (char *)SPWAW_oob_nation (raw->OOB_p4);
	str->people_p4 = (char *)SPWAW_oob_people (raw->OOB_p4);
	str->prefix_p4 = (char *)SPWAW_oob_prefix (raw->OOB_p4);
	str->miss_p1   = mission2str (dst->miss_p1);
	str->miss_p2   = mission2str (dst->miss_p2);

	for (i=0; i<SPWAW_VHEXCNT; i++) {
		dst->vhex[i].x = raw->vhex[i].x;
		dst->vhex[i].y = raw->vhex[i].y;
		dst->vhex[i].value = raw->vhex[i].value;
		dst->vhex[i].status = raw2vhstatus (raw->vhex[i].owner);
	}

	return (SPWERR_OK);
}

static SPWAW_ERROR
snapint_game_campaign (SPWAW_SNAPSHOT *ptr)
{
	SPWAW_ERROR			rc = SPWERR_OK;
	SPWAW_SNAP_CAMPAIGN_RAW		*raw;
	SPWAW_SNAP_CAMPAIGN_DATA	*dst;
	SPWAW_SNAP_CAMPAIGN_STRINGS	*str;

	CNULLARG (ptr);

	raw = &(ptr->raw.game.campaign);
	dst = &(ptr->game.campaign.data);
	str = &(ptr->game.campaign.strings);

	dst->campaign    = raw->campaign;
	dst->start.year  = raw->start_year + SPWAW_STARTYEAR;
	dst->start.month = (char)(raw->start_month);
	dst->start.day   = 0;
	dst->start.hour  = dst->start.minute = 0;
	dst->end.year    = raw->end_year + SPWAW_STARTYEAR;
	dst->end.month   = (char)(raw->end_month);
	dst->end.day     = 0;
	dst->end.hour    = dst->end.minute = 0;
	dst->battles     = raw->battles;
	dst->majvics     = raw->majvics;
	dst->minvics     = raw->minvics;
	dst->losses      = 0;
	dst->battles_max = raw->battles_max;

	if (raw->start_year != 0) {
		dst->losses = dst->battles - dst->majvics - dst->minvics;
	}

	rc = SPWAW_date2str (&(dst->start), str->start, sizeof (str->start)); ROE ("SPWAW_date2str(start date)");
	rc = SPWAW_date2str (&(dst->end), str->end, sizeof (str->end)); ROE ("SPWAW_date2str(end date)");

	dst->P1BL.men     = raw->P1BLmen;
	dst->P1BL.art     = raw->P1BLart;
	dst->P1BL.soft    = raw->P1BLsoft;
	dst->P1BL.apc     = raw->P1BLapc;
	dst->P1BL.afv     = raw->P1BLafv;
	dst->P1BL.gliders = raw->P1BLgliders;
	dst->P1BL.air     = raw->P1BLair;
	dst->P2BL.men     = raw->P2BLmen;
	dst->P2BL.art     = raw->P2BLart;
	dst->P2BL.soft    = raw->P2BLsoft;
	dst->P2BL.apc     = raw->P2BLapc;
	dst->P2BL.afv     = raw->P2BLafv;
	dst->P2BL.gliders = raw->P2BLgliders;
	dst->P2BL.air     = raw->P2BLair;

	dst->P1TL.men     = raw->P1TLmen;
	dst->P1TL.art     = raw->P1TLart;
	dst->P1TL.soft    = raw->P1TLsoft;
	dst->P1TL.apc     = raw->P1TLapc;
	dst->P1TL.afv     = raw->P1TLafv;
	dst->P1TL.gliders = raw->P1TLgliders;
	dst->P1TL.air     = raw->P1TLair;
	dst->P2TL.men     = raw->P2TLmen;
	dst->P2TL.art     = raw->P2TLart;
	dst->P2TL.soft    = raw->P2TLsoft;
	dst->P2TL.apc     = raw->P2TLapc;
	dst->P2TL.afv     = raw->P2TLafv;
	dst->P2TL.gliders = raw->P2TLgliders;
	dst->P2TL.air     = raw->P2TLair;

	dst->P1score = raw->P1score;
	dst->P2score = raw->P2score;

	return (SPWERR_OK);
}

static SPWAW_ERROR
snapint_game_map (SPWAW_SNAPSHOT *ptr)
{
	SPWAW_SNAP_MAP_RAW	*raw;
	SPWAW_SNAP_MAP		*dst;
	int			x, y, idx;

	CNULLARG (ptr);

	raw = &(ptr->raw.game.map);
	dst = &(ptr->game.map);

	dst->width  = raw->width;
	dst->height = raw->height;

	dst->data = safe_nmalloc (SPWAW_SNAP_MAP_DATA, dst->width * dst->height); COOM (dst->data, "SPWAW_SNAP_MAP_DATA array");

        for (x=0; x<dst->width; x++) {
                for (y=0; y<dst->height; y++) {
			idx = y*dst->width+x;
			dst->data[idx].h = (int)raw->data[idx].height;
			dst->data[idx].water  = rawtfs2water (raw->data[idx].has_T1, raw->data[idx].has_T2, raw->data[idx].has_T3, raw->data[idx].has_T4);
			dst->data[idx].bridge = rawtfs2bridge (raw->data[idx].has_T1, raw->data[idx].has_T2, raw->data[idx].has_T3, raw->data[idx].has_T4);
			dst->data[idx].road   = rawtfs2road (raw->data[idx].has_T1, raw->data[idx].has_T2, raw->data[idx].has_T3, raw->data[idx].has_T4);
			dst->data[idx].conn_road1 = (int)raw->data[idx].conn_road1;
			dst->data[idx].conn_road2 = (int)raw->data[idx].conn_road2;
			dst->data[idx].conn_rail  = (int)raw->data[idx].conn_rail;
                }
        }

	return (SPWERR_OK);
}

static SPWAW_ERROR
snapint_game (SPWAW_SNAPSHOT *ptr)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	CNULLARG (ptr);

	ptr->game.cmt.title = ptr->raw.game.cmt.title;

	rc = snapint_game_battle (ptr);		ROE ("snapint_game_battle()");
	rc = snapint_game_campaign (ptr);	ROE ("snapint_game_campaign()");
	rc = snapint_game_map (ptr);		ROE ("snapint_game_map()");

	return (SPWERR_OK);
}

static SPWAW_ERROR
snapint_oob_formations_stage1 (SPWAW_SNAP_OOB_RAW *raw, SPWAW_SNAP_OOB *ptr, SPWOOB_DATA *oob, STRTAB *stab)
{
	SPWAW_ERROR			rc = SPWERR_OK;
	SPWAW_SNAP_OOB_F		*p;
	USHORT				i;
	USHORT				form_oobrid, unit_oobrid;
	char				buf[256];

	CNULLARG (raw); CNULLARG (ptr); CNULLARG (oob);
	p = &(ptr->battle.formations);

	p->cnt = raw->formations.cnt;
	p->list = safe_nmalloc(SPWAW_SNAP_OOB_FEL, p->cnt);
	COOMGOTO (p->list, "SPWAW_SNAP_OOB_FEL list", handle_error);

	for (i=0; i<p->cnt; i++) {
		SPWAW_SNAP_OOB_FELRAW		*src = &(raw->formations.raw[i]);
		SPWAW_SNAP_OOB_FEL_DATA		*dat = &(p->list[i].data);
		SPWAW_SNAP_OOB_FEL_STRINGS	*str = &(p->list[i].strings);

		dat->OOBrid = src->OOBrid;

		form_oobrid = unit_oobrid = 0;

		if ((form_oobrid = dat->OOBrid) == 0) {
			// If there is no OOB record ID for this formation then it's a
			// special formation and we need to look up the OOB record ID
			// of its leader unit...
			SPWAW_SNAP_OOB_UELRAW	*u = NULL;

			if (check_unitid (src->leader, &(raw->units), &u)) {
				unit_oobrid = u->OOBrid;
			} else {
				RWE (SPWERR_BADSAVEDATA, "missing leader unit for formation");
			}
		}

		if (form_oobrid != 0) {
			dat->fstatus	= oob->fdata[form_oobrid].stat;
			dat->type	= oob->fdata[form_oobrid].type;
		} else {
			// For special formations we assume it's an average platoon:
			dat->fstatus	= SPWOOB_FSTAT_D;	// average
			dat->type	= SPWOOB_FTYPE_PLT;	// platoon
		}

		if (src->name == NULL) {
			if (dat->type == SPWOOB_FTYPE_FHQ) {
				memset (buf, 0, sizeof (buf));
				snprintf (buf, sizeof (buf) - 1, "%s HQ", ptr->people);
				src->name = STRTAB_add (stab, buf);
			} else {
				if (form_oobrid != 0) {
					src->name = azstrstab (oob->fdata[form_oobrid].name, stab);
				} else {
					// For special formations we use the type name of its leader unit
					src->name = azstrstab (oob->udata[unit_oobrid].name, stab);
				}
			}
		}

		dat->idx	= i;
		dat->RID	= src->RID;
		dat->FID	= src->FID;
		dat->status	= raw2fstatus (src->status);
		dat->leader.rid	= src->leader;
		dat->hcmd.rid	= src->hcmd;
		str->utype	= src->name;
	}

	return (SPWERR_OK);

handle_error:
	if (p->list) free (p->list);
	return (rc);
}

static inline void
loaded (SPWAW_SNAP_OOB_UELRAW *ptr, bool *flag, SPWAW_SNAP_OOB_PTR *id)
{
	if (flag) *flag = false;
	if (id) id->rid = SPWAW_BADIDX;
	if (!flag || !id) return;

	if (ptr->loader != SPWAW_BADIDX) {
		*flag = true;
		id->rid = ptr->loader;
	}
}

static SPWAW_ERROR
snapint_oob_units_stage1 (SPWAW_SNAP_OOB_RAW *raw, SPWAW_SNAP_OOB *ptr, SPWOOB_DATA *oob, STRTAB * /*stab*/)
{
	SPWAW_ERROR			rc = SPWERR_OK;
	SPWAW_SNAP_OOB_FORCE		*p;
	USHORT				i, uidx, cidx;
	USHORT				ldridx, crwidx;
	bool				iscrew;
	USHORT				ulidx, elidx;
	SPWAW_SNAP_OOB_LELRAW		*ulsrc, *elsrc;
	USHORT				pidx;
	SPWAW_SNAP_OOB_PELRAW		*psrc;


	CNULLARG (raw); CNULLARG (ptr); CNULLARG (oob);
	p = &(ptr->battle);

	// Detect unit and crew counts
	for (i=0; i<(int)raw->units.cnt; i++) {
		if (raw->units.raw[i].type == SPWAW_UNIT_TYPE_UNKNOWN) continue;
		if (raw->units.raw[i].type == SPWAW_UNIT_TYPE_CREW) p->crews.cnt++; else p->units.cnt++;
	}

	p->units.list = safe_nmalloc(SPWAW_SNAP_OOB_UEL, p->units.cnt);
	COOMGOTO (p->units.list, "SPWAW_SNAP_OOB_UEL list", handle_error);

	p->crews.list = safe_nmalloc(SPWAW_SNAP_OOB_UEL, p->crews.cnt);
	COOMGOTO (p->crews.list, "SPWAW_SNAP_OOB_UEL list", handle_error);

	uidx = cidx = 0;
	for (i=0; i<(int)raw->units.cnt; i++) {
		SPWAW_SNAP_OOB_UEL_STRINGS	*str;
		SPWAW_SNAP_OOB_UEL_DATA		*dat;

		if (raw->units.raw[i].type == SPWAW_UNIT_TYPE_UNKNOWN) continue;

		iscrew = (raw->units.raw[i].type == SPWAW_UNIT_TYPE_CREW);
		ldridx = raw->units.raw[i].leader;
		crwidx = raw->units.raw[i].crew;
		//log ("snapint_oob_units_stage1: [%5.5u] %4.4s: F<%5.5u,%3.3u> (%16.16s) ",
		//	raw->units.raw[i].RID, SPWAW_unittype2str(raw->units.raw[i].type), raw->units.raw[i].FMID, raw->units.raw[i].FSID, raw->units.raw[i].name);
		//log ("ldridx=%5.5u, crwidx=%5.5u, iscrew=%d, aband=%d\n",
		//	raw->units.raw[i].leader, raw->units.raw[i].crew, iscrew, raw->units.raw[i].aband);

		if (iscrew) {
			dat = &(p->crews.list[cidx].data);
			str = &(p->crews.list[cidx].strings);
			dat->idx = cidx;
			cidx++;
		} else {
			dat = &(p->units.list[uidx].data);
			str = &(p->units.list[uidx].strings);
			dat->idx = uidx;
			uidx++;
		}

		SPWAW_SNAP_OOB_UELRAW *src = &(raw->units.raw[i]);

		ulidx = lridx (&(raw->leaders), (src->leader!=SPWAW_BADIDX) ? src->leader : src->RID);
		ulsrc = (ulidx < raw->leaders.cnt) ? &(raw->leaders.raw[ulidx]) : NULL;

		elidx = lridx (&(raw->leaders), ldridx);
		elsrc = (elidx < raw->leaders.cnt) ? &(raw->leaders.raw[elidx]) : NULL;

		pidx = i;
		psrc = (pidx < raw->positions.cnt) ? &(raw->positions.raw[pidx]) : NULL;

		dat->uidx	= i;
		dat->ulidx	= ulidx;
		dat->elidx	= elidx;
		dat->RID	= src->RID;
		dat->FRID	= src->FRID;
		dat->FMID	= src->FMID;
		dat->FSID	= src->FSID;
		dat->OOB	= src->OOB;
		dat->type	= src->name;
		dat->name	= ulsrc ? ulsrc->name : NULL;
		dat->rank	= elsrc ? raw2rank (elsrc->rank) : raw2rank (SPWAW_RKIA);
		dat->kills	= elsrc ? elsrc->kills : 0;
		dat->status	= raw2ustatus (src->status);
		dat->entr	= raw2entr (src->entr);
		//dat->alive	= ulsrc ? (ulsrc->status != 0) : false;
		dat->alive	= !((dat->status == SPWAW_UDESTROYED) || (dat->status == SPWAW_UBURNING));
		//dat->lost	= !dat->alive || (dat->status == SPWAW_UDESTROYED) || (dat->status == SPWAW_UBURNING);
		dat->spotted	= psrc ? (psrc->seen == 0) : true;
		dat->hcnt	= src->men_ori;
		dat->hcnt_left	= src->men;
		dat->cost	= src->cost;
		dat->damage	= src->damage;
		dat->crew	= crwidx != SPWAW_BADIDX;
		dat->aband	= src->aband;
		dat->aunit.rid	= crwidx;
		loaded (src, &(dat->loaded), &(dat->loader));
		dat->exp	= src->exp;
		dat->eclass	= raw2exp (src->exp);
		dat->mor	= src->mor;
		dat->sup	= src->sup;
		dat->ral	= ulsrc ? ulsrc->ral : 0;
		dat->inf	= ulsrc ? ulsrc->inf : 0;
		dat->art	= ulsrc ? ulsrc->art : 0;
		dat->arm	= ulsrc ? ulsrc->arm : 0;
		dat->speed	= src->speed;
		dat->smkdev	= src->smkdev;
		dat->contact	= raw2cstatus (src->contact);
		dat->rof	= src->rof;
		dat->tgt	= src->tgt;
		dat->rf		= src->rf;
		dat->fc		= src->fc;
		dat->iv		= src->iv;
		dat->swim	= src->swim;
		dat->UTGidx	= src->UTGidx;
		dat->OOBrid	= src->OOBrid;
		dat->utype	= oob->udata[dat->OOBrid].type;
		dat->uclass	= oob->udata[dat->OOBrid].uclass;

		if (dat->loaded) {
			pidx = dat->loader.rid;
			psrc = (pidx < raw->positions.cnt) ? &(raw->positions.raw[pidx]) : NULL;
		}
		dat->posx	= psrc ? (short)psrc->x : -1;
		dat->posy	= psrc ? (short)psrc->y : -1;
		if ((dat->posx < 0) || (dat->posy < 0)) dat->posx = dat->posy = -1;
	}

	return (SPWERR_OK);

handle_error:
	if (p->units.list) free (p->units.list);
	if (p->crews.list) free (p->crews.list);
	return (rc);
}

static SPWAW_ERROR
snapint_oob_core_stage1 (SPWAW_SNAP_OOB_RAW *raw, SPWAW_SNAP_OOB *ptr, SPWOOB_DATA *oob, STRTAB *stab)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	CNULLARG (raw); CNULLARG (ptr);

	rc = snapint_oob_formations_stage1 (raw, ptr, oob, stab);	ROE ("snapint_oob_formations_stage1()");
	rc = snapint_oob_units_stage1 (raw, ptr, oob, stab);		ROE ("snapint_oob_units_stage1()");

	return (SPWERR_OK);
}

#define	NCMP(a,b)	((a < b) ? -1 : ((a>b) ? 1 : 0))

static int
sort_ucmp (const void *a, const void *b)
{
	int	n;

	SPWAW_SNAP_OOB_UEL	*fa = (SPWAW_SNAP_OOB_UEL *)a;
	SPWAW_SNAP_OOB_UEL	*fb = (SPWAW_SNAP_OOB_UEL *)b;

	n = NCMP (fa->data.FMID, fb->data.FMID);
	if (!n) n = NCMP (fa->data.FSID, fb->data.FSID);
	return (n);
}

static int
sort_fcmp (const void *a, const void *b)
{
	SPWAW_SNAP_OOB_FEL	*fa = (SPWAW_SNAP_OOB_FEL *)a;
	SPWAW_SNAP_OOB_FEL	*fb = (SPWAW_SNAP_OOB_FEL *)b;

	return (NCMP (fa->data.FID, fb->data.FID));
}

SPWAW_ERROR
OOB_link (SPWAW_SNAP_OOB *oob, bool prepsf)
{
	SPWAW_SNAP_OOB_FORCE	*bp, *cp, *sp;
	DWORD			i, j, k;
	SPWAW_SNAP_OOB_PTR	p, q;

	CNULLARG (oob);
	bp = &(oob->battle);

	cp = &(oob->core); clear_ptr (cp);
	sp = &(oob->support); clear_ptr (sp);

	/* Reset formation unit counts and core status */
	for (i=0; i<bp->formations.cnt; i++) {
		p.fp = &(bp->formations.list[i]);
		p.fp->data.ucnt = 0;
		if (prepsf) {
			if (p.fp->data.status == SPWAW_FCORE) {
				p.fp->core = true;
				cp->formations.cnt++;
			} else {
				p.fp->core = false;
				sp->formations.cnt++;
			}
		}
	}

	/* Unit/crew linkage */
	for (i=0; i<bp->units.cnt; i++) {
		p.up = &(bp->units.list[i]);
		if (p.up->data.aband == SPWAW_ALEFT) abort();
		if (p.up->data.aband == SPWAW_ANONE) continue;

		p.up->data.aunit.up = crewbyid (p.up->data.aunit.rid, bp);
		if (!p.up->data.aunit.up || (p.up->data.aunit.up->data.aunit.rid != p.up->data.RID))
		{
			p.up->data.aunit.up = NULL;
		}
	}
	for (i=0; i<bp->crews.cnt; i++) {
		p.up = &(bp->crews.list[i]);
		if (p.up->data.aband != SPWAW_ALEFT) abort();

		p.up->data.aunit.up = unitbyid (p.up->data.aunit.rid, bp);
		if (!p.up->data.aunit.up) abort();

		p.up->data.aunit.up->data.aunit.up = p.up;
	}

	/* Fix up unit crew linkage data */
	for (i=0; i<bp->units.cnt; i++) {
		p.up = &(bp->units.list[i]);

		/* Check crew linkage */
		if (p.up->data.aband != SPWAW_ANONE) {
			if ((p.up->data.status == SPWAW_UABANDONED) && p.up->data.aunit.up) {
				/* Update missing attributes with those of crew */
				p.up->data.name = p.up->data.aunit.up->data.name;
				p.up->data.rank = p.up->data.aunit.up->data.rank;
				p.up->data.ral  = p.up->data.aunit.up->data.ral;
				p.up->data.inf  = p.up->data.aunit.up->data.inf;
				p.up->data.art  = p.up->data.aunit.up->data.art;
				p.up->data.arm  = p.up->data.aunit.up->data.arm;
			}
		} else {
			p.up->data.aunit.up = NULL;
		}
	}

	/* Unit loader/passenger linkage */
	for (i=0; i<bp->units.cnt; i++) {
		p.up = &(bp->units.list[i]);

		if (p.up->data.loader.rid != SPWAW_BADIDX) {
			p.up->data.loader.up = unitorcrewbyid (p.up->data.loader.rid, bp);
			// Safety fix for loader references to non-existing units
			if (!p.up->data.loader.up) p.up->data.loaded = false;
		} else {
			p.up->data.loader.up = NULL;
			p.up->data.loaded = false;
		}
	}

	/* Unit/Formation linkage */
	for (i=0; i<bp->units.cnt; i++) {
		p.up = &(bp->units.list[i]);

		p.up->data.formation = formationbyid (p.up->data.FRID, bp);
		p.up->data.formation->data.ucnt++;
	}

	/* Crew loader/passenger linkage */
	for (i=0; i<bp->crews.cnt; i++) {
		p.up = &(bp->crews.list[i]);

		if (p.up->data.loader.rid != SPWAW_BADIDX) {
			p.up->data.loader.up = unitbyid (p.up->data.loader.rid, bp);
		} else {
			p.up->data.loader.up = NULL;
			p.up->data.loaded = false;
		}
	}

	/* Formation/Leader/Units linkage */
	for (i=0; i<bp->formations.cnt; i++) {
		p.fp  = &(bp->formations.list[i]);

		p.fp->data.leader.up = unitorcrewbyid (p.fp->data.leader.rid, bp);
		p.fp->data.hcmd.up = unitorcrewbyid (p.fp->data.hcmd.rid, bp);
		if (!p.fp->data.hcmd.up) {
			// TODO: more of these checks to prevent NULL pointer propagation
			RWE (SPWERR_BADSAVEDATA, "missing leader unit for formation");
		}

		/* Do not link crewmen to formation leader, use abandoned unit instead! */
		if (p.fp->data.leader.up->data.aband == SPWAW_ALEFT) {
			p.fp->data.leader.up = p.fp->data.leader.up->data.aunit.up;
		}

		if (p.fp->data.ucnt) {
			p.fp->data.ulist = safe_nmalloc (SPWAW_SNAP_OOB_UEL*, p.fp->data.ucnt);
			COOM (p.fp->data.ulist, "SPWAW_SNAP_OOB_UEL* list");
			for (k=j=0; j<bp->units.cnt; j++) {
				if (bp->units.list[j].data.formation == p.fp) {
					p.fp->data.ulist[k] = &(bp->units.list[j]);
					k++;
				}
			}
		}

		if (prepsf) {
			if (p.fp->core) {
				// Prevent warning C4244
				cp->units.cnt = cp->units.cnt + p.fp->data.ucnt;
				for (j=0; j<p.fp->data.ucnt; j++) {
					q.up = p.fp->data.ulist[j];
					q.up->core = true;
					if (q.up->data.aunit.up) {
						q.up->data.aunit.up->core = true;
						cp->crews.cnt++;
					}
				}
			} else {
				// Prevent warning C4244
				sp->units.cnt = sp->units.cnt + p.fp->data.ucnt;
				for (j=0; j<p.fp->data.ucnt; j++) {
					q.up = p.fp->data.ulist[j];
					q.up->core = false;
					if (q.up->data.aunit.up) {
						q.up->data.aunit.up->core = false;
						sp->crews.cnt++;
					}
				}
			}
		}
	}

	/* Loader/passenger fixup */
	for (i=0; i<bp->units.cnt; i++) {
		p.up = &(bp->units.list[i]);
		if (!p.up->data.loaded) continue;

		q.up = p.up->data.loader.up;
		while (q.up->data.loaded == true) q.up = q.up->data.loader.up;
		p.up->data.loader.up = q.up;
	}
	for (i=0; i<bp->crews.cnt; i++) {
		p.up = &(bp->crews.list[i]);
		if (!p.up->data.loaded) continue;

		q.up = p.up->data.loader.up;
		while (q.up->data.loaded == true) q.up = q.up->data.loader.up;
		p.up->data.loader.up = q.up;
	}

	/* Determine OOB force leader: it is the first unit of the (first) force HQ formation */
	for (i=0; i<bp->formations.cnt; i++) {
		p.fp = &(bp->formations.list[i]);
		if (p.fp->data.type != SPWOOB_FTYPE_FHQ) continue;
		bp->leader = p.fp->data.ulist[0];
		break;
	}

	/* Fall back to first force unit if force leader not found */
	if (!bp->leader) bp->leader = &(bp->units.list[0]);

	return (SPWERR_OK);
}

SPWAW_ERROR
OOB_build_subforce (SPWAW_SNAP_OOB_FORCE *bp, SPWAW_SNAP_OOB_FORCE *sp, bool core)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	USHORT			i, j, idx;
	SPWAW_SNAP_OOB_PTR	p;

	CNULLARG (bp); CNULLARG (sp);

	/* Copy subforce formation data */
	sp->formations.list = NULL;
	if (sp->formations.cnt) {
		sp->formations.list = safe_nmalloc(SPWAW_SNAP_OOB_FEL, sp->formations.cnt);
		COOMGOTO (sp->formations.list, "SPWAW_SNAP_OOB_FEL list", handle_error);
	}
	for (i=idx=0; i<bp->formations.cnt; i++) {
		if (bp->formations.list[i].core != core) continue;
		memcpy (&(sp->formations.list[idx++]), &(bp->formations.list[i]), sizeof (SPWAW_SNAP_OOB_FEL));
	}

	/* Copy subforce unit data */
	sp->units.list = NULL;
	if (sp->units.cnt) {
		sp->units.list = safe_nmalloc(SPWAW_SNAP_OOB_UEL, sp->units.cnt);
		COOMGOTO (sp->units.list, "SPWAW_SNAP_OOB_UEL list", handle_error);
	}
	for (i=idx=0; i<bp->units.cnt; i++) {
		if (bp->units.list[i].core != core) continue;
		memcpy (&(sp->units.list[idx++]), &(bp->units.list[i]), sizeof (SPWAW_SNAP_OOB_UEL));
	}

	/* Copy subforce crew data */
	sp->crews.list = NULL;
	if (sp->crews.cnt) {
		sp->crews.list = safe_nmalloc(SPWAW_SNAP_OOB_UEL, sp->crews.cnt);
		COOMGOTO (sp->crews.list, "SPWAW_SNAP_OOB_UEL list", handle_error);
	}
	for (i=idx=0; i<bp->crews.cnt; i++) {
		if (bp->crews.list[i].core != core) continue;
		memcpy (&(sp->crews.list[idx++]), &(bp->crews.list[i]), sizeof (SPWAW_SNAP_OOB_UEL));
	}

	/* Fixup subforce OOB records */
	for (i=0; i<sp->formations.cnt; i++) {
		p.fp = &(sp->formations.list[i]);
		p.fp->data.idx = i;
		p.fp->data.leader.up = unitbyid (p.fp->data.leader.up->data.RID, sp);
		p.fp->data.ulist = safe_nmalloc (SPWAW_SNAP_OOB_UEL*, p.fp->data.ucnt);
		COOM (p.fp->data.ulist, "SPWAW_SNAP_OOB_UEL* list");
		for (idx=j=0; j<sp->units.cnt; j++) {
			if (sp->units.list[j].data.formation->data.FID == p.fp->data.FID) {
				p.fp->data.ulist[idx++] = &(sp->units.list[j]);
				sp->units.list[j].data.formation = p.fp;
			}
		}
	}
	for (i=0; i<sp->units.cnt; i++) {
		p.up = &(sp->units.list[i]);
		p.up->data.idx = i;
		if (p.up->data.aunit.up){
			p.up->data.aunit.up = crewbyid (p.up->data.aunit.up->data.RID, sp);
		}
		if (p.up->data.loader.up) {
			if (unitbyid (p.up->data.loader.up->data.RID, sp))
				p.up->data.loader.up = unitbyid (p.up->data.loader.up->data.RID, sp);
			else
				p.up->data.loader.up = unitbyid (p.up->data.loader.up->data.RID, bp);
		}
	}
	for (i=0; i<sp->crews.cnt; i++) {
		p.up = &(sp->crews.list[i]);
		p.up->data.idx = i;
		if (p.up->data.aunit.up) {
			p.up->data.aunit.up = unitbyid (p.up->data.aunit.up->data.RID, sp);
		}
		if (p.up->data.loader.up) {
			if (unitbyid (p.up->data.loader.up->data.RID, sp))
				p.up->data.loader.up = unitbyid (p.up->data.loader.up->data.RID, sp);
			else
				p.up->data.loader.up = unitbyid (p.up->data.loader.up->data.RID, bp);
		}

	}

	/* OOB subforce force leader */
	sp->leader = &(sp->units.list[0]);

	return (SPWERR_OK);

handle_error:
	if (sp->formations.list) {
		for (i=0; i<sp->formations.cnt; i++) {
			if (sp->formations.list[i].data.ulist) free (sp->formations.list[i].data.ulist);
		}
		free (sp->formations.list);
	}
	if (sp->units.list) free (sp->units.list);
	if (sp->crews.list) free (sp->crews.list);
	return (rc);
}

static SPWAW_ERROR
snapint_oob_formations_stage2 (SPWAW_SNAP_OOB_FORCE *ptr)
{
	DWORD	i;

	CNULLARG (ptr);

	for (i=0; i<ptr->formations.cnt; i++) {
		SPWAW_SNAP_OOB_FEL_DATA		*dat = &(ptr->formations.list[i].data);
		SPWAW_SNAP_OOB_FEL_STRINGS	*str = &(ptr->formations.list[i].strings);

		// Prevent warning C4244
		FID2str ((BYTE)(dat->FID & 0xFF), str->name, sizeof (str->name));
		str->status	= fstatus2str (dat->status);
		str->fstatus	= (char*)SPWOOB_FSTAT_lookup (dat->fstatus);
		str->type	= (char*)SPWOOB_FTYPE_lookup (dat->type);
	}

	return (SPWERR_OK);
}

static inline void
snapint_oob_units_stage2_core (SPWAW_SNAP_OOB_UEL *ptr, STRTAB * /*stab*/)
{
	SPWAW_SNAP_OOB_UEL_DATA		*dat;
	SPWAW_SNAP_OOB_UEL_STRINGS	*str;

	if (!ptr) return;
	dat = &(ptr->data);
	str = &(ptr->strings);

	// Prevent warning C4244
	UID2str ((BYTE)(dat->FMID & 0xFF), (BYTE)(dat->FSID & 0xFF), str->uid, sizeof (str->uid));
	str->people	= (char *)SPWAW_oob_people ((BYTE)(dat->OOB & 0xFF));
	str->rank	= rank2str (dat->rank);
	str->exp	= exp2str (dat->eclass);
	str->status	= ustatus2str (dat->status);
	str->entr	= entr2str (dat->entr);
	str->contact	= cstatus2str (dat->contact);
	str->utype	= (char*)SPWOOB_UTYPE_lookup (dat->utype);
	str->uclass	= (char*)SPWOOB_UCLASS_lookup (dat->uclass);

	if (ptr->data.formation) ptr->core = ptr->data.formation->core;
}

static SPWAW_ERROR
snapint_oob_units_stage2 (SPWAW_SNAP_OOB_FORCE *ptr, STRTAB *stab)
{
	DWORD		i;

	CNULLARG (ptr);

	for (i=0; i<ptr->units.cnt; i++) {
		snapint_oob_units_stage2_core (&(ptr->units.list[i]), stab);
	}

	for (i=0; i<ptr->crews.cnt; i++) {
		snapint_oob_units_stage2_core (&(ptr->crews.list[i]), stab);
	}

	return (SPWERR_OK);
}

static SPWAW_ERROR
snapint_oob_core_stage2 (SPWAW_SNAP_OOB *ptr, STRTAB *stab)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	CNULLARG (ptr);

	rc = snapint_oob_formations_stage2 (&(ptr->battle));	ROE ("snapint_oob_formations_stage2(battle)");
	rc = snapint_oob_units_stage2 (&(ptr->battle), stab);	ROE ("snapint_oob_units_stage2(battle)");

	rc = snapint_oob_formations_stage2 (&(ptr->core));	ROE ("snapint_oob_formations_stage2(core)");
	rc = snapint_oob_units_stage2 (&(ptr->core), stab);	ROE ("snapint_oob_units_stage2(core)");

	rc = snapint_oob_formations_stage2 (&(ptr->support));	ROE ("snapint_oob_formations_stage2(support)");
	rc = snapint_oob_units_stage2 (&(ptr->support), stab);	ROE ("snapint_oob_units_stage2(support)");

	return (SPWERR_OK);
}

/* readiness calculation: max and actual values, calculated per-unit and summed.
 * A unit's readiness is influenced by:
 *    damage        more damage means less readiness
 *    headcount     less available effectives means less readiness
 *    suppression   more suppression means less readiness
 *    abandonment   means total loss of readiness (-100%)
 *    crew          means significant reduction of readiness (-90%)
 *    status        additional readiness reduction
 */
static double
calc_act_readiness (SPWAW_SNAP_OOB_UEL *ptr)
{
	double	rd, rh, r;
	int	ds;

	/* calculate damage */
	rd = (double)(ptr->data.cost - ptr->data.damage) / (double)ptr->data.cost;

	/* calculate headcount */
	rh = (double)ptr->data.hcnt_left / (double)ptr->data.hcnt;

	/* start with damage or headcount, whichever has the greatest effect */
	r = (rd < rh) ? rd : rh;

	/* reduction based on suppression vs morale */
	ds = ptr->data.mor - ptr->data.sup;
	if (ds <= 0)
		r = 0.0;
	else
		r *= (double)ds / (double)ptr->data.mor;

	/* reduction based on abandonment */
	switch (ptr->data.aband) {
		case SPWAW_ASTAY:
			r = 0.0;
			break;
		case SPWAW_ALEFT:
			r *= 0.1;
			break;
		default:
			break;
	}

	/* reduction based on status */
	switch (ptr->data.status) {
		case SPWAW_UPINNED:
			/* pinned:	 -30% readiness */
			r *= 0.7;
			break;
		case SPWAW_URETREATING:
			/* retreating:	 -50% readiness */
			r *= 0.5;
			break;
		case SPWAW_UROUTED:
			/* routed:	 -80% readiness */
			r *= 0.2;
			break;
		case SPWAW_UABANDONED:
			/* abandoned:	-100% readiness */
			r = 0.0;
			break;
		case SPWAW_UDESTROYED:
			/* destroyed:	-100% readiness */
			r = 0.0;
			break;
		case SPWAW_UBURNING:
			/* burning:	-100% readiness */
			r = 0.0;
			break;
		case SPWAW_UBUTTONED:
			/* buttoned:	 -20% readiness */
			r *= 0.8;
			break;
		case SPWAW_UIMMOBILIZED:
			/* immobilized:	 -75% readiness */
			r *= 0.25;
			break;
		default:
			break;
	}

	return (r);
}

static SPWAW_ERROR
snapint_oob_attrs (SPWAW_SNAP_OOB_FORCE *ptr)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	DWORD			i, j;
	SPWAW_SNAP_OOB_FEL	*fp;
	SPWAW_SNAP_OOB_UEL	*up;

	CNULLARG (ptr);

	/* Calculate attributes for each unit */
	for (i=0; i<ptr->units.cnt; i++) {
		up = &(ptr->units.list[i]);

		up->attr.gen.ready = calc_act_readiness (up);
		up->attr.gen.kills = up->data.kills;
		if (!up->data.alive) up->attr.gen.losses = 1;

		//log ("snapint_oob_attrs[%5.5u]: [%5.5u] %4.4s: F<%5.5u,%3.3u> (%16.16s) ",
		//	i, up->data.RID, up->data.type, up->data.FMID, up->data.FSID, up->data.name);
		//log ("type=%s alive=%d aband=%d aunit=%s losses=%d\n",
		//	up->strings.utype, up->data.alive,
		//	up->data.aband, up->data.aunit.up ? "ptr" : "nil",
		//	up->attr.gen.losses);

		/* Also allocate losses for crew members that were not recorded!
		 * (crew member killed and slot reused before save) */
		if ((up->data.aband == SPWAW_ASTAY) && !up->data.aunit.up) {
			//log ("  allocated loss for unrecorded KIA crew\n");
			up->attr.gen.losses++;
		}
	}

	/* No attributes for crewmen!
	 * Allocate some attributes with the abandoned unit. */
	for (i=0; i<ptr->crews.cnt; i++) {
		up = &(ptr->crews.list[i]);

		//log ("snapint_oob_attrs[%5.5u]: [%5.5u] %4.4s: F<%5.5u,%3.3u> (%16.16s) ",
		//	i, up->data.RID, up->data.type, up->data.FMID, up->data.FSID, up->data.name);
		//log ("type=%s alive=%d aband=%d aunit=%s losses=%d\n",
		//	up->strings.utype, up->data.alive,
		//	up->data.aband, up->data.aunit.up ? "ptr" : "nil",
		//	up->attr.gen.losses);

		//up->data.aunit.up->attr.gen.kills += up->data.kills;
		if (!up->data.alive) {
			//log ("  allocated loss for KIA crew\n");
			up->data.aunit.up->attr.gen.losses++;
		}

	}

	/* Calculate attributes for each formation */
	for (i=0; i<ptr->formations.cnt; i++) {
		fp = &(ptr->formations.list[i]);

		fp->attr.gen.ready = 0.0;
		for (j=0; j<fp->data.ucnt; j++) {
			up = fp->data.ulist[j];

			fp->attr.gen.ready  += up->attr.gen.ready;
			fp->attr.gen.kills  += up->attr.gen.kills;
			fp->attr.gen.losses += up->attr.gen.losses;

			snapint_addmmas (&(fp->attr.mmas_exp), up->data.exp, up->data.idx);
			snapint_addmmas (&(fp->attr.mmas_mor), up->data.mor, up->data.idx);
			snapint_addmmas (&(fp->attr.mmas_sup), up->data.sup, up->data.idx);
			snapint_addmmas (&(fp->attr.mmas_ral), up->data.ral, up->data.idx);
			snapint_addmmas (&(fp->attr.mmas_inf), up->data.inf, up->data.idx);
			snapint_addmmas (&(fp->attr.mmas_arm), up->data.arm, up->data.idx);
			snapint_addmmas (&(fp->attr.mmas_art), up->data.art, up->data.idx);
			snapint_addmmas (&(fp->attr.mmas_ready), up->attr.gen.ready * 100.0, up->data.idx);
			snapint_addmmas (&(fp->attr.mmas_kills), up->attr.gen.kills, up->data.idx);
		}
		fp->attr.gen.ready /= fp->data.ucnt;
	}

	/* Calculate attributes for entire force */
	ptr->attr.gen.ready = 0.0;
	for (i=0; i<ptr->units.cnt; i++) {
		up = &(ptr->units.list[i]);

		ptr->attr.gen.ready  += up->attr.gen.ready;
		ptr->attr.gen.kills  += up->attr.gen.kills;
		ptr->attr.gen.losses += up->attr.gen.losses;

		snapint_addmmas_set (&(ptr->attr.mmas_uexp), up->data.exp, up);
		snapint_addmmas_set (&(ptr->attr.mmas_umor), up->data.mor, up);
		snapint_addmmas_set (&(ptr->attr.mmas_usup), up->data.sup, up);
		snapint_addmmas_set (&(ptr->attr.mmas_ural), up->data.ral, up);
		snapint_addmmas_set (&(ptr->attr.mmas_uinf), up->data.inf, up);
		snapint_addmmas_set (&(ptr->attr.mmas_uarm), up->data.arm, up);
		snapint_addmmas_set (&(ptr->attr.mmas_uart), up->data.art, up);
		snapint_addmmas_set (&(ptr->attr.mmas_uready), up->attr.gen.ready * 100.0, up);
		snapint_addmmas_set (&(ptr->attr.mmas_ukills), up->attr.gen.kills, up);
		snapint_addmmas_set (&(ptr->attr.mmas_ucost), up->data.cost, up);
		snapint_addmmas_set (&(ptr->attr.mmas_uspeed), up->data.speed, up);
	}
	ptr->attr.gen.ready /= ptr->units.cnt;
	for (i=0; i<ptr->formations.cnt; i++) {
		fp = &(ptr->formations.list[i]);

		snapint_addmmas (&(ptr->attr.mmas_fready), fp->attr.gen.ready * 100.0, fp->data.idx);
		snapint_addmmas (&(ptr->attr.mmas_fkills), fp->attr.gen.kills, fp->data.idx);
		snapint_addmmas (&(ptr->attr.mmas_flosses), fp->attr.gen.losses, fp->data.idx);
	}

	return (rc);
}

static SPWAW_ERROR
snapint_oob_stats (SPWAW_SNAP_OOB_FORCE *ptr)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	DWORD			i, j;

	CNULLARG (ptr);

	memset (&(ptr->stats), 0, sizeof (ptr->stats));

	/* Calculate statistics for entire force */
	ptr->stats.ustats.cnt = ptr->units.cnt;
	for (i=0; i<ptr->units.cnt; i++) {
		SPWAW_SNAP_OOB_UEL *up = &(ptr->units.list[i]);

		if (up->data.alive) ptr->stats.ustats.cnt_left++;

		ptr->stats.ustats.rnk_cnt[up->data.rank]++;
		ptr->stats.ustats.exp_cnt[up->data.eclass]++;
		ptr->stats.ustats.cls_cnt[up->data.uclass]++;

		if (up->attr.gen.losses) {
			if (!up->data.alive) ptr->stats.ustats.rnk_loss[up->data.rank]   += up->attr.gen.losses;
			if (!up->data.alive) ptr->stats.ustats.exp_loss[up->data.eclass] += up->attr.gen.losses;
			if (!up->data.alive) ptr->stats.ustats.cls_loss[up->data.uclass] += up->attr.gen.losses;
		}

		// Prevent warning C4244
		ptr->stats.hcnt = ptr->stats.hcnt + (USHORT)(up->data.hcnt);
		ptr->stats.hcnt_left = ptr->stats.hcnt_left + (USHORT)up->data.hcnt_left;
	}

	ptr->stats.fstats.cnt = ptr->formations.cnt;
	for (i=0; i<ptr->formations.cnt; i++) {
		SPWAW_SNAP_OOB_FEL *fp = &(ptr->formations.list[i]);

		DWORD c = 0;
		for (j=0; j<fp->data.ucnt; j++) {
			//if (!fp->data.ulist[j]->data.lost) c++;
			if (fp->data.ulist[j]->data.alive) c++;
		}
		if (c) ptr->stats.fstats.cnt_left++;
	}

	return (rc);
}

static SPWAW_ERROR
snapint_oobp1 (SPWAW_SNAPSHOT *ptr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWOOB_DATA	*oobdata = NULL;
	STRTAB		*stab = NULL;

	CNULLARG (ptr);
	stab = (STRTAB *)ptr->stab;

	ptr->OOBp1.side   = 0;
	ptr->OOBp1.OOB    = ptr->game.battle.data.OOB_p1;
	ptr->OOBp1.nation = ptr->game.battle.strings.nation_p1;
	ptr->OOBp1.people = ptr->game.battle.strings.people_p1;

	// Prevent warning C4244
	oobdata = spwoob_data (ptr->oobdat, (BYTE)(ptr->OOBp1.OOB & 0xFF));
	if (!oobdata) RWE (SPWERR_BADOOB, "spwoob_data(OOBp1) failed");

	rc = snapint_oob_core_stage1 (&(ptr->raw.OOBp1), &(ptr->OOBp1), oobdata, stab);
	ROE ("snapint_oob_core_stage1(OOBp1)");

	qsort (ptr->OOBp1.battle.formations.list, ptr->OOBp1.battle.formations.cnt, sizeof (SPWAW_SNAP_OOB_FEL), sort_fcmp);
	qsort (ptr->OOBp1.battle.units.list, ptr->OOBp1.battle.units.cnt, sizeof (SPWAW_SNAP_OOB_UEL), sort_ucmp);
	qsort (ptr->OOBp1.battle.crews.list, ptr->OOBp1.battle.crews.cnt, sizeof (SPWAW_SNAP_OOB_UEL), sort_ucmp);

	rc = OOB_link (&(ptr->OOBp1), true);		ROE ("OOB_link(OOBp1)");
	rc = OOB_build_subforce (&(ptr->OOBp1.battle), &(ptr->OOBp1.core), true);	ROE ("OOB_build_subforce(OOBp1, core)");
	rc = OOB_build_subforce (&(ptr->OOBp1.battle), &(ptr->OOBp1.support), false);	ROE ("OOB_build_subforce(OOBp1, support)");

	rc = snapint_oob_core_stage2 (&(ptr->OOBp1), stab);
	ROE ("snapint_oob_core_stage2(OOBp1)");

	rc = snapint_oob_attrs (&(ptr->OOBp1.battle));	ROE ("snapint_oob_attrs(OOBp1.battle)");
	rc = snapint_oob_attrs (&(ptr->OOBp1.core));	ROE ("snapint_oob_attrs(OOBp1.core)");
	rc = snapint_oob_attrs (&(ptr->OOBp1.support));	ROE ("snapint_oob_attrs(OOBp1.support)");

	rc = snapint_oob_stats (&(ptr->OOBp1.battle));	ROE ("snapint_oob_stats(OOBp1.battle)");
	rc = snapint_oob_stats (&(ptr->OOBp1.core));	ROE ("snapint_oob_stats(OOBp1.core)");
	rc = snapint_oob_stats (&(ptr->OOBp1.support));	ROE ("snapint_oob_stats(OOBp1.support)");

	return (SPWERR_OK);
}

static SPWAW_ERROR
snapint_oobp2 (SPWAW_SNAPSHOT *ptr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWOOB_DATA	*oobdata = NULL;
	STRTAB		*stab = NULL;

	CNULLARG (ptr);
	stab = (STRTAB *)ptr->stab;

	ptr->OOBp2.side   = 1;
	ptr->OOBp2.OOB    = ptr->game.battle.data.OOB_p2;
	ptr->OOBp2.nation = ptr->game.battle.strings.nation_p2;
	ptr->OOBp2.people = ptr->game.battle.strings.people_p2;

	// Prevent warning C4244
	oobdata = spwoob_data (ptr->oobdat, (BYTE)(ptr->OOBp2.OOB & 0xFF));
	if (!oobdata) RWE (SPWERR_BADOOB, "spwoob_data(OOBp2) failed");

	rc = snapint_oob_core_stage1 (&(ptr->raw.OOBp2), &(ptr->OOBp2), oobdata, stab);
	ROE ("snapint_oob_core_stage1(OOBp2)");

	qsort (ptr->OOBp2.battle.formations.list, ptr->OOBp2.battle.formations.cnt, sizeof (SPWAW_SNAP_OOB_FEL), sort_fcmp);
	qsort (ptr->OOBp2.battle.units.list, ptr->OOBp2.battle.units.cnt, sizeof (SPWAW_SNAP_OOB_UEL), sort_ucmp);
	qsort (ptr->OOBp2.battle.crews.list, ptr->OOBp2.battle.crews.cnt, sizeof (SPWAW_SNAP_OOB_UEL), sort_ucmp);

	rc = OOB_link (&(ptr->OOBp2), false);		ROE ("OOB_link(OOBp2)");

	rc = snapint_oob_core_stage2 (&(ptr->OOBp2),stab);
	ROE ("snapint_oob_core_stage2(OOBp2)");

	rc = snapint_oob_attrs (&(ptr->OOBp2.battle));	ROE ("snapint_oob_attrs(OOBp2.battle)");

	rc = snapint_oob_stats (&(ptr->OOBp2.battle));	ROE ("snapint_oob_stats(OOBp2.battle)");

	return (SPWERR_OK);
}

SPWAW_ERROR
snapint (SPWAW_SNAPSHOT *ptr)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	CNULLARG (ptr);

	rc = snapint_game (ptr);	ROE ("snapint_game()");
	rc = snapint_oobp1 (ptr);	ROE ("snapint_oobp1()");
	rc = snapint_oobp2 (ptr);	ROE ("snapint_oobp2()");

	return (SPWERR_OK);
}
