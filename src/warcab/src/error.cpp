/** \file
 * The SPWaW war cabinet - error handling.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "common.h"

/* --- private variables --- */

/* Our own error messages */
static const char *errormsgs[LAST_ERR_CODE-SL_ERR_USER_START+1] = {
	"generic failure",
	"failed to set up persistent configuration",
	"failed to discover initial cwd",
	"failed to discover application path",
	"failed to discover user data storage path",
	"incomplete configuration detected",
	"failed to open resource archive",
	"resource archive item not found",
	"failed to load resource archive item",
	"failed to initialize GUI main window",
	"failed to initialize GUI actions",
	"failed to initialize GUI main menu",
	"failed to initialize GUI toolbar",
	"failed to initialize GUI statusbar",
	"failed to initialize GUI dossier view",
	"failed to initialize GUI reports view",
	"failed to initialize GUI plot view",
	"failed to initialize GUI view widget",
	"failed to initialize GUI strategic map widget",
	"failed to initialize GUI dossier reports view",
	"failed to initialize GUI battle reports view",
	"failed to initialize GUI turn reports view",
	"failed to initialize GUI OOB reports view",
	"failed to initialize GUI roster reports view",
	"failed to initialize GUI history reports view",
	"failed to initialize GUI unitlist reports view",
	"failed to initialize GUI MMAS reports view",
	"failed to initialize application",
	"failed to initialize application state",
	"failed to create savegame load dialog",
	"failed to create snapshot load dialog",
	"failed to create dossier load dialog",
	"failed to create dossier edit dialog",
	"failed to create about dialog",
	"GUI action failed",
	"failed to setup dossier",
	"failed to refresh dossier",
	"dossier initial load failed",
	"failed to create new dossier",
	"failed to find dossier file",
	"failed to load dossier",
	"unable to load corrupt dossier",
	"failed to save dossier",
	"unable to save dossier: no filename specified",
	"failed to edit dossier",
	"failed to add savegame to dossier",
	"failed to add snapshot to dossier",
	"failed to remove item from dossier",
};



/* --- code --- */

SL_ERROR
register_errorcodes (void)
{
        SL_ERROR	rc;
        int		i;

        for (i = SL_ERR_USER_START; i <= LAST_ERR_CODE; i++) {
                rc = SL_ERROR_code_register (i, errormsgs[i-SL_ERR_USER_START]);
		SL_ROE (rc);
        }
        RETURN_OK;
}
