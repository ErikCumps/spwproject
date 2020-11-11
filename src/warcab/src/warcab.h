/** \file
 * The SPWaW war cabinet - application state handling.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	WARCAB_H
#define	WARCAB_H	1

#include "stdafx.h"

#include "mdld_tree.h"
#include "gui/gui.h"

class WARCABTracking;

/* application state: options */
typedef struct s_WARCABOptions {
	char	*load;
} WARCABOptions;

class WARCABState	: public QObject
{
	Q_OBJECT

friend class WARCABTracking;

public:
	WARCABState	(SL_APP_INFO *info);
	~WARCABState	();

public:
	SL_ERROR	init		(void);
	SL_ERROR	mknew		(SPWAW_GAME_TYPE gametype, const char *name, const char *comment);
	SL_ERROR	load		(SPWAW_DOSSLIST *list);
	SL_ERROR	save		(void);
	SL_ERROR	saveas		(char *file);
	SL_ERROR	close		(void);
	SL_ERROR	add_campaign	(SPWAW_SAVELIST *list, bool &added);
	SL_ERROR	add_stdalone	(char *name, SPWAW_SAVELIST *list, bool &added);
	SL_ERROR	add_stdalone	(SPWAW_BATTLE *battle, SPWAW_SAVELIST *list, bool &added);
#if	ALLOW_SNAPSHOTS_LOAD
	SL_ERROR	add_campaign	(SPWAW_SNAPLIST *list, bool &added);
	SL_ERROR	add_stdalone	(char *name, SPWAW_SNAPLIST *list, bool &added);
	SL_ERROR	add_stdalone	(SPWAW_BATTLE *battle, SPWAW_SNAPLIST *list, bool &added);
#endif	/* ALLOW_SNAPSHOTS_LOAD */
	SL_ERROR	del		(MDLD_TREE_ITEM *item);
	SL_ERROR	edit		(char *name, char *comment);

	bool		needs_postedinit(void);
	bool		is_loaded	(void);
	bool		is_dirty	(void);
	char		*get_filename	(void);
	char		*get_name	(void);
	char		*get_comment	(void);
	SPWAW_SAVELIST	*get_gamelist	(void);
#if	ALLOW_SNAPSHOTS_LOAD
	SPWAW_SNAPLIST	*get_snaplist	(void);
#endif	/* ALLOW_SNAPSHOTS_LOAD */
	SPWAW_GAME_TYPE	get_gametype	(void);
	bool		can_track	(void);
	SPWAW_DOSSIER	*get_dossier	(void);
	QString &	get_savedir	(SPWAW_DOSSIER_TYPE type);
	void		set_savedir	(void);

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
#if	ALLOW_SNAPSHOTS_LOAD
		SPWAW_SNAPLIST	*snaplist;
#endif	/* ALLOW_SNAPSHOTS_LOAD */
		SL_BOOL		dirty;
		WARCABTracking	*tracking;
	} d;
	struct s_objects {
		QString		savedir;
	} o;

private:
	void		set_dirty		(bool flag);
	void		set_name		(char *name);
	SL_ERROR	refresh_savelists	(void);
	void		setup_tree		(void);
	//void		setup_tree_data		(MDLD_TREE_ITEM *tree, bool campaign);
	void		setup_tree_data		(MDLD_TREE_ITEM *tree, SPWAW_DOSSIER_TYPE type, SPWAW_GAME_TYPE gametype);
	void		refresh_tree		(void);
	//void		refresh_tree_data	(MDLD_TREE_ITEM *tree, bool campaign);
	void		refresh_tree_data	(MDLD_TREE_ITEM *tree, SPWAW_DOSSIER_TYPE type);
	void		free_tree		(void);
	void		free_tree_children	(MDLD_TREE_ITEM *tree);
	MDLD_TREE_ITEM *item_from_turn		(SPWAW_BTURN *turn);
	MDLD_TREE_ITEM *next_safe_item		(MDLD_TREE_ITEM *item);

private:
	typedef struct {
		union {
			SPWAW_SAVELIST *save;
			SPWAW_SNAPLIST *snap;
		} list;
		bool		savelist;
		SPWAW_GAME_TYPE	gametype;
	} PL_LIST;
	typedef SPWAW_ERROR (*PL_ADD)(void *, SPWAW_SNAPSHOT *, SPWAW_BTURN **);
	SL_ERROR	process_list	(PL_LIST &list, PL_ADD add, void *context, GuiProgress &gp, bool &added);

	SPWAW_ERROR	basic_campaign_add	(SPWAW_SAVEGAME_DESCRIPTOR *sgd);
};

extern WARCABState	*WARCAB;

extern SL_ERROR		WARCAB_init		(SL_APP_INFO *info);
extern void		WARCAB_shutdown		(void);

#endif	/* WARCAB_H */
