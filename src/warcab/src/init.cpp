/** \file
 * The SPWaW war cabinet - application initialization.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "init.h"
#include "common.h"
#include "gui/gui_private.h"

static void app_exit_atexit (void);

static bool	exited = false;

static SLOPT	SL_options = {
	{	/* event callbacks */
		GUI_EVT_abort,
		GUI_EVT_error,
	},
	{	/* hooks */
		DBG_oom_hook,
		DBG_panic_hook,
		NULL,
		GUI_HOOK_exit,
	}
};

static void
init_SL (int *argc, char **argv, SL_APP_INFO *info)
{
	SL_INIT		rc;
	SL_ERROR	ec;
	
	info->context.argc = *argc;
	info->context.argv = argv;

	rc = SL_init (info, &SL_options);
	if (rc != SL_INIT_OK) {
		char	buffer[512];

		memset (buffer, 0, sizeof (buffer));
		snprintf (buffer, sizeof (buffer) - 1, "SL_init() failed: %s!", SL_INIT_lookup (rc));
		QMessageBox::critical (0, "Initialization error", buffer, QMessageBox::Ok, 0, 0);
		exit (1);
	}

	ec = register_errorcodes ();
	if (SL_HAS_ERROR (ec)) {
		SL_ERROR_HANDLE (SL_ERR_FATAL_ERR, "failed to register application error codes");
	}
}

static void
init_app (SL_APP_INFO *info)
{
	SL_ERROR		ec;
	SPWAW_ERROR		spwe;
	SL_ERROR_REQUEST	req;
	bool			initial;

	/* Initialize debug logging */
	DBG_init ();

	DBG_log ("initializing CFG subsystem\n");
	ec = CFG_init ();
	if (SL_HAS_ERROR (ec)) {
		SL_ERROR_HANDLE (SL_ERR_FATAL_ERR, "failed to initialize configuration");
		SL_DIE (SL_EXIT_FATAL_ERROR, "configuration initialization error could not be ignored!");
	}

	DBG_log ("initializing RES subsystem\n");
	ec = RES_init ();
	if (SL_HAS_ERROR (ec)) {
		SL_ERROR_HANDLE (SL_ERR_FATAL_ERR, "failed to initialize resources");
		SL_DIE (SL_EXIT_FATAL_ERROR, "resource initialization error could not be ignored!");
	}

	DBG_log ("verifying configuration revision\n");
	if (CFG_needsreview(initial)) {
		if (initial) {
			QMessageBox::information (NULL, SL_APP_name(),
				"The application is run for the first time! \n\n"
				"Please review the application preferences... ",
				QMessageBox::Ok, QMessageBox::Ok);

		} else {
			QMessageBox::information (NULL, SL_APP_name(),
				"The application was updated! \n\n"
				"Please review the application preferences... ",
				QMessageBox::Ok, QMessageBox::Ok);
		}
		CFG_DLG();
	}

	DBG_log ("verifying configuration completeness\n");
	while (1) {
		if (CFG_iscomplete()) break;

		HANDLE_ERR_FUNCTION_EX (SL_ERR_FATAL_SOFTERR,
			"The application preferences are incomplete!\n"
			"Select <retry> to review the application preferences and try again.\n",
			ERR_CFG_INCOMPLETE,
			NULL, NULL, req);
		if (req != SL_ERR_REQUEST_RETRY) {
			SL_DIE (SL_EXIT_FATAL_ERROR, "This application preferences completeness error could not be ignored!");
		}
		CFG_DLG();
	}

	DBG_log ("initializing SPWAW library\n");
	while (1) {
		SPWAW_OOBCFG	*oobcfg_ptr = NULL;
		int		oobcfg_cnt = 0;
		CFG_oobcfg (&oobcfg_ptr, &oobcfg_cnt);

		if ((spwe = SPWAW_init (oobcfg_ptr, oobcfg_cnt, false)) == SPWERR_OK) break;

		if (spwe == SPWERR_NOOOBFILES) {
			HANDLE_ERR_FUNCTION_EX (SL_ERR_FATAL_SOFTERR,
				"Failed to initialize SPWAWLIB:\n"
				"The OOB folder specified in the application preferences does not contain valid OOB files!\n\n"
				"Select <retry> to review the application preferences and try again.\n",
				ERR_APP_INIT_FAILED,
				"SPWAWLIB error: %s\n", SPWAW_errstr (spwe),
				req);
			if (req != SL_ERR_REQUEST_RETRY) {
				SL_DIE (SL_EXIT_FATAL_ERROR, "This SPWAWLIB initialization error could not be ignored!");
			} else {
				CFG_DLG();
			}
		} else {
			HANDLE_ERR_FUNCTION_EX (SL_ERR_FATAL_ERR,
				"Failed to initialize SPWAWLIB!",
				ERR_APP_INIT_FAILED,
				"SPWAWLIB error: %s", SPWAW_errstr (spwe),
				req);
		}
	}

	DBG_log ("initializing WARCAB state\n");
	ec = WARCAB_init (info);
	if (SL_HAS_ERROR (ec)) {
		SL_ERROR_HANDLE (SL_ERR_FATAL_ERR, "failed to initialize application state");
		SL_DIE (SL_EXIT_FATAL_ERROR, "application state initialization error could not be ignored!");
	}

	DBG_log ("application started\n");
}

void
app_init (int *argc, char **argv, SL_APP_INFO *info)
{
	/* Register our shutdown routine */
	exited = false;
	atexit (app_exit_atexit);

	/* Initialize GUI */
	GUI_init (argc, argv);

	/* Initialize SL library */
	init_SL (argc, argv, info);

	/* Initialize application */
	init_app (info);
}

static void
exit_SL (void)
{
	SL_shutdown ();
}

static void
exit_app (int rc)
{
	WARCAB_shutdown ();
	SPWAW_shutdown ();
	RES_shutdown ();	// TODO: isn't this dangerous? Doesn't GUI use RES?
	if (rc != APP_EXIT_UNEXPECTED) CFG_shutdown ();
	DBG_shutdown ();
}

void
app_exit (int rc)
{
	DBG_log ("terminating application with exit code %d\n", rc);

	/* Shut down application */
	exit_app (rc);

	/* Shut down SL library */
	exit_SL ();

	/* Shut down GUI */
	GUI_exit ();

	/* Flag exit to neutralize atexit handler */
	exited = true;

	exit (rc);
}

static void
app_exit_atexit (void)
{
	if (!exited) app_exit (APP_EXIT_UNEXPECTED);
}
