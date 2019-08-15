/** \file
 * The SPWaW war cabinet.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "common.h"
#include "build.h"
#include "init.h"

#include <debug_memtrace_delete.h>

static SL_APP_INFO	app_info = {
	{
		"Warcab",
		"The SP:WaW and winSPWW2 war cabinet",
		"0.8.0-BETA#7",
		"2005-2019",
		"Erik Cumps",
		"erik.cumps@gmail.com"
	},
	{
		(char *)BLD_BUILD, (char *)BLD_DATE, (char *)BLD_TIME
	},
};

int
main (int argc, char **argv)
{
	int		rc = 0;

	MEMTRACE_INIT ("./warcab_memtrace.log");

	/* Initialize */
	app_init (&argc, argv, &app_info);

#if	TESTING

	TEST_CFG	cfg;

	memset (&cfg, 0, sizeof (cfg));
	cfg.error.panic = SL_false;
	cfg.error.threads = SL_false;
	cfg.assrt.dev = SL_false;
	test_suite (&cfg);

#else	/* !TESTING */

	rc = GUI_exec ();

#endif	/* !TESTING */

	/* Shut down */
	if (rc) rc = APP_EXIT_ERROR;
	app_exit (rc);

	DBG_log ("exit code %d\n", rc);
	return (rc);
}
