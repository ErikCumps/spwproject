/** \file
 * A simple tool to extract tiles from an image file.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef MAIN_H
#define MAIN_H	1

#include "stdafx.h"

#define	snprintf	_snprintf

typedef struct s_CONFIG {
	char	src[MAX_PATH+1];
	char	stm[MAX_PATH+1];
	char	ext[MAX_PATH+1];
	int	w;
	int	h;
	int	x;
	int	y;
	int	sw;
	int	sh;
	int	sx;
	int	sy;
} CONFIG;

#endif	/* MAIN_H */

