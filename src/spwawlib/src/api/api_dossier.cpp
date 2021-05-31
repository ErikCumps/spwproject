/** \file
 * The SPWaW Library - dossier API implementation.
 *
 * Copyright (C) 2007-2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_dossier.h>
#include "dossier/dossier.h"
#include "spwoob/spwoob_list.h"
#include "common/internal.h"
#include "utils/filecheck.h"

SPWAWLIB_API SPWAW_ERROR
SPWAW_dossier_new (SPWAW_GAME_TYPE gametype, const char *oobdir, const char *name, const char *comment, SPWAW_DOSSIER **dossier)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_DOSSIER	*ptr = NULL;
	STRTAB		*stab = NULL;

	CSPWINIT;
	CNULLARG (oobdir); CNULLARG (name); CNULLARG (comment); CNULLARG (dossier);
	*dossier = NULL;

	/* Create dossier structure and string table */
	rc = dossier_new (&ptr);
	ERRORGOTO ("dossier_new()", handle_error);
	stab = (STRTAB *)ptr->stab;

	/* Set game type */
	ptr->gametype = gametype;

	/* Set name and comment */
	ptr->name = STRTAB_add (stab, (char *)name);
	ptr->comment = STRTAB_add (stab, (char *)comment);

	/* Validate and record OOB dir */
	rc = cfg_add_oobdir (oobdir, gametype);
	ERRORGOTO ("cfg_add_oobdir()", handle_error);
	ptr->oobdir = STRTAB_add (stab, (char *)oobdir);

	*dossier = ptr;
	return (SPWERR_OK);

handle_error:
	SPWAW_dossier_free (&ptr);
	return (rc);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dossier_info (const char *file, SPWAW_DOSSIER_INFO *info)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	int		fd;

	CSPWINIT;
	CNULLARG (file); CNULLARG (info);
	clear_ptr (info);

	/* Open file */
	fd = open (file, O_RDONLY|O_BINARY);
	if (fd < 0) FAILGOTO (SPWERR_FOFAILED, "dossier file open", handle_error);

	/* Verify file integrity */
	rc = fcheck_verify (fd);
	ERRORGOTO ("fcheck_verify()", handle_error);

	/* Load dossier info */
	rc = dossier_loadinfo (fd, info);
	ERRORGOTO ("dossier_loadinfo()", handle_error);

	/* Cleanup and return */
	close (fd); fd = -1;

	return (SPWERR_OK);

handle_error:
	if (fd >= 0) close (fd);
	return (rc);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dossier_load (const char *file, SPWAW_DOSSIER **dossier, SPWAW_DOSSIER_LOAD_CB *load_cb)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_DOSSIER	*ptr = NULL;
	int		fd = -1;

	CSPWINIT;
	CNULLARG (file); CNULLARG (dossier);
	*dossier = NULL;

	/* Create dossier structure and string table */
	rc = dossier_new (&ptr);
	ERRORGOTO ("dossier_new()", handle_error);

	/* Open file */
	fd = open (file, O_RDONLY|O_BINARY);
	if (fd < 0) FAILGOTO (SPWERR_FOFAILED, "dossier file open", handle_error);

	/* Verify file integrity */
	rc = fcheck_verify (fd);
	ERRORGOTO ("fcheck_verify()", handle_error);

	/* Load dossier data */
	rc = dossier_load (fd, ptr, load_cb);
	ERRORGOTO ("dossier_load()", handle_error);

	/* Cleanup and return */
	close (fd);

	*dossier = ptr;
	return (SPWERR_OK);

handle_error:
	if (fd >= 0) close (fd);
	SPWAW_dossier_free (&ptr);
	return (rc);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dossier_save (SPWAW_DOSSIER **dossier, const char *file, bool compress, SPWAW_DOSSIER_SAVE_CB *save_cb)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	char		tf[MAX_PATH+1];
	char		bf[MAX_PATH+1];
	int		fd;

	CSPWINIT;
	CNULLARG (dossier); CNULLARG (*dossier); CNULLARG (file);

	memset (tf, 0, sizeof (tf));
	snprintf (tf, sizeof (tf) - 1, "%s.save.", file);

	memset (bf, 0, sizeof (bf));
	snprintf (bf, sizeof (bf) - 1, "%s.backup", file);

	/* Open .save. file and save dossier data */
	fd = open (tf, O_RDWR|O_BINARY|O_CREAT|O_TRUNC, 0666);
	if (fd < 0) FAILGOTO (SPWERR_FOFAILED, "dossier .save. file create", handle_error);

	rc = dossier_save (*dossier, fd, compress, save_cb);
	ERRORGOTO ("dossier_save()", handle_error);

	rc = fcheck_make (fd);
	ERRORGOTO ("fcheck_make()", handle_error);

	close (fd);

	/* Safely replace original dossier file with .save. file */
	unlink (bf);
	if ((rename (file, bf) != 0) && (errno != ENOENT)) {
		FAILGOTO (SPWERR_FWFAILED, "safe dossier backup", handle_error);
	}
	if (rename (tf, file) != 0) FAILGOTO (SPWERR_FWFAILED, "safe dossier replace", handle_error);
	unlink (bf);

	return (SPWERR_OK);

handle_error:
	if (fd >= 0) close (fd);
	return (rc);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dossier_free (SPWAW_DOSSIER **dossier)
{
	SPWAW_DOSSIER	*ptr;

	CSPWINIT;
	CNULLARG (dossier);

	ptr = *dossier; *dossier = NULL;
	if (ptr) {
		dossier_clean (ptr);
		free (ptr);
	}

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dossier_edit (SPWAW_DOSSIER *dossier, const char *name, const char *comment)
{
	STRTAB	*stab = NULL;

	CSPWINIT;
	CNULLARG (dossier);

	stab = (STRTAB *)dossier->stab;

	/* Update name and/or comment */
	if (name) {
		STRTAB_del (stab, dossier->name);
		dossier->name = STRTAB_add (stab, (char *)name);
	}
	if (comment) {
		STRTAB_del (stab, dossier->comment);
		dossier->comment = STRTAB_add (stab, (char *)comment);
	}

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dossier_set_savedir (SPWAW_DOSSIER *dossier, const char *savedir)
{
	STRTAB	*stab = NULL;

	CSPWINIT;
	CNULLARG (dossier);

	stab = (STRTAB *)dossier->stab;

	STRTAB_del (stab, dossier->savedir);
	dossier->savedir = STRTAB_add (stab, (char *)savedir);

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dossier_add_campaign_snap (SPWAW_DOSSIER *dossier, SPWAW_SNAPSHOT *snap, SPWAW_BTURN **bturn)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_BTURN	*p = NULL;

	CSPWINIT;
	CNULLARG (dossier); CNULLARG (snap);
	if (bturn) *bturn = NULL;

	/* Apply snapshot type eligibility rules */
	if (snap->type == SPWAW_STDALONE_BATTLE) {
		RWE (SPWERR_BADBTYPE, "this snapshot does not allow campaign tracking");
	}

	/* Apply snapshot compatibility rules */
	if (dossier->gametype != snap->gametype) {
		ERROR2 ("dossier gametype \"%s\" != snapshot gametype \"%s\"",
			SPWAW_gametype2str (dossier->gametype), SPWAW_gametype2str (snap->gametype));
		rc = SPWERR_NOMATCH_GTYPE;
	} else if (dossier->bcnt != 0) {
		if (!HASERROR) {
			if (snap->game.battle.data.OOB_p1 != dossier->props.OOB) {
				ERROR2 ("dossier OOB (%d) != snapshot OOB (%d)", dossier->props.OOB, snap->game.battle.data.OOB_p1);
				rc = SPWERR_NOMATCH_OOB;
			}
		}
		if (!HASERROR) {
			if (snap->OOBp1.core.formations.cnt != dossier->props.ifcnt) {
				ERROR2 ("dossier initial formation count (%d) != snapshot core formations count (%d)", dossier->props.ifcnt, snap->OOBp1.core.formations.cnt);
				// However, this is not an error for winSPWW2 or winSPMBT.
				switch (dossier->gametype) {
					case SPWAW_GAME_TYPE_WINSPWW2:
					case SPWAW_GAME_TYPE_WINSPMBT:
						break;
					default:
						rc = SPWERR_NOMATCH_CORECNT;
						break;
				}
			}
		}
		if (!HASERROR) {
			if (snap->OOBp1.core.units.cnt != dossier->props.iucnt) {
				ERROR2 ("dossier initial unit count (%d) != snapshot core units count (%d)", dossier->props.iucnt, snap->OOBp1.core.units.cnt);
				// However, this is not an error for winSPWW2 or winSPMBT.
				switch (dossier->gametype) {
					case SPWAW_GAME_TYPE_WINSPWW2:
					case SPWAW_GAME_TYPE_WINSPMBT:
						break;
					default:
						rc = SPWERR_NOMATCH_CORECNT;
						break;
				}
			}
		}
		if (!HASERROR) {
			SPWAW_TIMESTAMP	tsd, tss;
			SPWAW_date2stamp (&(dossier->props.start), &tsd);
			SPWAW_date2stamp (&(snap->game.campaign.data.start), &tss);
			if (tss != tsd) {
				char ddate[32];
				char sdate[32];
				SPWAW_date2str (&(dossier->props.start), ddate, sizeof(ddate));
				SPWAW_date2str (&(snap->game.campaign.data.start), sdate, sizeof(sdate));
				ERROR2 ("dossier campaign start date \"%s\" != snapshot campaign start date \"%s\"", ddate, sdate);
				rc = SPWERR_NOMATCH_CSDATE;
			}
		}
		if (!HASERROR) {
			SPWAW_TIMESTAMP	tsd, tss;
			SPWAW_date2stamp (&(dossier->props.end), &tsd);
			SPWAW_date2stamp (&(snap->game.campaign.data.end), &tss);
			if (tss != tsd) {
				char ddate[32];
				char sdate[32];
				SPWAW_date2str (&(dossier->props.end), ddate, sizeof(ddate));
				SPWAW_date2str (&(snap->game.campaign.data.end), sdate, sizeof(sdate));
				ERROR2 ("dossier campaign end date \"%s\" != snapshot campaign end date \"%s\"", ddate, sdate);
				rc = SPWERR_NOMATCH_CEDATE;
			}
		}
		if (!HASERROR) {
			if (snap->game.campaign.data.battles_max != dossier->props.maxbtlcnt) {
				ERROR2 ("dossier campaign max battle count (%d) != snapshot campaign max battle count (%d)", dossier->props.maxbtlcnt, snap->game.campaign.data.battles_max);
				rc = SPWERR_NOMATCH_CMBCNT;
			}
		}
	}
	ROE ("snapshot compatibility verification");

	/* Set dossier type (if not already set) */
	if (dossier->type == SPWAW_EMPTY_DOSSIER) {
		if ((snap->gametype == SPWAW_GAME_TYPE_SPWAW) && (snap->savetype == SPWAW_SAVE_TYPE_MEGACAM)) {
			dossier->type = SPWAW_MEGACAM_DOSSIER;
		} else {
			dossier->type = SPWAW_CAMPAIGN_DOSSIER;
		}
	}

	rc = dossier_add_to_campaign (dossier, snap, &p);
	ROE ("dossier_add()");

	if (bturn) *bturn = p;

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dossier_add_battle (SPWAW_DOSSIER *dossier, SPWAW_SNAPSHOT *snap, const char *name, SPWAW_BATTLE **battle)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	CSPWINIT;
	CNULLARG (dossier); CNULLARG (snap);
	if (battle) *battle = NULL;

	/* Apply snapshot compatibility rules */
	if (dossier->gametype != snap->gametype) {
		ERROR2 ("dossier gametype \"%s\" != snapshot gametype \"%s\"",
			SPWAW_gametype2str (dossier->gametype), SPWAW_gametype2str (snap->gametype));
		return (SPWERR_NOMATCH_GTYPE);
	}

	/* Set dossier type (if not already set) */
	if (dossier->type == SPWAW_EMPTY_DOSSIER) {
		dossier->type = SPWAW_STDALONE_DOSSIER;
	}

	rc = dossier_add_new_battle (dossier, snap, name, battle);

	return (rc);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dossier_add_battle_snap (SPWAW_BATTLE *battle, SPWAW_SNAPSHOT *snap, SPWAW_BTURN **bturn)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_BTURN	*p = NULL;

	CSPWINIT;
	CNULLARG (battle); CNULLARG (snap);
	if (bturn) *bturn = NULL;

	/* Apply snapshot compatibility rules */
	if (battle->dossier->gametype != snap->gametype) {
		ERROR2 ("dossier gametype \"%s\" != snapshot gametype \"%s\"",
			SPWAW_gametype2str (battle->dossier->gametype), SPWAW_gametype2str (snap->gametype));
		return (SPWERR_NOMATCH_GTYPE);
	}

	/* Apply battle compatibility rules */
	if (battle->tcnt != 0) {
		if (!HASERROR) {
			if (snap->game.battle.data.OOB_p1 != battle->OOB_p1) {
				ERROR2 ("battle player OOB (%d) != snapshot player OOB (%d)", battle->OOB_p1, snap->game.battle.data.OOB_p1);
				rc = SPWERR_NOMATCH_OOB;
			}
		}
		if (!HASERROR) {
			if (snap->game.battle.data.OOB_p2 != battle->OOB_p2) {
				ERROR2 ("battle opponent OOB (%d) != snapshot opponent OOB (%d)", battle->OOB_p2, snap->game.battle.data.OOB_p2);
				rc = SPWERR_NOMATCH_OOB;
			}
		}
		if (!HASERROR) {
			bool		nomatch = false;
			SPWAW_TIMESTAMP	tsb, tss;

			SPWAW_date2stamp (&(battle->bdate.date), &tsb);
			SPWAW_date2stamp (&(snap->game.battle.data.tdate.date), &tss);

			if (SPWAW_isMonthOnlyDate (&(battle->bdate.date)) && SPWAW_isMonthOnlyDate(&(snap->game.battle.data.tdate.date))) {
				nomatch = (tss != tsb);
			} else {
				nomatch = ((tss - (snap->game.battle.data.tdate.turn * SPWAW_MINSPERTURN)) != tsb);
			}
			if (nomatch) {
				char bdate[32];
				char sdate[32];
				SPWAW_date2str (&(battle->bdate.date), bdate, sizeof(bdate));
				SPWAW_date2str (&(snap->game.battle.data.tdate.date), sdate, sizeof(sdate));
				ERROR2 ("battle date \"%s\" != snapshot date \"%s\"", bdate, sdate);
				rc = SPWERR_NOMATCH_DATE;
			}
		}
		if (!HASERROR) {
			if (strcmp(snap->game.battle.data.location, battle->location_data) != 0) {
				ERROR2 ("battle location data \"%s\" != snapshot location \"%s\"", battle->location_data, snap->game.battle.data.location);
				rc = SPWERR_NOMATCH_LOCATION;
			}
		}
		if (!HASERROR) {
			if (strcmp(SPWAW_mission2str(snap->game.battle.data.miss_p1), battle->miss_p1) != 0) {
				ERROR2 ("battle player mission type \"%s\" != snapshot player mission type \"%s\"", battle->miss_p1, SPWAW_mission2str(snap->game.battle.data.miss_p1));
				rc = SPWERR_NOMATCH_MISSION;
			}
		}
		if (!HASERROR) {
			if (strcmp(SPWAW_mission2str(snap->game.battle.data.miss_p2), battle->miss_p2) != 0) {
				ERROR2 ("battle opponent mission type \"%s\" != snapshot opponent mission type \"%s\"", battle->miss_p2, SPWAW_mission2str(snap->game.battle.data.miss_p2));
				rc = SPWERR_NOMATCH_MISSION;
			}
		}
		if (!HASERROR) {
			if (battle->tfirst->info.pbir_battle.fcnt != snap->OOBp1.battle.formations.cnt) {
				ERROR2 ("battle player fcnt (%d) != snapshot player fcnt (%d)", battle->tfirst->info.pbir_battle.fcnt, snap->OOBp1.battle.formations.cnt);
				rc = SPWERR_NOMATCH_UFCNT;
			}
		}
		if (!HASERROR) {
			if (battle->tfirst->info.pbir_battle.ucnt != snap->OOBp1.battle.units.cnt) {
				ERROR2 ("battle player ucnt (%d) != snapshot player ucnt (%d)", battle->tfirst->info.pbir_battle.ucnt, snap->OOBp1.battle.units.cnt);
				rc = SPWERR_NOMATCH_UFCNT;
			}
		}
		if (!HASERROR) {
			if (battle->tfirst->info.obir_battle.fcnt != snap->OOBp2.battle.formations.cnt) {
				ERROR2 ("battle opponent fcnt (%d) != snapshot opponent fcnt (%d)", battle->tfirst->info.obir_battle.fcnt, snap->OOBp2.battle.formations.cnt);
				rc = SPWERR_NOMATCH_UFCNT;
			}
		}
		if (!HASERROR) {
			if (battle->tfirst->info.obir_battle.ucnt != snap->OOBp2.battle.units.cnt) {
				ERROR2 ("battle opponent ucnt (%d) != snapshot opponent ucnt (%d)", battle->tfirst->info.obir_battle.ucnt, snap->OOBp2.battle.units.cnt);
				rc = SPWERR_NOMATCH_UFCNT;
			}
		}

	}
	ROE ("snapshot compatibility verification");

	rc = dossier_add_to_battle (battle, snap, &p);
	ROE ("dossier_add_to_battle()");

	if (bturn) *bturn = p;

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dossier_del (SPWAW_DOSSIER *dossier, SPWAW_BTURN *bturn)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	CSPWINIT;
	CNULLARG (dossier); CNULLARG (bturn);

	rc = dossier_del (dossier, bturn);
	ROE ("dossier_del()");

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dossier_del (SPWAW_DOSSIER *dossier, SPWAW_BATTLE *battle)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	CSPWINIT;
	CNULLARG (dossier); CNULLARG (battle);

	rc = dossier_del (dossier, battle);
	ROE ("dossier_del()");

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dossier_savelist (SPWAW_DOSSIER *dossier, SPWAW_SAVELIST **list)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	CSPWINIT;
	CNULLARG (dossier); CNULLARG (list);

	rc = dossier_savelist (dossier, list);
	ROE ("dossier_savelist()");

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dossier_snaplist (SPWAW_DOSSIER *dossier, SPWAW_SNAPLIST **list)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	CSPWINIT;
	CNULLARG (dossier); CNULLARG (list);

	rc = dossier_snaplist (dossier, list);
	ROE ("dossier_snaplist()");

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dossier_find_battle (SPWAW_DOSSIER *dossier, SPWAW_BATTLE_DATE *bdate, SPWAW_BATTLE **battle)
{
	CSPWINIT;
	CNULLARG (dossier); CNULLARG (bdate); CNULLARG (battle);

	*battle = dossier_find_battle (dossier, bdate);

	return (SPWERR_OK);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_battle_set_location (SPWAW_BATTLE *battle, const char *location)
{
	STRTAB	*stab = NULL;

	CSPWINIT;
	CNULLARG (battle);

	stab = (STRTAB *)battle->dossier->stab;

	STRTAB_del (stab, battle->location);
	battle->location = STRTAB_add (stab, (char *)location);

	return (SPWERR_OK);
}
