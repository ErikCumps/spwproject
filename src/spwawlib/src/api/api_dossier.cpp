#include "stdafx.h"
#include <spwawlib_dossier.h>
#include "dossier/dossier.h"
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

	/* Record OOB data */
	rc = SPWOOB_copy (ptr->oobdat, cfg.oobptr);
	ERRORGOTO ("spwoob_copy()", handle_error);

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
	int		fd;
	SPWAW_ERROR	rc = SPWERR_OK;

	CSPWINIT;
	CNULLARG (dossier); CNULLARG (*dossier); CNULLARG (file);

	fd = open (file, O_RDWR|O_BINARY|O_CREAT|O_TRUNC, 0666);
	if (fd < 0) FAILGOTO (SPWERR_FOFAILED, "dossier file create", handle_error);

	rc = dossier_save (*dossier, fd, compress);
	ERRORGOTO ("dossier_save()", handle_error);

	rc = fcheck_make (fd);
	ERRORGOTO ("fcheck_make()", handle_error);

	close (fd);

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
SPWAW_dossier_add (SPWAW_DOSSIER *dossier, SPWAW_SNAPSHOT *snap, SPWAW_BTURN **bturn)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_BTURN	*p = NULL;

	CSPWINIT;
	CNULLARG (dossier); CNULLARG (snap);
	if (bturn) *bturn = NULL;

	rc = SPWOOB_compare (snap->oobdat, dossier->oobdat);
	ROE ("SPWOOB_compare(snapshot, dossier)");

	if ((dossier->bcnt != 0) && (rc == SPWERR_OK)) {
		if (snap->game.battle.data.OOB_p1 != dossier->OOB) {
			ERROR2 ("dossier OOB (%d) != snapshot OOB (%d)", dossier->OOB, snap->game.battle.data.OOB_p1);
			rc = SPWERR_NOMATCH_OOB;
		}
		if (snap->OOBp1.core.formations.cnt != dossier->fcnt) {
			ERROR2 ("dossier formation count (%d) != snapshot core formations count (%d)", dossier->fcnt, snap->OOBp1.core.formations.cnt);
			rc = SPWERR_NOMATCH_CORECNT;
		} else if (snap->OOBp1.core.units.cnt != dossier->ucnt) {
			ERROR2 ("dossier unit count (%d) != snapshot core units count (%d)", dossier->ucnt, snap->OOBp1.core.units.cnt);
			rc = SPWERR_NOMATCH_CORECNT;
		}
	}
	ROE ("snapshot compatibility verification");

	rc = dossier_add (dossier, snap, &p);
	ROE ("dossier_add()");

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
