/** \file
 * The SPWaW Library - dossier API implementation.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
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
SPWAW_dossier_new (const char *name, const char *comment, SPWAW_DOSSIER **dossier)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_DOSSIER	*ptr = NULL;
	STRTAB		*stab = NULL;

	CSPWINIT;
	CNULLARG (name); CNULLARG (comment); CNULLARG (dossier);
	*dossier = NULL;

	/* Create dossier structure and string table */
	rc = dossier_new (&ptr);
	ERRORGOTO ("dossier_new()", handle_error);
	stab = (STRTAB *)ptr->stab;

	/* Set name and comment */
	ptr->name = STRTAB_add (stab, (char *)name);
	ptr->comment = STRTAB_add (stab, (char *)comment);

	/* Record original OOB dir */
	ptr->oobdir = STRTAB_add (stab, cfg.oobdir);

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
SPWAW_dossier_load (const char *file, SPWAW_DOSSIER **dossier)
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
	rc = dossier_load (fd, ptr);
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
SPWAW_dossier_save (SPWAW_DOSSIER **dossier, const char *file, bool compress)
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

	rc = dossier_save (*dossier, fd, compress);
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
SPWAW_dossier_export (const char *file, const char *export)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	int		src = -1;
	FILE		*dst = NULL;

	CSPWINIT;
	CNULLARG (file); CNULLARG (export);

	/* Open files and export dossier data */
	src = open (file, O_RDONLY|O_BINARY); if (src < 0) rc = SPWERR_FOFAILED;
	ERRORGOTO ("dossier file open", handle_error);

	dst = fopen (export, "w"); if (!dst) rc = SPWERR_FOFAILED;
	ERRORGOTO ("export file open", handle_error);

	rc = dossier_export (src, dst);
	ERRORGOTO ("dossier_export()", handle_error);

	/* Cleanup and return */
	close (src); fclose (dst);

	return (SPWERR_OK);

handle_error:
	if (src >= 0) close (src);
	if (dst) fclose (dst);
	return (rc);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_dossier_import (const char *import, const char *file)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	FILE		*src = NULL;
	int		dst = -1;

	CSPWINIT;
	CNULLARG (import); CNULLARG (file);

	/* Open files and export dossier data */
	src = fopen (import, "r"); if (!src) rc = SPWERR_FOFAILED;
	ERRORGOTO ("import file open", handle_error);

	dst = open (file, O_CREAT|O_TRUNC|O_WRONLY|O_BINARY, 0666); if (dst < 0) rc = SPWERR_FOFAILED;
	ERRORGOTO ("dossier file open", handle_error);

	rc = dossier_import (src, dst);
	ERRORGOTO ("dossier_import()", handle_error);

	/* Cleanup and return */
	fclose (src); close (dst);

	return (SPWERR_OK);

handle_error:
	if (src) fclose (src);
	if (dst >= 0) close (dst);
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
SPWAW_dossier_add_campaign_snap (SPWAW_DOSSIER *dossier, SPWAW_SNAPSHOT *snap, SPWAW_BTURN **bturn)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_BTURN	*p = NULL;

	CSPWINIT;
	CNULLARG (dossier); CNULLARG (snap);
	if (bturn) *bturn = NULL;

	/* Apply snapshot type eligibility rules */
	if (snap->type != SPWAW_CAMPAIGN_BATTLE) {
		RWE (SPWERR_BADBTYPE, "this snapshot does not allow campaign tracking");
	}

	/* Apply dossier type compatibility rules */
	if (dossier->bcnt != 0) {
		if (!HASERROR) {
			if (snap->game.battle.data.OOB_p1 != dossier->props.OOB) {
				ERROR2 ("dossier OOB (%d) != snapshot OOB (%d)", dossier->props.OOB, snap->game.battle.data.OOB_p1);
				rc = SPWERR_NOMATCH_OOB;
			}
		}
		if (!HASERROR) {
			if (snap->OOBp1.core.formations.cnt != dossier->props.fcnt) {
				ERROR2 ("dossier formation count (%d) != snapshot core formations count (%d)", dossier->props.fcnt, snap->OOBp1.core.formations.cnt);
				rc = SPWERR_NOMATCH_CORECNT;
			}
		}
		if (!HASERROR) {
			if (snap->OOBp1.core.units.cnt != dossier->props.ucnt) {
				ERROR2 ("dossier unit count (%d) != snapshot core units count (%d)", dossier->props.ucnt, snap->OOBp1.core.units.cnt);
				rc = SPWERR_NOMATCH_CORECNT;
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
			if (snap->game.campaign.data.battles_max != dossier->props.maxbcnt) {
				ERROR2 ("dossier campaign max battle count (%d) != snapshot campaign max battle count (%d)", dossier->props.maxbcnt, snap->game.campaign.data.battles_max);
				rc = SPWERR_NOMATCH_CMBCNT;
			}
		}
	}
	ROE ("snapshot compatibility verification");

	/* Set dossier type (if not already set) */
	if (dossier->type == SPWAW_EMPTY_DOSSIER) {
		dossier->type = SPWAW_CAMPAIGN_DOSSIER;
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
			SPWAW_TIMESTAMP	tsb, tss;
			SPWAW_date2stamp (&(battle->date), &tsb);
			SPWAW_date2stamp (&(snap->game.battle.data.date), &tss);
			if ((tss - (snap->game.battle.data.turn * SPWAW_MINSPERTURN)) != tsb) {
				char bdate[32];
				char sdate[32];
				SPWAW_date2str (&(battle->date), bdate, sizeof(bdate));
				SPWAW_date2str (&(snap->game.battle.data.date), sdate, sizeof(sdate));
				ERROR2 ("battle date \"%s\" != snapshot date \"%s\"", bdate, sdate);
				rc = SPWERR_NOMATCH_DATE;
			}
		}
		if (!HASERROR) {
			if (strcmp(snap->game.battle.data.location, battle->location) != 0) {
				ERROR2 ("battle location \"%s\" != snapshot location \"%s\"", battle->location, snap->game.battle.data.location);
				rc = SPWERR_NOMATCH_LOCATION;
			}
		}
		if (!HASERROR) {
			if (strcmp(SPWAW_mission2str(snap->game.battle.data.miss_p1), battle->miss_p1) != 0) {
				ERROR2 ("battle player mission type \"%s\" != snapshot player mission type \"%s\"", battle->miss_p1, snap->game.battle.data.miss_p1);
				rc = SPWERR_NOMATCH_MISSION;
			}
		}
		if (!HASERROR) {
			if (strcmp(SPWAW_mission2str(snap->game.battle.data.miss_p2), battle->miss_p2) != 0) {
				ERROR2 ("battle opponent mission type \"%s\" != snapshot opponent mission type \"%s\"", battle->miss_p2, snap->game.battle.data.miss_p2);
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
