/** \file
 * The SPWaW war cabinet - savegame file tracking.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	TRACKING_H
#define	TRACKING_H	1

#include "stdafx.h"

#include "warcab.h"

class WARCABTracking	: public QObject
{
	Q_OBJECT

public:
	WARCABTracking	(WARCABState *warcab);
	~WARCABTracking	();

public:
	void		statereport	(SL_STDBG_INFO_LEVEL level);
	bool		is_ready	(void);

private:
	struct s_data {
		SL_BOOL			ready;
		WARCABState		*warcab;
		QFileSystemWatcher	*watcher;
		volatile SL_BOOL	active;
		SPWAW_DOSSIER_TRACKING	tracking;
		char			status_base[512];
		QDateTime		*processed;
	} d;

private:
	typedef enum e_STATE {
		INACTIVE = 0,
		TRACKING,
		ADDING,
		ADDED,
		FAILED,
	} STATE;

private:
	void		activate	(SPWAW_DOSSIER_TRACKING &tracking);
	void		deactivate	();
	void		set_status	(STATE state);
	void		tracking_set	(SPWAW_DOSSIER_TRACKING &tracking);
	void		tracking_clear	(void);

private slots:
	void		loaded		(MDLD_TREE_ITEM *tree);
	void		closed		(void);
	void		added		(MDLD_TREE_ITEM *item);
	void		watcher_event	(const QString& path);};

#endif	/* TRACKING_H */
