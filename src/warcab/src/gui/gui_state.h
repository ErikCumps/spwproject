/** \file
 * The SPWaW war cabinet - GUI - GUI state.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_STATE_H
#define	GUI_STATE_H	1

typedef struct s_GUI_STATE {
	Qt::WindowStates	state;
	QSize			*size;
	QPoint			*pos;
	QByteArray		*state_panes;
	QByteArray		*state_main;
} GUI_STATE;

#endif	/* GUI_STATE_H */
