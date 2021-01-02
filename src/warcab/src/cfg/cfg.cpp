/** \file
 * The SPWaW war cabinet - configuration handling.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "../common.h"
#include "cfg_dlg.h"



/* --- forward declarations --- */

static void	statereport	(SL_STDBG_INFO_LEVEL level);



/* --- private types  --- */

/*! Game configuration data structure */
typedef struct s_GAMECFG {
	bool		active;			/*!< Config activation flag			*/
	QString		name;			/*!< Game name					*/
	SPWAW_GAME_TYPE	type;			/*!< Game type					*/
	QString		path;			/*!< Game path					*/
} GAMECFG;

/*! Configuration data structure */
typedef struct s_CFGDATA {
	int		revision;		/*!< Configuration revision number	*/
	char		*cwd_path;		/*!< Initial current working directory	*/
	char		*app_path;		/*!< Application path			*/
	char		*usr_path;		/*!< User data storage path		*/
	bool		local;			/*!< Local configuration flag		*/
	char		*snp_path;		/*!< Snapshots path			*/
	bool		compress;		/*!< Dossier compression flag		*/
	bool		autoload;		/*!< Dossier autoload flag		*/
	char		*lastdoss;		/*!< Last opened dossier		*/
	bool		fhistory;		/*!< Campaign full history flag		*/
	INTEL_MODE	intel_mode;		/*!< Default intel mode			*/
	int		hcftype;		/*!< Default height colorfield type	*/
	bool		german_cross;		/*!< German Cross flag			*/
	GUI_STATE	gui_state;		/*!< GUI state				*/
	GameCfgIdx	def_gamecfg_idx;	/*!< Default game config index		*/
} CFGDATA;



/* --- private macros  --- */

/*! Configuration version */
#define	CFG_REVISION		8

/*! Configuration version with legacy GameCfg */
#define	CFG_LEGACY_GAMECFG	7

/*! GameCFgIdx for legacy SP:WaW game config */
#define	GAMECFG_LEGACY_SPWAW	0

/*! GameCFgIdx for legacy winSPWW2 game config */
#define	GAMECFG_LEGACY_WINSPWW2	1

/* gamedir/oobdir/savedir conversion strings */

/*! SP:WaW relative OOB directory */
#define	GOSC_SPWAW_OOB		""

/*! SP:WaW relative save directory */
#define	GOSC_SPWAW_SAVE		"SAVE"

/*! SP:WaW relative Mega Campaign save directory */
#define	GOSC_SPWAW_MEGACAM	"MegaCam\\save"

/*! winSPWW2 relative OOB directory */
#define	GOSC_WINSPWW2_OOB	"Game Data\\OOBs"

/*! winSPWW2 relative save directory */
#define	GOSC_WINSPWW2_SAVE	"Saved Games"

/*! Warcab relative test OOB directory */
#define	GOSC_TEST_OOB		"_OOB_"

/*! Warcab relative test save directory */
#define	GOSC_TEST_SAVE		"_SAVE_"



/* --- private variables --- */

/*! Module name */
static const char		*MODULE = "CFG";

/*! Initialization status flag */
static SL_BOOL			initialized = SL_false;

/*! QT settings objects */
static QSettings		*storage_local = NULL;
static QSettings		*storage_global = NULL;

/*! Configuration */
static CFGDATA			cfg;
static GAMECFG			gamecfg[GAMECFG_CNT];
static SPWAW_OOBCFG		oobcfg[GAMECFG_CNT];
static int			oobcfg_cnt;



/* --- code --- */

static bool
gamedir_from_oobdir_core (QString &dir, const char *oobdir)
{
	int	idx;

	idx = dir.lastIndexOf (oobdir, -1, Qt::CaseInsensitive);
	if (idx != -1) {
		if (oobdir[0] != '\0') dir.truncate(idx-1);
		return (true);
	}
	return (false);
}

static bool
gamedir_from_oobdir (QString &oobdir, SPWAW_GAME_TYPE gametype, QString &gamedir)
{
	bool	ok;

	gamedir.clear();
	if (oobdir.isEmpty()) return (false);

	gamedir = oobdir;

	if (gamedir_from_oobdir_core (gamedir, GOSC_TEST_OOB)) return (true);

	switch (gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
		default:
			ok = gamedir_from_oobdir_core (gamedir, GOSC_SPWAW_OOB);
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			ok = gamedir_from_oobdir_core (gamedir, GOSC_WINSPWW2_OOB);
			break;
	}
	if (!ok) gamedir.clear();

	return (ok);
}

static bool
gamedir_from_savedir_core (QString &dir, const char *savedir)
{
	int	idx;

	idx = dir.lastIndexOf (savedir, -1, Qt::CaseInsensitive);
	if (idx != -1) {
		if (savedir[0] != '\0') dir.truncate(idx-1);
		return (true);
	}
	return (false);
}

static bool
gamedir_from_savedir (QString &savedir, SPWAW_GAME_TYPE gametype, QString &gamedir)
{
	bool	ok;

	gamedir.clear();
	if (savedir.isEmpty()) return (false);

	gamedir = savedir;

	if (gamedir_from_savedir_core (gamedir, GOSC_TEST_SAVE)) return (true);

	switch (gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
		default:
			ok = gamedir_from_savedir_core (gamedir, GOSC_SPWAW_SAVE);
			if (!ok) {
				ok = gamedir_from_savedir_core (gamedir, GOSC_SPWAW_MEGACAM);
			}
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			ok = gamedir_from_savedir_core (gamedir, GOSC_WINSPWW2_SAVE);
			break;
	}
	if (!ok) gamedir.clear();

	return (ok);
}

static bool
validate_gamedir (QString gamedir, SPWAW_GAME_TYPE gametype)
{
	QDir	path, gdpo, gdps1, gdps2;
	bool	ok;

	if (gamedir.isEmpty()) return (false);

	path = gamedir;
	if (!path.exists()) return (false);

	gdpo = gdps1 = path;
	if (gdpo.cd(GOSC_TEST_OOB) && gdps1.cd(GOSC_TEST_SAVE)) return (true);

	ok = false;
	switch (gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			gdpo = gdps1 = gdps2 = path;
			if (gdpo.cd(GOSC_SPWAW_OOB) && gdps1.cd(GOSC_SPWAW_SAVE) && gdps2.cd(GOSC_SPWAW_MEGACAM)) {
				ok = true;
			}
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			gdpo = gdps1 = path;
			if (gdpo.cd(GOSC_WINSPWW2_OOB) && gdps1.cd(GOSC_WINSPWW2_SAVE)) {
				ok = true;
			}
			break;
		default:
			break;
	}
	return (ok);
}


static bool
oobdir_from_gamedir (QString &gamedir, SPWAW_GAME_TYPE gametype, QString &oobdir)
{
	QDir	path, sdp;
	bool	ok;

	oobdir.clear();
	if (gamedir.isEmpty()) return (false);

	path = gamedir;

	if (!path.exists()) return (false);

	sdp = path;
	if (sdp.cd(GOSC_TEST_OOB)) {
		oobdir = sdp.absolutePath();
		return (true);
	}

	sdp = path; ok = false;
	switch (gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
		default:
			if (sdp.cd(GOSC_SPWAW_OOB)) {
				oobdir = path.absolutePath() + QDir::separator() + GOSC_SPWAW_OOB;
				ok = true;
			}
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			if (sdp.cd(GOSC_WINSPWW2_OOB)) {
				oobdir = path.absolutePath() + QDir::separator() + GOSC_WINSPWW2_OOB;
				ok = true;
			}
			break;
	}
	return (ok);
}

static bool
savedir_from_gamedir (QString &gamedir, SPWAW_GAME_TYPE gametype, SPWAW_DOSSIER_TYPE type, QString &savedir)
{
	QDir	path, sdp;
	bool	ok;

	savedir.clear();
	if (gamedir.isEmpty()) return (false);

	path = gamedir;

	if (!path.exists()) return (false);

	sdp = path;
	if (sdp.cd(GOSC_TEST_SAVE)) {
		savedir = sdp.absolutePath();
		return (true);
	}

	sdp = path; ok = false;
	switch (gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
		default:
			switch (type) {
				case SPWAW_MEGACAM_DOSSIER:
					if (sdp.cd(GOSC_SPWAW_MEGACAM)) {
						savedir = path.absolutePath() + QDir::separator() + GOSC_SPWAW_MEGACAM;
						ok = true;
					}
					break;
				default:
					if (sdp.cd(GOSC_SPWAW_SAVE)) {
						savedir = path.absolutePath() + QDir::separator() + GOSC_SPWAW_SAVE;
						ok = true;
					}
					break;
			}
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			if (sdp.cd(GOSC_WINSPWW2_SAVE)) {
				savedir = path.absolutePath() + QDir::separator() + GOSC_WINSPWW2_SAVE;
				ok = true;
			}
			break;
	}
	return (ok);
}


static bool
savedir_from_oobdir (QString &oobdir, SPWAW_GAME_TYPE gametype, SPWAW_DOSSIER_TYPE type, QString &savedir)
{
	QString	gamedir;

	savedir.clear();
	if (oobdir.isEmpty()) return (false);

	if (!gamedir_from_oobdir (oobdir, gametype, gamedir)) return (false);

	return (savedir_from_gamedir(gamedir, gametype, type, savedir));
}

static void
GAMECFG_init (void)
{
	for (int i=0; i<GAMECFG_CNT; i++) {
		gamecfg[i].active = false;
		gamecfg[i].name.clear();
		gamecfg[i].type = SPWAW_GAME_TYPE_UNKNOWN;
		gamecfg[i].path.clear();
	}
}

static void
GAMECFG_shutdown (void)
{
	for (int i=0; i<GAMECFG_CNT; i++) {
		if (!gamecfg[i].active) continue;
		gamecfg[i].name.clear();
		gamecfg[i].type = SPWAW_GAME_TYPE_UNKNOWN;
		gamecfg[i].path.clear();
	}
}

static void
GAMECFG_statereport (SL_STDBG_INFO_LEVEL level)
{
	for (int i=0; i<GAMECFG_CNT; i++) {
		/* basic information */
		if (level >= SL_STDBG_LEVEL_BAS) {
			SAYSTATE1 ("\tgamecfg slot #%d:\n", i);
			SAYSTATE1 ("\t\tactive    = %s\n", gamecfg[i].active?"yes":"no");
			SAYSTATE1 ("\t\tgame name = \"%s\"\n", gamecfg[i].name);
			SAYSTATE1 ("\t\tgame type = %s\n", SPWAW_gametype2str (gamecfg[i].type));
			SAYSTATE1 ("\t\tgame path = \"%s\"\n", qPrintable(gamecfg[i].path));
		}

		/* extended information */
		if (level >= SL_STDBG_LEVEL_EXT) {
		}

		/* deep probe */
		if (level >= SL_STDBG_LEVEL_DEEP) {
		}

		SAYSTATE0 ("\n");
	}
}

static int
GAMECFG_active_count (void)
{
	int	cnt = 0;

	for (int i=0; i<GAMECFG_CNT; i++) {
		if (gamecfg[i].active) cnt++;
	}
	return (cnt);
}

static GameCfgList
GAMECFG_active_list (int &def_gamecfg)
{
	GameCfgList	list;

	def_gamecfg = GAMECFG_IDX_NOT_SET;
	for (int i=0; i<GAMECFG_CNT; i++) {
		if (gamecfg[i].active) {
			list.append(GameCfg(gamecfg[i].name, gamecfg[i].type, gamecfg[i].path));
			if (i == cfg.def_gamecfg_idx) def_gamecfg = list.size()-1;
		}
	}
	return (list);
}

static void
OOBCFG_init (void)
{
	memset (oobcfg, 0, sizeof (oobcfg));
	oobcfg_cnt = 0;
}

static void
OOBCFG_clear (void)
{
	for (int i=0; i<GAMECFG_CNT; i++) {
		if (oobcfg[i].oobdir) SL_SAFE_FREE (oobcfg[i].oobdir);
	}
	OOBCFG_init();
}

static void
OOBCFG_update (void)
{
	int	idx;
	QString	oobdir;

	OOBCFG_clear();

	idx = 0;
	for (int i=0; i<GAMECFG_CNT; i++) {
		if (!gamecfg[i].active) continue;
		if (!oobdir_from_gamedir (gamecfg[i].path, gamecfg[i].type, oobdir)) continue;

		oobcfg[idx].gametype = gamecfg[i].type;
		SL_SAFE_STRDUP (oobcfg[idx].oobdir, qPrintable(oobdir));

		idx++;
	}
	oobcfg_cnt = idx;
}

static void
load_config_local_flag (void)
{
	QVariant	data;

	data = storage_local->value ("CfgLocal");
	if (data.isNull ()) {
		cfg.local = DEFAULT_LOCALCFG;
	} else {
		cfg.local = data.toBool();
	}
}

static void
save_config_local_flag (void)
{
	QVariant	data;

	data.setValue (cfg.local);
	storage_local->setValue ("CfgLocal", data);
}

static void
config_load_game_spwaw (QSettings *storage, GAMECFG &cfg)
{
	QVariant	data;
	QString		gamedir;

	cfg.type = SPWAW_GAME_TYPE_SPWAW;
	cfg.name = QString ("My %1 game").arg(SPWAW_gametype2str (cfg.type));

	data = storage->value ("OobPathSPWAW");
	if (!gamedir_from_oobdir (data.toString(), cfg.type, cfg.path)) return;

	cfg.active = validate_gamedir (cfg.path, cfg.type);
}

static void
config_load_game_winspww2 (QSettings *storage, GAMECFG &cfg)
{
	QVariant	data;

	cfg.type = SPWAW_GAME_TYPE_WINSPWW2;
	cfg.name = QString ("My %1 game").arg(SPWAW_gametype2str (cfg.type));

	data = storage->value ("OobPathWINSPWW2");
	if (!gamedir_from_oobdir (data.toString(), cfg.type, cfg.path)) return;

	cfg.active = validate_gamedir (cfg.path, cfg.type);
}

static void
config_load_gamecfg_legacy (QSettings *storage)
{
	QVariant	data;

	data = storage->value ("DefGameType");
	if (data.isNull ()) {
		cfg.def_gamecfg_idx = GAMECFG_IDX_NOT_SET;
	} else {
		switch ((SPWAW_GAME_TYPE)data.toInt()) {
			case SPWAW_GAME_TYPE_SPWAW:
				cfg.def_gamecfg_idx = GAMECFG_LEGACY_SPWAW;
				break;
			case SPWAW_GAME_TYPE_WINSPWW2:
				cfg.def_gamecfg_idx = GAMECFG_LEGACY_WINSPWW2;
				break;
			default:
				cfg.def_gamecfg_idx = GAMECFG_IDX_NOT_SET;
				break;
		}
	}

	config_load_game_spwaw (storage, gamecfg[GAMECFG_LEGACY_SPWAW]);
	config_load_game_winspww2 (storage, gamecfg[GAMECFG_LEGACY_WINSPWW2]);
}

static void
config_load_game (QSettings *storage, GameCfgIdx idx, GAMECFG &cfg)
{
	QVariant	datan, datat, datap;

	cfg.active = false;
	cfg.name.clear();
	cfg.type = SPWAW_GAME_TYPE_UNKNOWN;
	cfg.path.clear();

	if ((idx < 0) || (idx > GAMECFG_CNT)) return;

	datan = storage->value (QString("%1_Name").arg(idx));
	datat = storage->value (QString("%1_Type").arg(idx));
	datap = storage->value (QString("%1_Path").arg(idx));

	if (datan.isNull() || datat.isNull() || datap.isNull()) return;

	cfg.name = datan.toString();
	cfg.type = (SPWAW_GAME_TYPE)datat.toInt();
	cfg.path = datap.toString();

	cfg.active = validate_gamedir (cfg.path, cfg.type);
}

static void
config_load_gamecfg (QSettings *storage)
{
	QVariant	data;

	storage->beginGroup("GameCfg");

	data = storage->value ("Default");
	if (data.isNull ()) {
		cfg.def_gamecfg_idx = GAMECFG_IDX_NOT_SET;
	} else {
		cfg.def_gamecfg_idx = data.toInt();
	}

	for (int i=0; i<GAMECFG_CNT; i++) config_load_game (storage, i, gamecfg[i]);

	storage->endGroup();
}

static void
config_load (QSettings *storage)
{
	QVariant	data;
	char		buffer[MAX_PATH+1];

	data = storage->value ("CfgRevision");
	if (data.isNull ()) {
		cfg.revision = DEFAULT_REVISION;
	} else {
		cfg.revision = data.toInt();
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

	data = storage->value ("FullCampaignHistory");
	if (data.isNull ()) {
		cfg.fhistory = DEFAULT_FULL_HISTORY;
	} else {
		cfg.fhistory = data.toBool();
	}

	data = storage->value ("DefaultIntelMode");
	if (data.isNull ()) {
		cfg.intel_mode = DEFAULT_INTEL_MODE;
	} else {
		cfg.intel_mode = raw2intelmode(data.toInt());
	}

	data = storage->value ("DefaultHeightColorfieldType");
	if (data.isNull ()) {
		cfg.hcftype = DEFAULT_HCFTYPE;
	} else {
		cfg.hcftype = data.toInt();
	}

	data = storage->value ("GermanCross");
	if (data.isNull ()) {
		cfg.german_cross = DEFAULT_GERMAN_CROSS;
	} else {
		cfg.german_cross = data.toBool();
	}

	if (cfg.revision <= CFG_LEGACY_GAMECFG) {
		config_load_gamecfg_legacy (storage);
	} else {
		config_load_gamecfg (storage);
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
config_save_game (QSettings *storage, GameCfgIdx idx, GAMECFG &cfg)
{
	QVariant	datan, datat, datap;

	if ((idx < 0) || (idx > GAMECFG_CNT)) return;

	datan.setValue (cfg.name);
	datat.setValue ((int)cfg.type);
	datap.setValue (cfg.path);

	storage->setValue (QString("%1_Name").arg(idx), datan);
	storage->setValue (QString("%1_Type").arg(idx), datat);
	storage->setValue (QString("%1_Path").arg(idx), datap);
}

static void
config_save_gamecfg (QSettings *storage)
{
	QVariant	data;

	storage->beginGroup("GameCfg");

	data.setValue ((int)cfg.def_gamecfg_idx);
	storage->setValue ("Default", data);

	data.setValue (GAMECFG_CNT);
	storage->setValue ("Count", data);

	for (int i=0; i<GAMECFG_CNT; i++) config_save_game (storage, i, gamecfg[i]);

	storage->endGroup();
}


static void
config_save (QSettings *storage)
{
	QVariant	data;

	storage->clear();

	data.setValue (CFG_REVISION);
	storage->setValue ("CfgRevision", data);

	data.setValue (QString (cfg.snp_path));
	storage->setValue ("SnapshotsPath", data);

	data.setValue (cfg.compress);
	storage->setValue ("Compression", data);

	data.setValue (cfg.autoload);
	storage->setValue ("Autoload", data);

	data.setValue (QString (cfg.lastdoss));
	storage->setValue ("LastDossier", data);

	data.setValue (cfg.fhistory);
	storage->setValue ("FullCampaignHistory", data);

	data.setValue (intelmode2raw (cfg.intel_mode));
	storage->setValue ("DefaultIntelMode", data);

	data.setValue (cfg.hcftype);
	storage->setValue ("DefaultHeightColorfieldType", data);

	data.setValue (cfg.german_cross);
	storage->setValue ("GermanCross", data);

	config_save_gamecfg (storage);

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

	storage_local = new QSettings ("warcab.ini", QSettings::IniFormat);
	if (!storage_local)
		RETURN_ERR_FUNCTION_EX0 (ERR_CFG_NOPERSIST, "failed to create QSettings storage_local object");

	storage_global = new QSettings (SL_APP_author (), SL_APP_name ());
	if (!storage_global)
		RETURN_ERR_FUNCTION_EX0 (ERR_CFG_NOPERSIST, "failed to create QSettings storage_global object");

	memset (&cfg, 0, sizeof (cfg));
	GAMECFG_init();
	OOBCFG_init();

	if (!(cfg.cwd_path = SL_APP_cwd ()))
		RETURN_ERR_FUNCTION_EX0 (ERR_CFG_PATH_NOCWD, "SL_APP_cwd() call failed");

	if (!(cfg.app_path = SL_APP_exe_path ()))
		RETURN_ERR_FUNCTION_EX0 (ERR_CFG_PATH_NOAPP, "SL_APP_exe_path() call failed");

	if (!(cfg.usr_path = SL_APP_data_path ()))
		RETURN_ERR_FUNCTION_EX0 (ERR_CFG_PATH_NOUSR, "SL_APP_data_path() call failed");

	load_config_local_flag ();
	if (cfg.local) {
		config_load (storage_local);
	} else {
		config_load (storage_global);
	}
	OOBCFG_update();

	initialized = SL_true;

	RETURN_OK;
}

void
CFG_save (void)
{
	if (!initialized) return;

	if (cfg.local) {
		config_save (storage_local);
		// Don't modify the global config, it could be used by other instances
	} else {
		storage_local->clear();
		config_save (storage_global);
	}
	save_config_local_flag ();
	storage_local->sync();
	storage_global->sync();
}

void
CFG_shutdown (void)
{
	if (!initialized) return;
	SL_STDBG_delete (statereport);

	CFG_save();

	delete storage_global;
	delete storage_local;

	if (cfg.gui_state.size)		delete cfg.gui_state.size;
	if (cfg.gui_state.pos)		delete cfg.gui_state.pos;
	if (cfg.gui_state.state_panes)	delete cfg.gui_state.state_panes;
	if (cfg.gui_state.state_main)	delete cfg.gui_state.state_main;

	GAMECFG_shutdown();
	OOBCFG_clear();

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
	if (cfg.def_gamecfg_idx == GAMECFG_IDX_NOT_SET) return (false);

	return (GAMECFG_active_count() != 0);
}

bool
CFG_oobcfg (SPWAW_OOBCFG **list, int *cnt)
{
	if (!list || !cnt) return (false);

	*list = oobcfg;
	*cnt = oobcfg_cnt;

	return (true);
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
CFG_snp_path (void)
{
	return (initialized ? cfg.snp_path : NULL);
}

static void
CFG_snp_path_set (char *path)
{
	SL_SAFE_FREE (cfg.snp_path);

	if (path && strlen(path)) {
		SL_SAFE_STRDUP (cfg.snp_path, qPrintable(QDir(path).absolutePath()));
		DEVASSERT (cfg.snp_path != NULL);

		// Try to make sure the directory exists, but ignore any errors for now
		QDir::root().mkpath(cfg.snp_path);
	}
}

bool
CFG_compress (void)
{
	return (initialized ? cfg.compress : DEFAULT_COMPRESSION);
}

static bool
CFG_autoload (void)
{
	return (initialized ? cfg.autoload : DEFAULT_AUTOLOAD);
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

	if (file && strlen (file)) {
		SL_SAFE_STRDUP (cfg.lastdoss, file);
	}
}

bool
CFG_full_history (void)
{
	return (initialized ? cfg.fhistory : DEFAULT_FULL_HISTORY);
}

INTEL_MODE
CFG_intel_mode (void)
{
	return (initialized ? cfg.intel_mode : DEFAULT_INTEL_MODE);
}

int
CFG_hcftype (void)
{
	return (initialized ? cfg.hcftype : DEFAULT_HCFTYPE);
}

bool
CFG_german_cross (void)
{
	return (initialized ? cfg.german_cross : DEFAULT_GERMAN_CROSS);
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

QString
CFG_gamename (GameCfgIdx idx)
{
	QString	unconfigured("UNCONFIGURED GAME");

	if (!initialized) return (unconfigured);
	if ((idx < 0) || (idx > GAMECFG_CNT)) return (unconfigured);
	if (!gamecfg[idx].active) return (unconfigured);

	return (gamecfg[idx].name);
}

SPWAW_GAME_TYPE
CFG_gametype (GameCfgIdx idx)
{
	if (!initialized) return (SPWAW_GAME_TYPE_UNKNOWN);
	if ((idx < 0) || (idx > GAMECFG_CNT)) return (SPWAW_GAME_TYPE_UNKNOWN);
	if (!gamecfg[idx].active) return (SPWAW_GAME_TYPE_UNKNOWN);

	return (gamecfg[idx].type);
}

QString
CFG_gamepath (GameCfgIdx idx)
{
	QString	unconfigured("UNCONFIGURED GAME");

	if (!initialized) return (unconfigured);
	if ((idx < 0) || (idx > GAMECFG_CNT)) return (unconfigured);
	if (!gamecfg[idx].active) return (unconfigured);

	return (gamecfg[idx].path);
}

static void
CFG_DLG_data_prepare (CfgDlgData &data, bool isfirstrun)
{
	data.isfirstrun	= isfirstrun;
	data.locprf	= cfg.local;
	data.snp	= cfg.snp_path;
	data.compress	= cfg.compress;
	data.autoload	= cfg.autoload;
	data.fhistory	= cfg.fhistory;
	data.imode	= intelmode2raw(cfg.intel_mode);
	data.hcftype	= cfg.hcftype;
	data.gecross	= cfg.german_cross;
	data.def_game	= cfg.def_gamecfg_idx;
	for (int i=0; i<GAMECFG_CNT; i++) {
		data.games[i].active = gamecfg[i].active;
		data.games[i].name   = gamecfg[i].name;
		data.games[i].type   = gamecfg[i].type;
		data.games[i].path   = gamecfg[i].path;
	}
}

static void
CFG_DLG_data_apply (CfgDlgData &data)
{
	cfg.local		= data.locprf;
	CFG_snp_path_set ((char *)qPrintable (data.snp));
	cfg.compress		= data.compress;
	cfg.autoload		= data.autoload;
	cfg.fhistory		= data.fhistory;
	cfg.intel_mode		= raw2intelmode(data.imode);
	cfg.hcftype		= data.hcftype;
	cfg.german_cross	= data.gecross;
	cfg.def_gamecfg_idx	=data.def_game;
	for (int i=0; i<GAMECFG_CNT; i++) {
		gamecfg[i].active = data.games[i].active;
		gamecfg[i].name	  = data.games[i].name;
		gamecfg[i].type	  = data.games[i].type;
		gamecfg[i].path	  = data.games[i].path;
	}
}

bool
CFG_DLG (bool isfirstrun)
{

	CfgDlg		*dlg = NULL;
	int		rc;

	if (isfirstrun && cfg.local) {
		/* There could be a global config present, preload it here */
		config_load (storage_global);
	}

	CfgDlgData data;

	CFG_DLG_data_prepare (data, isfirstrun);

	dlg = new CfgDlg (&data);
	rc = dlg->exec ();

	if (rc == QDialog::Accepted) {
		CFG_DLG_data_apply (data);
		OOBCFG_update();
		CFG_save();
	}

	delete dlg;

	return (rc == QDialog::Accepted);
}

GameCfgList
CFG_gamecfg_list (int &def_gamecfg)
{
	return (GAMECFG_active_list(def_gamecfg));
}

bool
CFG_valid_gamepath (char *dir, SPWAW_GAME_TYPE gametype, bool &path_exists)
{
	path_exists = false;
	if (!dir) return (false);
	if (!QDir(dir).exists()) return (false);
	path_exists = true;

	return (validate_gamedir (dir, gametype));
}

/* This returns a gamedir from an oobdir, even when the actual oobdir doesn't exist */
QString
CFG_game_from_oob (char *dir, SPWAW_GAME_TYPE gametype)
{
	QString	oobdir, gamedir;

	oobdir = dir;
	gamedir_from_oobdir (oobdir, gametype, gamedir);
	return (gamedir);
}

/* This only returns an oobdir from a gamedir if the gamedir exists */
QString
CFG_oob_from_game (char *dir, SPWAW_GAME_TYPE gametype)
{
	QString	gamedir, oobdir;

	gamedir = dir;
	oobdir_from_gamedir (gamedir, gametype, oobdir);
	return (oobdir);
}

/* This only returns a savedir from a gamedir if the gamedir exists */
QString
CFG_save_from_game (char *dir, SPWAW_GAME_TYPE gametype, SPWAW_DOSSIER_TYPE type)
{
	QString	gamedir, savedir;

	gamedir = dir;
	savedir_from_gamedir (gamedir, gametype, type, savedir);
	return (savedir);
}

/* This only returns a savedir from an oobdir if the oobidr exists */
QString
CFG_save_from_oob (char *dir, SPWAW_GAME_TYPE gametype, SPWAW_DOSSIER_TYPE type)
{
	QString	oobdir, savedir;

	oobdir = dir;
	savedir_from_oobdir (oobdir, gametype, type, savedir);
	return (savedir);
}

static void
statereport (SL_STDBG_INFO_LEVEL level)
{
	SAYSTATE1 ("### %s state:\n", MODULE);

	/* basic information */
	if (level >= SL_STDBG_LEVEL_BAS) {
		SAYSTATE1 ("\tinitialized         = %s\n", SL_BOOL_tostr (initialized));
		SAYSTATE1 ("\tCWD                 = %s\n", cfg.cwd_path);
		SAYSTATE1 ("\tapp path            = %s\n", cfg.app_path);
		SAYSTATE1 ("\tuser data path      = %s\n", cfg.usr_path);
		SAYSTATE1 ("\tuse local config    = %s\n", cfg.local?"yes":"no");
		SAYSTATE1 ("\tsnapshots path      = %s\n", cfg.snp_path);
		SAYSTATE1 ("\tcompression         = %s\n", cfg.compress?"enabled":"disabled");
		SAYSTATE1 ("\tautoload            = %s\n", cfg.autoload?"enabled":"disabled");
		SAYSTATE1 ("\tlast dossier        = %s\n", cfg.lastdoss);
		SAYSTATE1 ("\tfull history        = %s\n", cfg.fhistory?"enabled":"disabled");
		SAYSTATE1 ("\tdefault intel mode  = %s\n", intelmode2str (cfg.intel_mode));
		SAYSTATE1 ("\tdefault hcf type    = %s\n", SMAP_hpmctype2str ((SMAP_HPMC_TYPE)cfg.hcftype));
		SAYSTATE1 ("\tGerman Cross        = %s\n", cfg.german_cross?"enabled":"disabled");
		SAYSTATE1 ("\tGUI state           = %u\n", cfg.gui_state.state);
		SAYSTATE2 ("\tGUI size            = (%d, %d)\n", cfg.gui_state.size->width(), cfg.gui_state.size->height());
		SAYSTATE2 ("\tGUI position        = (%d, %d)\n", cfg.gui_state.pos->x(), cfg.gui_state.pos->y());
		if (cfg.gui_state.state_panes->isEmpty()) {
			SAYSTATE1 ("\tGUI main panes      = %s\n", "empty");
		} else if (cfg.gui_state.state_panes->isNull()) {
			SAYSTATE1 ("\tGUI main panes      = %s\n", "NULL");
		} else {
			SAYSTATE1 ("\tGUI main panes      = %d bytes\n", cfg.gui_state.state_panes->size());
		}
		if (cfg.gui_state.state_main->isEmpty()) {
			SAYSTATE1 ("\tGUI main window     = %s\n", "empty");
		} else if (cfg.gui_state.state_main->isNull()) {
			SAYSTATE1 ("\tGUI main window     = %s\n", "NULL");
		} else {
			SAYSTATE1 ("\tGUI main window     = %d bytes\n", cfg.gui_state.state_main->size());
		}
		SAYSTATE1 ("\tdefault gamecfg idx = %d\n", cfg.def_gamecfg_idx);
	}

	/* extended information */
	if (level >= SL_STDBG_LEVEL_EXT) {
		SAYSTATE1 ("\tstorage_local       = 0x%8.8x\n", storage_local);
		SAYSTATE1 ("\tstorage_global      = 0x%8.8x\n", storage_global);
		SAYSTATE1 ("\tGUI size obj        = 0x%8.8x\n", cfg.gui_state.size);
		SAYSTATE1 ("\tGUI main panes obj  = 0x%8.8x\n", cfg.gui_state.state_panes);
		SAYSTATE1 ("\tGUI main window obj = 0x%8.8x\n", cfg.gui_state.state_main);
		SAYSTATE0 ("\n");
		SAYSTATE1 ("\tOOBCFG[0] type      = %s\n", SPWAW_gametype2str(oobcfg[0].gametype));
		SAYSTATE1 ("\tOOBCFG[0] oobdir    = %s\n", oobcfg[0].oobdir);
		SAYSTATE1 ("\tOOBCFG[1] type      = %s\n", SPWAW_gametype2str(oobcfg[1].gametype));
		SAYSTATE1 ("\tOOBCFG[1] oobdir    = %s\n", oobcfg[1].oobdir);
	}

	/* deep probe */
	if (level >= SL_STDBG_LEVEL_DEEP) {
	}

	SAYSTATE0 ("\n");

	GAMECFG_statereport (level);
}
