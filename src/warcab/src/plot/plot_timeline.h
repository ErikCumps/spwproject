/** \file
 * The SPWaW war cabinet - plot handling - axis timeline labels.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef PLOT_TIMELINE_H
#define PLOT_TIMELINE_H	1

#include "plot.h"

class PlotTimeline	: public QwtScaleDraw
{
public:
	PlotTimeline	(void);
	~PlotTimeline	(void);

	SL_ERROR	error_code;

public:
	virtual QwtText	label	(double v) const;
};

extern void	PLOT_date2timeline	(SPWAW_DATE &date, double &timeline);
extern void	PLOT_date2timeline	(SPWAW_BATTLE_DATE &date, double &timeline);
extern void	PLOT_date2timeline	(SPWAW_TURN_DATE &date, double &timeline);
extern void	PLOT_timeline2date	(double &timeline, SPWAW_DATE &date);

extern bool	PLOT_hasBtlidx		(double timeline);
extern USHORT	PLOT_decodeBtlidx	(double timeline, short year, char month);
extern void	PLOT_encodeBtlidx	(double &timeline, USHORT btlidx);

extern bool	PLOT_hasBtlidx		(SPWAW_DATE &date);
extern USHORT	PLOT_decodeBtlidx	(SPWAW_DATE &date);
extern void	PLOT_encodeBtlidx	(SPWAW_DATE &date, USHORT btlidx);

#endif /* PLOT_TINELINE_H */
