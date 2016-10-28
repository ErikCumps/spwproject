/** \file
 * The SPWaW war cabinet - plot handling - legend.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "plot_legend.h"

PlotLegend::PlotLegend (bool active)
	: QwtLegend()
{
	DBG_TRACE_CONSTRUCT;

	if (active) setItemMode (QwtLegend::CheckableItem);

	SET_GUICLS_NOERR;
}

PlotLegend::~PlotLegend (void)
{
	DBG_TRACE_DESTRUCT;
}
