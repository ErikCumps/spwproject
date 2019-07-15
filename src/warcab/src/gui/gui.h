/** \file
 * The SPWaW war cabinet - GUI.
 *
 * Copyright (C) 2005-2017 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_COMMON_H
#define	GUI_COMMON_H	1

#include "gui/gui_msgbox.h"
#include "gui/gui_errorbox.h"
#include "gui/gui_progress.h"

class	GuiMainWindow;
class	GuiActions;
class	GuiMainMenu;
class	GuiToolbar;
class	GuiStatus;
class	GuiDossier;
class	GuiDlgEditDossier;
class	GuiOobView;
class	GuiOob;
class	GuiRosterView;
class	GuiRoster;
class	GuiHistoryView;
class	GuiUnitlistView;
class	GuiHistory;
class	GuiMMASView;
class	GuiMMAS;
class	GuiTableView;
class	GuiRptOob;
class	GuiRptRst;
class	GuiRptHst;
class	GuiRptMMAS;
class	GuiRptDsrSts;
class	GuiRptDsrFrc;
class	GuiRptDsrOvr;
class	GuiRptDsr;
class	GuiRptBtlCmp;
class	GuiRptBtlFrc;
class	GuiRptBtlOvr;
class	GuiRptBtl;
class	GuiRptTrnCmp;
class	GuiRptTrnFrc;
class	GuiRptTrnOvr;
class	GuiRptTrn;
class	GuiMainRpt;

extern void		GUI_init	(int *argc, char **argv);
extern void		GUI_exit	(void);
extern int		GUI_exec	(void);

extern SL_EVT_RC	GUI_EVT_abort	(const char *file, unsigned long line, const char *func, const char *msg);
extern void		GUI_HOOK_exit	(unsigned int ec, char *msg, SL_BOOL dump);
extern SL_ERROR_REQUEST	GUI_EVT_error	(SL_ERROR_DATA *stack);

#endif	/* GUI_COMMON_H */

