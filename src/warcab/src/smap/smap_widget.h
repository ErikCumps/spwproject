/** \file
 * The SPWaW war cabinet - strategic map - widget.
 *
 * Copyright (C) 2012-2016 Erik Cumps <erik.cumps@gmail.com>
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
	void	load			(SPWAW_SNAPSHOT *snap);
	void	clear			(void);
	void	reset_cursor		(void);

public:
	void	enable_grid		(bool enable);
	void	enable_victoryhexes	(bool enable);
	void	enable_influence	(bool enable);
	void	enable_frontline	(bool enable);

public:
	typedef enum e_ZOOMLEVEL {
		ZOOM_1X = 0,
		ZOOM_2X,
		ZOOM_LIMIT
	} ZOOMLEVEL;

	void	set_zoom		(ZOOMLEVEL zoom);

signals:
	void	wouldscroll		(int x, int y);

public slots:
	void	askscroll		(void);

protected:
	void	paintEvent		(QPaintEvent *e);
	void	mouseReleaseEvent	(QMouseEvent *e);

	//TEST
	void	contextMenuEvent(QContextMenuEvent *event);
	//TEST

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
	} UNIT_INFO;

	struct s_data {
		ModelTable	*model;
		MDLT_DEF	*model_def;

		SmapRenderer	*renderlist[ZOOM_LIMIT];
		ZOOMLEVEL	zoom;
		SmapRenderer	*rptr;

		bool		o_grid;
		bool		o_influence;
		bool		o_vhex;
		bool		o_frontline;

		SmapHexGrid	grid;

		int		b_cnt;
		UNIT_INFO	*b_info;

		int		r_cnt;
		UNIT_INFO	*r_info;

		SmapHexPos	cursor;

		// DEBUG
		int		hpx;
		int		hpy;
	} d;

private:
	void	setcursor	(SmapHexPos &pos);
	void	setrender	(ZOOMLEVEL zoom);
	void	calc_bridgecon	(void);
	void	calc_influence	(void);
	void	calc_frontline	(void);
};

#endif /* STRATMAP_WIDGET_H */