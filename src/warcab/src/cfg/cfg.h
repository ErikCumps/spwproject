/** \file
 * The SPWaW war cabinet - configuration handling.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
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
extern bool		CFG_iscomplete		(void);
extern bool		CFG_oobcfg		(SPWAW_OOBCFG **list, int *cnt);

extern char *		CFG_cwd_path		(void);
extern char *		CFG_app_path		(void);
extern char *		CFG_usr_path		(void);
extern SPWAW_GAME_TYPE	CFG_default_gametype	(void);
extern char *		CFG_oob_path		(SPWAW_GAME_TYPE gametype);
extern char *		CFG_save_path		(SPWAW_GAME_TYPE gametype);
extern char *		CFG_snap_path		(void);
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

class CfgGameType
{
public:
	CfgGameType (SPWAW_GAME_TYPE type, const char *name)	: type(type), name(name) {}
	CfgGameType (SPWAW_GAME_TYPE type, QString name)	: type(type), name(name) {}

	SPWAW_GAME_TYPE		type;
	QString			name;
};

extern QList<CfgGameType>	CFG_gametypes	(void);

#endif	/* CFG_COMMON_H */

