/** \file
 * The SPWaW war cabinet - configuration handling.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "../common.h"
#include "cfg_dlg.h"



/* --- forward declarations --- */

static void	statereport	(SL_STDBG_INFO_LEVEL level);



/* --- private types  --- */

/*! Configuration data structure */
typedef struct s_CFGDATA {
	int		revision;	/*!< Configuration revision number	*/
	char		*cwd_path;	/*!< Initial current working directory	*/
	char		*app_path;	/*!< Application path			*/
	char		*usr_path;	/*!< User data storage path		*/
	char		*oob_path;	/*!< SPWAW OOB path			*/
	char		*sve_path;	/*!< SPWAW savegames path		*/
	char		*snp_path;	/*!< Snapshots path			*/
	bool		compress;	/*!< Dossier compression flag		*/
	bool		autoload;	/*!< Dossier autoload flag		*/
	char		*lastdoss;	/*!< Last opened dossier		*/
	GUI_STATE	gui_state;	/*!< GUI state				*/
} CFGDATA;



/* --- private macros  --- */

/*! Configuration version */
#define	CFG_REVISION	2


/* --- private variables --- */

/*! Module name */
static const char	*MODULE = "CFG";

/*! Initialization status flag */
static SL_BOOL		initialized = SL_false;

/*! QT settings object */
static QSettings	*storage = NULL;

/*! Configuration */
static CFGDATA		cfg;



/* --- code --- */

static void
config_load (void)
{
	QVariant	data;
	char		buffer[MAX_PATH+1];

	data = storage->value ("CfgRevision");
	if (data.isNull ()) {
		cfg.revision = DEFAULT_REVISION;
	} else {
		cfg.revision = data.toInt();
	}

	data = storage->value ("OobPath");
	if (!data.isNull ()) {
		SL_SAFE_STRDUP (cfg.oob_path, qPrintable(data.toString()));
	}

	data = storage->value ("SavesPath");
	if (!data.isNull ()) {
		SL_SAFE_STRDUP (cfg.sve_path, qPrintable(data.toString()));
	}

	data = storage->value ("SnapshotsPath");
	if (data.isNull ()) {
		memset (buffer, 0, sizeof (buffer));
		snprintf (buffer, sizeof (buffer) - 1, DEFAULT_SNAPPATH, cfg.app_path);
		SL_SAFE_STRDUP (cfg.snp_path, buffer);
	} else {
		SL_SAFE_STRDUP (cfg.snp_path, qPrintable(data.toString()));
	}

	data = storage->value ("Compression");
	if (data.isNull ()) {
		cfg.compress = DEFAULT_COMPRESSION;
	} else {
		cfg.compress = data.toBool();
	}

	data = storage->value ("Autoload");
	if (data.isNull ()) {
		cfg.autoload = DEFAULT_AUTOLOAD;
	} else {
		cfg.autoload = data.toBool();
	}

	data = storage->value ("LastDossier");
	if (!data.isNull ()) {
		SL_SAFE_STRDUP (cfg.lastdoss, qPrintable(data.toString()));
		if (!strlen (cfg.lastdoss)) SL_SAFE_FREE (cfg.lastdoss);
	}

	storage->beginGroup("GUI");

	data = storage->value ("State");
	if (data.isNull ()) {
		cfg.gui_state.state = DEFAULT_GUI_STATE;
	} else {
		cfg.gui_state.state = (Qt::WindowStates)data.toUInt();
	}

	data = storage->value ("Size");
	if (data.isNull ()) {
		cfg.gui_state.size = new QSize (DEFAULT_GUI_WIDTH, DEFAULT_GUI_HEIGHT);
	} else {
		cfg.gui_state.size = new QSize (data.toSize());
	}
	DEVASSERT (cfg.gui_state.size != NULL);

	data = storage->value ("Position");
	if (data.isNull ()) {
		cfg.gui_state.pos = new QPoint (DEFAULT_GUI_XPOS, DEFAULT_GUI_YPOS);
	} else {
		cfg.gui_state.pos = new QPoint (data.toPoint());
		if (cfg.gui_state.pos->x() < 0) cfg.gui_state.pos->setX (0);
		if (cfg.gui_state.pos->y() < 0) cfg.gui_state.pos->setY (0);
	}
	DEVASSERT (cfg.gui_state.pos != NULL);

	data = storage->value ("Panes");
	if (data.isNull ()) {
		cfg.gui_state.state_panes = new QByteArray ("");
	} else {
		cfg.gui_state.state_panes = new QByteArray (QByteArray::fromBase64(data.toByteArray()));
	}
	DEVASSERT (cfg.gui_state.state_panes != NULL);

	data = storage->value ("MainWindow");
	if (data.isNull ()) {
		cfg.gui_state.state_main = new QByteArray ("");
	} else {
		cfg.gui_state.state_main = new QByteArray (QByteArray::fromBase64(data.toByteArray()));
	}
	DEVASSERT (cfg.gui_state.state_main != NULL);

	storage->endGroup();
}

static void
config_save (void)
{
	QVariant	data;

	data.setValue (CFG_REVISION);
	storage->setValue ("CfgRevision", data);

	data.setValue (QString (cfg.oob_path));
	storage->setValue ("OobPath", data);

	data.setValue (QString (cfg.sve_path));
	storage->setValue ("SavesPath", data);

	data.setValue (QString (cfg.snp_path));
	storage->setValue ("SnapshotsPath", data);

	data.setValue (cfg.compress);
	storage->setValue ("Compression", data);

	data.setValue (cfg.autoload);
	storage->setValue ("Autoload", data);

	data.setValue (QString (cfg.lastdoss));
	storage->setValue ("LastDossier", data);

	storage->beginGroup("GUI");

	data.setValue ((uint)cfg.gui_state.state);
	storage->setValue ("State", data);

	data.setValue (*(cfg.gui_state.size));
	storage->setValue ("Size", data);

	if (cfg.gui_state.pos->x() < 0) cfg.gui_state.pos->setX (0);
	if (cfg.gui_state.pos->y() < 0) cfg.gui_state.pos->setY (0);
	data.setValue (*(cfg.gui_state.pos));
	storage->setValue ("Position", data);

	data.setValue (cfg.gui_state.state_panes->toBase64());
	storage->setValue ("Panes", data);

	data.setValue (cfg.gui_state.state_main->toBase64());
	storage->setValue ("MainWindow", data);

	storage->endGroup();
}

SL_ERROR
CFG_init (void)
{
	if (initialized) RETURN_OK;
	SL_STDBG_add (statereport, MODULE);

	storage = new QSettings (SL_APP_auth (), SL_APP_name ());
	if (!storage)
		RETURN_ERR_FUNCTION_EX0 (ERR_CFG_NOPERSIST, "failed to create QSettings storage object");

	memset (&cfg, 0, sizeof (cfg));

	if (!(cfg.cwd_path = SL_APP_cwd ()))
		RETURN_ERR_FUNCTION_EX0 (ERR_CFG_PATH_NOCWD, "SL_APP_cwd() call failed");

	if (!(cfg.app_path = SL_APP_exe_path ()))
		RETURN_ERR_FUNCTION_EX0 (ERR_CFG_PATH_NOAPP, "SL_APP_exe_path() call failed");

	if (!(cfg.usr_path = SL_APP_data_path ()))
		RETURN_ERR_FUNCTION_EX0 (ERR_CFG_PATH_NOUSR, "SL_APP_data_path() call failed");

	config_load ();

	initialized = SL_true;

	RETURN_OK;
}

void
CFG_shutdown (void)
{
	if (!initialized) return;
	SL_STDBG_delete (statereport);

	config_save ();
	delete storage;

	if (cfg.gui_state.size)		delete cfg.gui_state.size;
	if (cfg.gui_state.pos)		delete cfg.gui_state.pos;
	if (cfg.gui_state.state_panes)	delete cfg.gui_state.state_panes;
	if (cfg.gui_state.state_main)	delete cfg.gui_state.state_main;

	if (cfg.oob_path) SL_SAFE_FREE (cfg.oob_path);
	if (cfg.sve_path) SL_SAFE_FREE (cfg.sve_path);
	if (cfg.snp_path) SL_SAFE_FREE (cfg.snp_path);
	if (cfg.lastdoss) SL_SAFE_FREE (cfg.lastdoss);

	initialized = SL_false;
}

bool
CFG_needsreview (bool &isfirstrun)
{
	bool	b;

	b = (cfg.revision != CFG_REVISION);
	isfirstrun = (b && (cfg.revision == DEFAULT_REVISION));
	return (b);
}

bool
CFG_iscomplete (void)
{
	bool	b = true;

	if (!cfg.oob_path || !strlen (cfg.oob_path)) b = false;
	if (!cfg.sve_path || !strlen (cfg.sve_path)) b = false;

	return (b);
}

char *
CFG_cwd_path (void)
{
	return (initialized ? cfg.cwd_path : NULL);
}

char *
CFG_app_path (void)
{
	return (initialized ? cfg.app_path : NULL);
}

char *
CFG_usr_path (void)
{
	return (initialized ? cfg.usr_path : NULL);
}

char *
CFG_oob_path (void)
{
	return (initialized ? cfg.oob_path : NULL);
}

static void
CFG_SET_oob_path (char *str)
{
	if (!initialized || !str) return;

	if (cfg.oob_path) SL_SAFE_FREE (cfg.oob_path);
	SL_SAFE_STRDUP (cfg.oob_path, str);
	DEVASSERT (cfg.oob_path != NULL);
}

char *
CFG_save_path (void)
{
	return (initialized ? cfg.sve_path : NULL);
}

static void
CFG_SET_sve_path (char *str)
{
	if (!initialized || !str) return;

	if (cfg.sve_path) SL_SAFE_FREE (cfg.sve_path);
	SL_SAFE_STRDUP (cfg.sve_path, str);
	DEVASSERT (cfg.sve_path != NULL);
}

char *
CFG_snap_path (void)
{
	return (initialized ? cfg.snp_path : NULL);
}

static void
CFG_SET_snp_path (char *str)
{
	if (!initialized || !str) return;

	if (cfg.snp_path) SL_SAFE_FREE (cfg.snp_path);
	SL_SAFE_STRDUP (cfg.snp_path, str);
	DEVASSERT (cfg.snp_path != NULL);

	// Try to make sure the directory exists, but ignore any errors for now
	QDir::root().mkpath(cfg.snp_path);
}

bool
CFG_compress (void)
{
	return (initialized ? cfg.compress : false);
}

static void
CFG_SET_compress (bool b)
{
	if (!initialized) return;

	cfg.compress = b;
}

static bool
CFG_autoload (void)
{
	return (initialized ? cfg.autoload : false);
}

static void
CFG_SET_autoload (bool b)
{
	if (!initialized) return;

	cfg.autoload = b;
}

char *
CFG_autoload_get (void)
{
	if (!cfg.autoload) return (NULL);
	return (cfg.lastdoss);
}

void
CFG_autoload_set (char *file)
{
	if (!cfg.autoload) return;

	SL_SAFE_FREE (cfg.lastdoss);
	if (file && strlen (file)) SL_SAFE_STRDUP (cfg.lastdoss, file);
}

GUI_STATE *
CFG_gui_state_get (void)
{
	return (initialized ? &(cfg.gui_state) : NULL);
}

void
CFG_gui_state_set (GUI_STATE &state)
{
	if (!initialized) return;

	cfg.gui_state.state = state.state;
	if (state.size) {
		delete cfg.gui_state.size;
		cfg.gui_state.size = new QSize (*(state.size));
	}
	if (state.pos) {
		delete cfg.gui_state.pos;
		cfg.gui_state.pos = new QPoint (*(state.pos));
	}
	if (state.state_panes) {
		delete cfg.gui_state.state_panes;
		cfg.gui_state.state_panes = new QByteArray (*(state.state_panes));
	}
	if (state.state_main) {
		delete cfg.gui_state.state_main;
		cfg.gui_state.state_main = new QByteArray (*(state.state_main));
	}
}

bool
CFG_DLG (void)
{
	CfgDlgData	data;
	CfgDlg		*dlg = NULL;
	int		rc;

	data.oob	= CFG_oob_path ();
	data.sve	= CFG_save_path ();
	data.snp	= CFG_snap_path ();
	data.compress	= CFG_compress ();
	data.autoload	= CFG_autoload ();

	dlg = new CfgDlg ();
	dlg->data_set (&data);
	rc = dlg->exec ();
	dlg->data_get (&data);

	if (rc == QDialog::Accepted) {
		CFG_SET_oob_path (data.oob);
		CFG_SET_sve_path (data.sve);
		CFG_SET_snp_path (data.snp);
		CFG_SET_compress (data.compress);
		CFG_SET_autoload (data.autoload);
	}
	dlg->data_free (&data);
	delete dlg;

	return (rc == QDialog::Accepted);
}

static void
statereport (SL_STDBG_INFO_LEVEL level)
{
	SAYSTATE1 ("### %s state:\n", MODULE);

	/* basic information */
	if (level >= SL_STDBG_LEVEL_BAS) {
		SAYSTATE1 ("\tinitialized     = %s\n", SL_BOOL_tostr (initialized));
		SAYSTATE1 ("\tCWD             = %s\n", cfg.cwd_path);
		SAYSTATE1 ("\tapp path        = %s\n", cfg.app_path);
		SAYSTATE1 ("\tuser data path  = %s\n", cfg.usr_path);
		SAYSTATE1 ("\tOOB path        = %s\n", cfg.oob_path);
		SAYSTATE1 ("\tsaves path      = %s\n", cfg.sve_path);
		SAYSTATE1 ("\tsnapshots path  = %s\n", cfg.snp_path);
		SAYSTATE1 ("\tcompression     = %s\n", cfg.compress?"enabled":"disabled");
		SAYSTATE1 ("\tautoload        = %s\n", cfg.autoload?"enabled":"disabled");
		SAYSTATE1 ("\tlast dossier    = %s\n", cfg.lastdoss);
		SAYSTATE1 ("\tGUI state       = %u\n", cfg.gui_state.state);
		SAYSTATE2 ("\tGUI size        = (%d, %d)\n", cfg.gui_state.size->width(), cfg.gui_state.size->height());
		SAYSTATE2 ("\tGUI position    = (%d, %d)\n", cfg.gui_state.pos->x(), cfg.gui_state.pos->y());
		if (cfg.gui_state.state_panes->isEmpty()) {
			SAYSTATE1 ("\tGUI main panes  = %s\n", "empty");
		} else if (cfg.gui_state.state_panes->isNull()) {
			SAYSTATE1 ("\tGUI main panes  = %s\n", "NULL");
		} else {
			SAYSTATE1 ("\tGUI main panes  = %d bytes\n", cfg.gui_state.state_panes->size());
		}
		if (cfg.gui_state.state_main->isEmpty()) {
			SAYSTATE1 ("\tGUI main window = %s\n", "empty");
		} else if (cfg.gui_state.state_main->isNull()) {
			SAYSTATE1 ("\tGUI main window = %s\n", "NULL");
		} else {
			SAYSTATE1 ("\tGUI main window = %d bytes\n", cfg.gui_state.state_main->size());
		}
	}

	/* extended information */
	if (level >= SL_STDBG_LEVEL_EXT) {
		SAYSTATE1 ("\tstorage             = 0x%8.8x\n", storage);
		SAYSTATE1 ("\tGUI size obj        = 0x%8.8x\n", cfg.gui_state.size);
		SAYSTATE1 ("\tGUI main panes obj  = 0x%8.8x\n", cfg.gui_state.state_panes);
		SAYSTATE1 ("\tGUI main window obj = 0x%8.8x\n", cfg.gui_state.state_main);
	}

	/* deep probe */
	if (level >= SL_STDBG_LEVEL_DEEP) {
	}

	SAYSTATE0 ("\n");
}
