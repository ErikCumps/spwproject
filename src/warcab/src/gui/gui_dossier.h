/** \file
 * The SPWaW war cabinet - GUI - dossier treeview.
 *
 * Copyright (C) 2005-2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_DOSSIER_H
#define	GUI_DOSSIER_H	1

#include "gui_private.h"
#include "model/model_dossier.h"

class GuiDossier	: public QTreeView
{
	Q_OBJECT

public:
	GuiDossier	(QWidget *P);
	~GuiDossier	(void);

	SL_ERROR	error_code;

public:
	void			contextMenuEvent	(QContextMenuEvent *event);
	void			setCurrentItem		(MDLD_TREE_ITEM *item);
	MDLD_TREE_ITEM *	get_actionitem		(void);

public slots:
	void	was_loaded		(MDLD_TREE_ITEM *tree);
	void	will_close		(void);
	void	was_closed		(void);
	void	was_added		(MDLD_TREE_ITEM *item);
	void	will_delete		(MDLD_TREE_ITEM *item);
	void	was_deleted		(MDLD_TREE_ITEM *next);
	void	was_edited		(void);

	void	delete_item		(void);

	void	select_parent_item	(void);
	void	select_first_item	(void);
	void	select_prev_item	(void);
	void	select_next_item	(void);
	void	select_last_item	(void);
	void	select_child_item	(void);

	virtual void	currentChanged	(const QModelIndex &current, const QModelIndex &previous);

	void		cmpcurr_changed	(const QModelIndex &current, const QModelIndex &previous);
	void		cmpbase_changed	(const QModelIndex &current, const QModelIndex &previous);

	void	set_cmpcurr	(MDLD_TREE_ITEM *item);
	void	set_cmpbase	(MDLD_TREE_ITEM *item);

private:
	struct s_data {
		ModelDossier	*model;

		MDLD_TREE_ITEM	*actionitem;
	} d;

private:
	MDLD_TREE_ITEM *selected	(void);
	void		refresh		(void);
};

#endif	/* GUI_DOSSIER_H */
