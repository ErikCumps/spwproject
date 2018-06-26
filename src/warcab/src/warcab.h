/** \file
 * The SPWaW war cabinet - application state handling.
 *
 * Copyright (C) 2005-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	WARCAB_H
#define	WARCAB_H	1

#include "stdafx.h"

#include "mdld_tree.h"

/* application state: options */
typedef struct s_WARCABOptions {
	char	*load;
} WARCABOptions;

class WARCABState	: public QObject
{
	Q_OBJECT

public:
	WARCABState	(SL_APP_INFO *info);
	~WARCABState	();

public:
	SL_ERROR	init	(void);
	SL_ERROR	mknew	(void);
	SL_ERROR	load	(SPWAW_DOSSLIST *list);
	SL_ERROR	save	(void);
	SL_ERROR	saveas	(char *file);
	SL_ERROR	close	(void);
	SL_ERROR	add	(SPWAW_SAVELIST *list);
	SL_ERROR	add	(SPWAW_SNAPLIST *list);
	SL_ERROR	del	(MDLD_TREE_ITEM *item);
	SL_ERROR	edit	(char *name, char *comment);

	bool		needs_postedinit(void);
	bool		is_loaded	(void);
	bool		is_dirty	(void);
	char		*get_filename	(void);
	char		*get_name	(void);
	char		*get_comment	(void);
	SPWAW_SAVELIST	*get_gamelist	(void);
	SPWAW_SNAPLIST	*get_snaplist	(void);

	void		statereport	(SL_STDBG_INFO_LEVEL level);

signals:
	void		was_loaded	(MDLD_TREE_ITEM *tree);
	void		will_close	(void);
	void		was_closed	(void);
	void		was_added	(MDLD_TREE_ITEM *item);
	void		will_delete	(MDLD_TREE_ITEM *item);
	void		was_deleted	(MDLD_TREE_ITEM *next);
	void		was_edited	(void);

private:
	struct s_data {
		WARCABOptions	options;
		char		filename[MAX_PATH+1];
		SPWAW_DOSSIER	*dossier;
		MDLD_TREE_ITEM	*tree;
		SPWAW_SAVELIST	*gamelist;
		SPWAW_SNAPLIST	*snaplist;
		SL_BOOL		dirty;
	} d;

private:
	void		set_dirty		(bool flag);
	void		set_name		(char *name);
	SL_ERROR	refresh_savelists	(void);
	void		setup_tree		(void);
	void		setup_tree_data		(MDLD_TREE_ITEM *tree, bool campaign);
	void		refresh_tree		(void);
	void		refresh_tree_data	(MDLD_TREE_ITEM *tree, bool campaign);
	void		free_tree		(void);
	void		free_tree_children	(MDLD_TREE_ITEM *tree);
	MDLD_TREE_ITEM *item_from_turn		(SPWAW_BTURN *turn);
	MDLD_TREE_ITEM *next_safe_item		(MDLD_TREE_ITEM *item);
};

extern WARCABState	*WARCAB;

extern SL_ERROR		WARCAB_init		(SL_APP_INFO *info);
extern void		WARCAB_shutdown		(void);

#endif	/* WARCAB_H */
