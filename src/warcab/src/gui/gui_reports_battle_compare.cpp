/** \file
 * The SPWaW war cabinet - GUI - battle report - comparisons.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "gui_reports_battle.h"

GuiRptBtlCmp::GuiRptBtlCmp (QWidget *P)
	: QScrollArea (P)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	GUINEW (d.font, QFont ("Courier", 8, QFont::Normal, false), ERR_GUI_REPORTS_INIT_FAILED, "font");

	GUINEW (d.frame, QFrame (this), ERR_GUI_REPORTS_INIT_FAILED, "frame");
	GUINEW (d.layout, QGridLayout (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "layout");

	GUINEW (d.label, QLabel (d.frame), ERR_GUI_REPORTS_INIT_FAILED, "label");
	d.label->setAlignment (Qt::AlignLeft|Qt::AlignTop);
	d.label->setMargin (10);
	d.label->setWordWrap (true);
	d.label->setFont (*d.font);

	d.layout->addWidget (d.label, 0, 0);

	setWidget(d.frame);
	setWidgetResizable (true);

	if (!connect (this, SIGNAL (cmpcurr(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpcurr(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <cmpcurr> to <dossier:set_cmpcurr>");

	if (!connect (this, SIGNAL (cmpbase(MDLD_TREE_ITEM*)), GUI_WIN->get_dossier(), SLOT (set_cmpbase(MDLD_TREE_ITEM*))))
		SET_GUICLS_ERROR (ERR_GUI_REPORTS_INIT_FAILED, "failed to connect <cmpbase> to <dossier:set_cmpbase>");

	SET_GUICLS_NOERR;
}

GuiRptBtlCmp::~GuiRptBtlCmp (void)
{
	DBG_TRACE_DESTRUCT;

	// QT deletes child widgets
	delete d.font;
}

void
GuiRptBtlCmp::set_parent (GuiRptBtl *p)
{
	d.parent = p;
}

void
GuiRptBtlCmp::refresh (bool /*forced*/)
{
	DBG_TRACE_FENTER;

	d.label->setText ("No comparisons available yet.");

	if (isVisible()) {
		emit cmpcurr (NULL);
		emit cmpbase (NULL);
	}

	DBG_TRACE_FLEAVE;
}
