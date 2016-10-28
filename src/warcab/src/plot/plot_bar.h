/** \file
 * The SPWaW war cabinet - plot handling - bar-type plot.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef PLOT_BAR_H
#define PLOT_BAR_H	1

#include "plot.h"
#include "plot_bar_item.h"

class PlotBar	: public PlotBarItem
{

public:
	PlotBar		(char *name, void *parent, PLOT_TYPE type = PLOT_BAR, QColor * color = NULL, int alpha = 255, int width = 0);
	~PlotBar	(void);

	SL_ERROR	error_code;

public:
	void	set	(QColor * color, int alpha, int width);
	char	*name	(void);
	void	*parent	(void);

private:
	struct s_data {
		char		*name;
		PLOT_TYPE	type;
		void		*prnt;
	} d;
};

#endif /* PLOT_BAR_H */
