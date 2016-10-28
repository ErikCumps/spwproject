/** \file
 * The SPWaW war cabinet - configuration handling.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	CFG_COMMON_H
#define	CFG_COMMON_H	1

#include "gui/gui_state.h"

extern SL_ERROR		CFG_init		(void);
extern void		CFG_shutdown		(void);

extern bool		CFG_needsreview		(bool &isfirstrun);
extern bool		CFG_iscomplete		(void);

extern char *		CFG_cwd_path		(void);
extern char *		CFG_app_path		(void);
extern char *		CFG_usr_path		(void);
extern char *		CFG_oob_path		(void);
extern char *		CFG_save_path		(void);
extern char *		CFG_snap_path		(void);
extern bool		CFG_compress		(void);

extern char *		CFG_autoload_get	(void);
extern void		CFG_autoload_set	(char *file);

extern GUI_STATE *	CFG_gui_state_get	(void);
extern void		CFG_gui_state_set	(GUI_STATE &state);

extern bool		CFG_DLG			(void);

#endif	/* CFG_COMMON_H */

