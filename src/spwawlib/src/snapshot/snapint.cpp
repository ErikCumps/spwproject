/** \file
 * The SPWaW Library - snapshot handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "snapshot/index.h"
#include "snapshot/translate.h"
#include "snapshot/snapshot.h"
//#include "utils/log.h"
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

		if (src->name == NULL) {
			if (src->FID == 0) {
				memset (buf, 0, sizeof (buf));
				snprintf (buf, sizeof (buf) - 1, "%s HQ", ptr->people);
				src->name = STRTAB_add (stab, buf);
			} else {
				//DEBUG
				src->name = azstrstab (oob->fdata[src->OOBrid].name, stab);
				//ERROR2 ("unexpected NULL value for raw formation name (side=%d, FID=%u)\n", ptr->side, src->FID);
				//RWE (SPWERR_BADSAVEDATA, "invalid raw formation name");
				//DEBUG
			}
		}

		dat->idx	= i;
		dat->RID	= src->RID;
		dat->FID	= src->FID;
		dat->OOBrid	= src->OOBrid;
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

/* Unit leader and crew linkage determination:
 *
 * <unit has crew?>
 * 	NO : <unit has leader?>
 * 		YES: leader found
 * 		NO : use unit #id as leader #id
 * 	YES: <crew has unit?>
 * 		NO : INVALID UNIT!
 * 		YES: <crew unit == unit?>
 * 			YES: <crew has no leader?>
 * 				NO : leader found
 * 				YES: [GOTO <unit has no leader?>]
 * 			NO : [GOTO <unit has no leader?>]
 */
static inline bool
determine_ldrcrw (SPWAW_SNAP_OOB_URAW *ptr, int i, USHORT &ldr, USHORT &crw, bool &iscrew)
{
	SPWAW_SNAP_OOB_UELRAW	*uptr, *cptr = NULL;
	DWORD			idx;
	USHORT			lidx = SPWAW_BADIDX;

	ldr = SPWAW_BADIDX;
	crw = SPWAW_BADIDX;
	iscrew = false;

	if (!ptr) return (false);

	uptr = &(ptr->raw[i]);
	if ((uptr->crew != SPWAW_BADIDX) && ((idx = uridx (ptr, uptr->crew)) != SPWAW_BADIDX)) {
		//log ("ldrcrw[%u] ID=%4.4x, crew=%4.4x\n", i, uptr->ID, uptr->crew);
		cptr = &(ptr->raw[idx]);
		//log ("ldrcrw[%u] crew=%4.4x, unit=%4.4x\n", i, cptr->ID, cptr->crew);
		if (cptr->crew == SPWAW_BADIDX) return (false);
		if (cptr->crew == uptr->RID) {
			lidx = cptr->leader;
		}
	}
	//log ("ldrcrw[%u] lidx=%4.4x", i, lidx);
	if (lidx == SPWAW_BADIDX) lidx = uptr->leader;
	//log_nots (", %4.4x", lidx);
	if (lidx == SPWAW_BADIDX) lidx = uptr->RID;
	//log_nots (", %4.4x\n", lidx);

	ldr = lidx;
	crw = (cptr?cptr->RID:SPWAW_BADIDX);
	iscrew = (cptr && (cptr->RID < uptr->RID));

	//if (iscrew) log ("ldrcrw[%u] ldr=%4.4x, crw=%4.4x, iscrew=%u\n", i, ldr, crw, iscrew);

	return (true);
}

static inline void
abandoned (SPWAW_SNAP_OOB_UELRAW *ptr, USHORT cidx, SPWAW_ABAND *flag, SPWAW_SNAP_OOB_PTR *id)
{
	if (flag) *flag = SPWAW_ANONE;
	if (id) id->rid = SPWAW_BADIDX;
	if (!flag || !id) return;

	if (cidx != SPWAW_BADIDX) {
		if ((DWORD)cidx > ptr->RID) {
			*flag = SPWAW_ASTAY;
		} else {
			*flag = SPWAW_ALEFT;
		}
		id->rid = cidx;
	}
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
		if (!determine_ldrcrw (&(raw->units), i, ldridx, crwidx, iscrew)) continue;
		if (iscrew) p->crews.cnt++; else p->units.cnt++;
	}

	p->units.list = safe_nmalloc(SPWAW_SNAP_OOB_UEL, p->units.cnt);
	COOMGOTO (p->units.list, "SPWAW_SNAP_OOB_UEL list", handle_error);

	p->crews.list = safe_nmalloc(SPWAW_SNAP_OOB_UEL, p->crews.cnt);
	COOMGOTO (p->crews.list, "SPWAW_SNAP_OOB_UEL list", handle_error);

	uidx = cidx = 0;
	for (i=0; i<(int)raw->units.cnt; i++) {
		SPWAW_SNAP_OOB_UEL_STRINGS	*str;
		SPWAW_SNAP_OOB_UEL_DATA		*dat;

		SPWAW_SNAP_OOB_UELRAW *src = &(raw->units.raw[i]);
		if (!determine_ldrcrw (&(raw->units), i, ldridx, crwidx, iscrew)) continue;
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
		dat->crew	= (src->crew != SPWAW_BADIDX);
		abandoned (src, crwidx, &(dat->aband), &(dat->aunit));
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
		dat->radio	= src->radio;
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

	/* OOB force leader */
	bp->leader = &(bp->units.list[0]);

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
			p.up->data.aband = SPWAW_ANONE;
		}
	}
	for (i=0; i<bp->crews.cnt; i++) {
		p.up = &(bp->crews.list[i]);
		if (p.up->data.aband != SPWAW_ALEFT) abort();

		p.up->data.aunit.up = unitbyid (p.up->data.aunit.rid, bp);
		if (!p.up->data.aunit.up) abort();
		p.up->data.aunit.up->data.aband = SPWAW_ASTAY;
		p.up->data.aunit.up->data.aunit.up = p.up;
	}

	/* Fix up unit crew linkage data */
	for (i=0; i<bp->units.cnt; i++) {
		p.up = &(bp->units.list[i]);

		/* Check crew linkage */
		if (p.up->data.aband != SPWAW_ANONE) {
			if (p.up->data.status == SPWAW_UABANDONED) {
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

static BYTE
find_oobid_by_name (SPWAW_SNAP_OOB_FEL *ptr, SPWOOB_DATA *oob, SPWAW_DATE *date)
{
	BYTE			rv = 0, i;
	int			score = -100;
	int			s, j, k, c, l;

	//log ("\t[NAME] OOBID SEARCH FOR: F#%d, utype <%s>, oob %d, date %d/%d\n",
	//	ptr->data.ID, ptr->strings.utype, oob->id, date->year, date->month);
	//for (i=0; i<ptr->data.ucnt; i++) {
	//	log ("\tunit[%d] #%d (%d_%d) OOBid %d <%s>\n", i, ptr->data.ulist[i]->data.ID,
	//		ptr->data.ulist[i]->data.FID, ptr->data.ulist[i]->data.FSID,
	//		ptr->data.ulist[i]->data.OOBid, oob->udata[ptr->data.ulist[i]->data.OOBid].name);
	//}

	for (i=0; i<SPWOOB_FCNT; i++) {
		if (!oob->fdata[i].valid) continue;

		if (strcmp (ptr->strings.utype, oob->fdata[i].name) == 0) {
			/* formation type name match is 1 point */
			s = 1;

			//log ("\tOOB f[%d] <%s> from=%d/%d to=%d\n", i,
			//	oob->fdata[i].name, oob->fdata[i].start_yr, oob->fdata[i].start_mo, oob->fdata[i].end_yr);

			/* formation availability period match is 2 extra points */
			if (	(oob->fdata[i].start_yr <= date->year)	&&
				(oob->fdata[i].start_mo <= date->month)	&&
				(date->year <= oob->fdata[i].end_yr)	)
			{
				s += 2;
			}

			j = k = 0; while ((k<ptr->data.ucnt) && (j<SPWOOB_FMUCNT)) {
				c = oob->fdata[i].unit_cnt[j];
				l = oob->fdata[i].unit_ids[j];
				if (!c) { j++; continue; }

				if (l<1000) {
					//log ("\t\tOOB f[%d].unit[%d] {UNIT} id=%d cnt=%d <%s> from=%d/%d to=%d\n",
					//	i, j, l, c, oob->udata[l].name,
					//	oob->udata[l].start_yr, oob->udata[l].start_mo, oob->udata[l].end_yr);

					while (c--) {
						if (strcmp (oob->udata[ptr->data.ulist[k]->data.OOBrid].name, oob->udata[l].name) == 0) {
							/* Formation unit name match is 1 extra point */
							s += 1; k++;
							/* Formation unit availability period match is 1 extra point */
							if (	(oob->udata[l].start_yr <= date->year)	&&
								(oob->udata[l].start_mo <= date->month)	&&
								(date->year <= oob->udata[l].end_yr)	)
							{
								s += 1;
							}
						}
					}
				} else {
					//log ("\t\tOOB f[%d].unit[%d] {FORM} id=%d cnt=%d <%s> from=%d/%d to=%d\n",
					//	i, j, l, c, oob->fdata[l-1000].name,
					//	oob->fdata[l-1000].start_yr, oob->fdata[l-1000].start_mo, oob->fdata[l-1000].end_yr);

					/* Formation formation availability period match failure deducts 1 extra point */
					if (!(	(oob->fdata[l-1000].start_yr <= date->year)	&&
						(oob->fdata[l-1000].start_mo <= date->month)	&&
						(date->year <= oob->fdata[l-1000].end_yr)	))
					{
						s -= 1;
					}
				}
				j++;
			}
			//log ("\t\t---\n");
			//log ("\tSuggested match: %d <%s>, score: %d\n", i, oob->fdata[i].name, s);
			if (s > score) {
				//log ("\tSelected match: current best score %d (previous %d)\n", s, score);
				rv = i;
				score = s;
			}
		}
	}
	return (rv);
}

static BYTE
find_formation_oobid (SPWAW_SNAP_OOB_FEL *ptr, SPWOOB_DATA *oob, SPWAW_DATE *date)
{
	BYTE			rv = 0;
	SPWAW_SNAP_OOB_FEL	*fp;
	int			sfpos, i, cnt, id;

//	log (">>> OOBID LOOKUP: F#%d, utype <%s>, oob %d, date %d/%d\n",
//		ptr->data.ID, ptr->strings.utype, oob->id, date->year, date->month);

	fp = ptr->data.hcmd.up->data.formation;
//	log ("  Higher command formation: F#%d, OOBid %d, utype <%s>\n", fp->data.FID, fp->data.OOBid, fp->strings.utype);

	sfpos = ptr->data.RID - fp->data.RID;
//	log ("  Position in higher formation: %d\n", sfpos);

	i = 0; id = 1000;
	while (i < SPWOOB_FMUCNT) {
		cnt = oob->fdata[fp->data.OOBrid].unit_cnt[i];
		id  = oob->fdata[fp->data.OOBrid].unit_ids[i];
		//if (id<1000) {
		//	log ("\tsfpos=%d | OOB f[%d].unit[%d] {UNIT} id=%d cnt=%d <%s> from=%d/%d to=%d\n",
		//		sfpos, fp->data.OOBrid, i, id, cnt, oob->udata[id].name,
		//		oob->udata[id].start_yr, oob->udata[id].start_mo, oob->udata[id].end_yr);
		//} else {
		//	log ("\tsfpos=%d | OOB f[%d].unit[%d] {FORM} id=%d cnt=%d <%s> from=%d/%d to=%d\n",
		//		sfpos, fp->data.OOBrid, i, id, cnt, oob->fdata[id-1000].name,
		//		oob->fdata[id-1000].start_yr, oob->fdata[id-1000].start_mo, oob->fdata[id-1000].end_yr);
		//}

		if (id > 1000) {
			while (cnt && (sfpos > 1)) { sfpos--; cnt--; }
			if (cnt && (sfpos == 1)) {
				//log ("\tFOUND formation at index %d!\n", i);
				break;
			}
		}
		i++;
	}
	// Prevent warning C4244
	if (i < 10) rv = (BYTE)((id - 1000) & 0xFF);
	if (!rv) rv = find_oobid_by_name (ptr, oob, date);

	//log ("<<< rv=%d\n\n", rv);

	return (rv);
}

static SPWAW_ERROR
snapint_oob_formations_stage2 (SPWAW_SNAP_OOB_FORCE *ptr, SPWOOB_DATA *oob, STRTAB * /*stab*/, SPWAW_DATE *date)
{
	DWORD	i;

	CNULLARG (ptr);

	for (i=0; i<ptr->formations.cnt; i++) {
		SPWAW_SNAP_OOB_FEL_DATA		*dat = &(ptr->formations.list[i].data);
		SPWAW_SNAP_OOB_FEL_STRINGS	*str = &(ptr->formations.list[i].strings);

		if (!dat->OOBrid) dat->OOBrid = find_formation_oobid (&(ptr->formations.list[i]), oob, date);

		dat->fstatus	= oob->fdata[dat->OOBrid].stat;
		dat->type	= oob->fdata[dat->OOBrid].type;

		// Prevent warning C4244
		FID2str ((BYTE)(dat->FID & 0xFF), str->name, sizeof (str->name));
		str->status	= fstatus2str (dat->status);
		str->fstatus	= (char*)SPWOOB_FSTAT_lookup (dat->fstatus);
		str->type	= (char*)SPWOOB_FTYPE_lookup (dat->type);

	}

	////DEBUG
	//log ("### checking OOB record ID lookup system\n");
	//BYTE	t;
	//for (i=0; i<ptr->formations.cnt; i++) {
	//	dat = &(ptr->formations.list[i].data);
	//	str = &(ptr->formations.list[i].strings);

	//	log ("record #%d/formation #%d/OOBid %d\n", dat->ID, dat->FID, dat->OOBid);
	//	log ("name <%s> type <%s> utype <%s>\n", str->name, str->type, str->utype);
	//	t = find_oobid_by_name (&(ptr->formations.list[i]), oob, date);
	//	if (t == dat->OOBid) {
	//		log ("+++ OK: OOBid [%d] == %d\n", dat->OOBid, t);
	//	} else {
	//		log ("+++ FAILED: OOBid [%d] != %d\n", dat->OOBid, t);
	//	}
	//}
	//log ("### check finished\n\n");
	////DEBUG

	return (SPWERR_OK);
}

static inline void
snapint_oob_units_stage2_core (SPWAW_SNAP_OOB_UEL *ptr, STRTAB * /*stab*/)
{
	SPWAW_SNAP_OOB_UEL_DATA	*dat;
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
snapint_oob_core_stage2 (SPWAW_SNAP_OOB *ptr, SPWOOB_DATA *oob, STRTAB *stab, SPWAW_DATE *date)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	CNULLARG (ptr);

	rc = snapint_oob_formations_stage2 (&(ptr->battle), oob, stab, date);	ROE ("snapint_oob_formations_stage2(battle)");
	rc = snapint_oob_units_stage2 (&(ptr->battle), stab);			ROE ("snapint_oob_units_stage2(battle)");

	rc = snapint_oob_formations_stage2 (&(ptr->core), oob, stab, date);	ROE ("snapint_oob_formations_stage2(core)");
	rc = snapint_oob_units_stage2 (&(ptr->core), stab);			ROE ("snapint_oob_units_stage2(core)");

	rc = snapint_oob_formations_stage2 (&(ptr->support), oob, stab, date);	ROE ("snapint_oob_formations_stage2(support)");
	rc = snapint_oob_units_stage2 (&(ptr->support), stab);			ROE ("snapint_oob_units_stage2(support)");

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

		/* Also allocate losses for crew members that were not recorded!
		 * (crew member killed and slot reused before save) */
		if (up->data.crew && !up->data.aunit.up) up->attr.gen.losses++;
	}

	/* No attributes for crewmen!
	 * Allocate some attributes with the abandoned unit. */
	for (i=0; i<ptr->crews.cnt; i++) {
		up = &(ptr->crews.list[i]);

		//up->data.aunit.up->attr.gen.kills += up->data.kills;
		if (!up->data.alive) up->data.aunit.up->attr.gen.losses++;
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

	rc = snapint_oob_core_stage2 (&(ptr->OOBp1), oobdata, stab, &(ptr->game.battle.data.date));
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

	rc = snapint_oob_core_stage2 (&(ptr->OOBp2), oobdata, stab, &(ptr->game.battle.data.date));
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
