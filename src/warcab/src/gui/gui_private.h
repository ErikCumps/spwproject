/** \file
 * The SPWaW war cabinet - GUI - private header file.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_PRIVATE_H
#define	GUI_PRIVATE_H	1

#include "../common.h"

#include "gui_mainwindow.h"
#include "gui_actions.h"
#include "gui_mainmenu.h"
#include "gui_toolbar.h"
#include "gui_status.h"
#include "gui_dossier.h"
#include "gui_mainreport.h"
#include "model/model_filter.h"

/*! Minimum time to show splash screen at startup, in ms */
#define	GUI_SPLASH_TIME	2000

/*! Type code for GUI initialization QEvent */
#define	GUI_EVENT_INIT	(QEvent::User)

extern QApplication	*GUI_APP;
extern GuiMainWindow	*GUI_WIN;

#define	GUI_ACTIONS	GUI_WIN->get_actions()

#define	SET_GUICLS_NOERR											\
	do { error_code = SL_ERR_NO_ERROR; SL_ERROR_none (); return; } while (0)

#define	SET_GUICLS_ERROR(rc_,extra_)										\
	do { error_code = rc_; SET_ERR_FUNCTION (error_code, extra_); return; } while (0)

#define	GUINEW(var_,what_,err_,msg_)										\
	do {													\
		if (!(var_ = new what_))									\
			SET_GUICLS_ERROR (err_, "failed to create <" #msg_ "> GUI element");			\
	} while (0)

#define	GUIERR(obj_,err_)											\
	do {													\
		if (SL_HAS_ERROR (obj_->error_code))								\
			SET_GUICLS_ERROR (err_, "failed to create <" #obj_ "> object");				\
	} while (0)

#define GUIROE(obj_)												\
	do { if (SL_HAS_ERROR (obj_->error_code)) return; } while (0)

static inline bool
is_initial_empty_index (const QModelIndex &idx)
{
	return ((idx.row() == -1) && (idx.column() == -1) && (idx.internalPointer() == NULL));
}

static inline void
dbg_log_index (const QModelIndex &idx)
{
	DBG_log ("idx=0x%8.8x {r=%d, c=%d, p=0x%8.8x, m=0x%8.8x}", &idx, idx.row(), idx.column(), idx.internalPointer(), idx.model());
}

typedef struct s_GUI_FILTER_TARGET {
	MDL_MF_TGTLST	*list;
	int		curr;
} GUI_FILTER_TARGET;

/* GUI reference and value tracking ***/

#include "util/util_mdltree_reftrack.h"
#include "util/util_valtrack.h"

#define	GUIVALTRACK(type_,name_)		\
	type_			name_;		\
	UtilValtrack<type_>	track_##name_;

#define	GUIVALCHANGED(name_)	d.track_##name_.changed (d.##name_)

#endif	/* GUI_PRIVATE_H */
