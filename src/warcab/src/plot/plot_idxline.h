/** \file
 * The SPWaW war cabinet - plot handling - axis index labels.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef PLOT_IDXLINE_H
#define PLOT_IDXLINE_H	1

#include "plot.h"

class PlotIdxline	: public QwtScaleDraw
{
public:
	PlotIdxline	(int *cnt_ptr, double **lst_ptr);
	~PlotIdxline	(void);

	SL_ERROR	error_code;

public:
	virtual QwtText	label	(double v) const;

private:
	struct s_data {
		int	*cnt;
		double	**lst;
	} d;
};

#endif /* PLOT_IDXLINE_H */
