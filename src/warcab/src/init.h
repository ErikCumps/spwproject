/** \file
 * The SPWaW war cabinet - application initialization.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INIT_H
#define	INIT_H	1

#define	APP_EXIT_OK		0
#define	APP_EXIT_ERROR		1
#define	APP_EXIT_UNEXPECTED	100

extern void	app_init	(int *argc, char **argv, SL_APP_INFO *info);
extern void	app_exit	(int rc);

#endif	/* INIT_H */