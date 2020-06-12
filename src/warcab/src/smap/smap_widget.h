/** \file
 * The SPWaW war cabinet - strategic map - widget.
 *
 * Copyright (C) 2012-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	STRATMAP_WIDGET_H
#define	STRATMAP_WIDGET_H	1

#include "gui/gui_private.h"
#include "smap_hexgrid.h"
#include "smap_renderer.h"
#include "model/model_table.h"

class SmapWidget	: public QFrame
{
	Q_OBJECT

public:
	SmapWidget	(ModelTable *model, QWidget *P=0);
	~SmapWidget	(void);

	SL_ERROR	error_code;

public:
	QSize	sizeHint() const;

public:
	void	load			(SPWAW_BTURN *turn, SMAP_HPMC_TYPE type = SMAP_HPMC_GREY);
	void	clear			(void);
	void	reset_cursor		(void);
	void	save_smap		(void);

public:
	void	enable_grid		(bool enable, bool paint);
	void	enable_victoryhexes	(bool enable, bool paint);
	void	enable_influence	(bool enable, bool paint);
	void	enable_frontline	(bool enable, bool paint);
	void	select_hcf		(SMAP_HPMC_TYPE type, bool paint);
	void	set_intel_mode		(INTEL_MODE mode, bool paint);

public:
	typedef enum e_ZOOMLEVEL {
		ZOOM_1X = 0,
		ZOOM_2X,
		ZOOM_LIMIT
	} ZOOMLEVEL;

	void	set_zoom		(ZOOMLEVEL zoom, bool paint);

public:
	void	trigger_repaint		(void);

signals:
	void	wouldscroll		(int x, int y);

public slots:
	void	askscroll		(void);

protected:
	void	paintEvent		(QPaintEvent *e);
	void	mouseReleaseEvent	(QMouseEvent *e);
	void	contextMenuEvent(QContextMenuEvent *event);

public:
	typedef struct s_COMPOSITOR_CFG {
		bool		grid;
		bool		influence;
		bool		vhex;
		bool		frontline;
		SMAP_HPMC_TYPE	hcf;
	} COMPOSITOR_CFG;

	typedef struct s_BATTLE_INFO {
		char		*location;
		SPWAW_TURN_DATE	date;
		SPWAW_GAME_TYPE	gametype;
		SPWAW_TERRAIN	terrain;
		SMAP_HH		max_height;
	} BATTLE_INFO;

private:
	typedef struct s_UNIT_INFO {
		SmapHexPos	pos;
		QString		uid;
		QString		unit;
		double		ready;
		int		damage;
		int		hcnt;
		int		morale;
		int		suppression;
		QString		status;
		QString		spotted;
		QString		entrenchment;
		QString		contact;
		bool		KIA;
	} UNIT_INFO;

	typedef struct s_GRID_INFO {
		SmapHexGrid	grid;
		int		b_cnt;
		UNIT_INFO	*b_info;
		int		r_cnt;
		UNIT_INFO	*r_info;
	} GRID_INFO;

	struct s_data {
		ModelTable	*model;
		MDLT_DEF	*model_def;

		SMAP_RENDERDATA	rdlist[ZOOM_LIMIT];
		SmapRenderer	*renderlist[ZOOM_LIMIT];
		ZOOMLEVEL	zoom;
		SmapRenderer	*rptr;

		COMPOSITOR_CFG	comp_cfg;

		BATTLE_INFO	battle;

		GRID_INFO	info_full;
		GRID_INFO	info_lmtd;
		GRID_INFO	info_none;
		GRID_INFO	*info;

		SmapHexPos	cursor;

		INTEL_MODE	intel_mode;
	} d;

private:
	void	clear_grid_info	(GRID_INFO &info);
	void	apply_intelmode	(bool paint);
	void	load_info_full	(SPWAW_BTURN *turn);
	void	load_info_lmtd	(SPWAW_BTURN *turn);
	void	load_info_none	(SPWAW_BTURN *turn);
	void	calc_bridgecon	(GRID_INFO &info);
	void	calc_influence	(GRID_INFO &info);
	void	calc_frontline	(GRID_INFO &info);
	void	applycursor	(void);
	void	setcursor	(SmapHexPos &pos);
	void	setrender	(ZOOMLEVEL zoom);
	void	composite	(QPainter &paint,  COMPOSITOR_CFG &cfg);
};

#endif /* STRATMAP_WIDGET_H */
