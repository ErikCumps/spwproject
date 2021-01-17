/** \file
 * The SPWaW war cabinet - configuration handling.
 *
 * Copyright (C) 2005-2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	CFG_COMMON_H
#define	CFG_COMMON_H	1

#include "gui/gui_state.h"
#include "intel_mode.h"

extern SL_ERROR		CFG_init		(void);
extern void		CFG_shutdown		(void);

extern bool		CFG_needsreview		(bool &isfirstrun);
extern bool		CFG_iscomplete		(bool msgbox);
extern bool		CFG_oobcfg		(SPWAW_OOBCFG **list, int *cnt);

extern char *		CFG_cwd_path		(void);
extern char *		CFG_app_path		(void);
extern char *		CFG_usr_path		(void);
extern char *		CFG_snp_path		(void);
extern bool		CFG_compress		(void);

extern char *		CFG_autoload_get	(void);
extern void		CFG_autoload_set	(char *file);

extern bool		CFG_full_history	(void);
extern INTEL_MODE	CFG_intel_mode		(void);
extern int		CFG_hcftype		(void);
extern bool		CFG_german_cross	(void);

extern GUI_STATE *	CFG_gui_state_get	(void);
extern void		CFG_gui_state_set	(GUI_STATE &state);

extern bool		CFG_DLG			(bool isfirstrun);

typedef	int		GameCfgIdx;

#define	GAMECFG_IDX_NOT_SET	-1

extern QString		CFG_gamename		(GameCfgIdx idx);
extern SPWAW_GAME_TYPE	CFG_gametype		(GameCfgIdx idx);
extern QString		CFG_gamepath		(GameCfgIdx idx);

class GameCfg
{
public:
	GameCfg (QString name, SPWAW_GAME_TYPE type, QString path)	: name(name), type(type), path(path) {}

	QString			name;
	SPWAW_GAME_TYPE		type;
	QString			path;
};

typedef	QList<GameCfg>	GameCfgList;

extern GameCfgList	CFG_gamecfg_list	(int &def_gamecfg);

extern	bool		CFG_valid_gamepath	(char *dir, SPWAW_GAME_TYPE gametype, bool &path_exists);

/* This returns a gamedir from an oobdir, even when the actual oobdir doesn't exist */
extern	QString		CFG_game_from_oob	(char *dir, SPWAW_GAME_TYPE gametype);

/* This only returns an oobdir from a gamedir if the gamedir exists */
extern	QString		CFG_oob_from_game	(char *dir, SPWAW_GAME_TYPE gametype);

/* This only returns a savedir from a gamedir if the gamedir exists */
extern	QString		CFG_save_from_game	(char *dir, SPWAW_GAME_TYPE gametype, SPWAW_DOSSIER_TYPE type);

/* This only returns a savedir from an oobdir if the oobidr exists */
extern	QString		CFG_save_from_oob	(char *dir, SPWAW_GAME_TYPE gametype, SPWAW_DOSSIER_TYPE type);


#endif	/* CFG_COMMON_H */

