/** \file
 * Slay's Library - application termination handling.
 *
 * Copyright (C) 2003-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "internal_debug.h"
#include <SL/system.h>
#include <SL/bool.h>
#include <SL/message.h>
#include <SL/stdbg.h>
#include <SL/app.h>
#include <SL/app_exit.h>
#include "internal_opts.h"



/* --- forward declarations --- */

static void statereport (SL_STDBG_INFO_LEVEL level);


/* --- private variables --- */

/*! Module name */
static const char *MODULE = "SL_EXIT";

/*! Initialization status */
static SL_BOOL	initialized = SL_false;


/* --- code --- */

SL_SINIT
SL_EXIT_init (void)
{
	IDBGLOG0 ("start");

	if (initialized) {
		IDBGLOG0 ("already initialized");
		return (SL_SINIT_REINIT);
	}

	SL_STDBG_add (statereport, MODULE);

	initialized = SL_true;
	IDBGLOG0 ("done");
	return (SL_SINIT_OK);
}

void
SL_EXIT_shutdown (void)
{
	IDBGLOG0 ("start");

	if (!initialized) {
		IDBGLOG0 ("not yet initialized");
		return;
	}
	initialized = SL_false;

	SL_STDBG_delete (statereport);

	IDBGLOG0 ("done");
}

static void
say (char *fmt, ...)
{
	va_list	AP;
	SLMSG	msg;

	ASSERT (fmt);

	va_start (AP, fmt);

	msg.from		= SLMSG_FROM_EXIT;
	msg.type		= SLMSG_TYPE_EXIT;
	msg.prio		= SLMSG_PRIO_CRITICAL;
	msg.data.source		= MODULE;
	msg.data.type		= SLMSG_DATA_TYPE_PRINTF;
	msg.data.u.printf.fmt	= fmt;
	msg.data.u.printf.args	= AP;
	SLMSG_send (&msg);

	va_end (AP);
}

/*! Basic application termination reporting
 * \param ec	Exit code
 * \param msg	Termination reason
 * \param dump	Flag indicating a coredump was produced
 */
static void
basic_exit_report (unsigned int ec, char *msg, SL_BOOL dump)
{
	char	*fmt = "\n***** Instant death: %s, exit code %d%s *****\n";

	say (fmt, msg, ec, dump ? " [dumped core]" : "");
}

/*! Application termination reporting
 * \param ec	Exit code
 * \param msg	Termination reason
 * \param dump	Flag indicating a coredump was produced
 */
static void
exit_report (unsigned int ec, char *msg, SL_BOOL dump)
{
	char	*name = NULL;
	char	*fmt1 = "\n***** %s terminated: %s, exit code %d%s *****\n";
	char	*fmt2 = "\n***** Application terminated: %s, exit code %d%s *****\n";


	if (SL_OPTIONS.hook.exit) SL_OPTIONS.hook.exit (ec, msg, dump);

	name = SL_APP_exe_name ();
	if (name)
		say (fmt1, name, msg, ec, dump ? " [dumped core]" : "");
	else
		say (fmt2, msg, ec, dump ? " [dumped core]" : "");

}

/*! Application termination with reporting
 * \param ec	Exit code
 * \param msg	Termination reason
 */
void
SL_EXIT_exit (unsigned int ec, char *msg)
{
	/* Report death */
	exit_report (ec, msg, SL_false);

	/* Does not return */
	exit (ec);
}

/*! Application termination with reporting and coredump
 * \param ec	Exit code
 * \param msg	Termination reason
 * \param fix	Pointer to storage for return address fixup
 */
void
SL_EXIT_exit_and_dump (unsigned int ec, char *msg, SL_EXIT_FIXUP *fix)
{
	/* Report death */
	exit_report (ec, msg, SL_true);

	/* Does not return */
	if (fix == NULL) {
		SL_system_coredump ();
	} else {
		*fix = (SL_EXIT_FIXUP)SL_system_coredump;
		FlushInstructionCache (SL_system_process(), fix, sizeof (SL_EXIT_FIXUP));
	}
}

/*! Application termination without reporting
 * \param ec	Exit code
 * \param msg	Termination reason
 */
void
SL_EXIT_exit_NOW (unsigned int ec, char *msg)
{
	/* Basic report */
	basic_exit_report (ec, msg, SL_false);

	/* Does not return */
	exit (ec);
}

/*! Application termination with coredump but without reporting
 * \param ec	Exit code
 * \param msg	Termination reason
 * \param fix	Pointer to storage for return address fixup
 */
void
SL_EXIT_exit_and_dump_NOW (unsigned int ec, char *msg, SL_EXIT_FIXUP *fix)
{
	/* Basic report */
	basic_exit_report (ec, msg, SL_true);

	/* Does not return */
	if (fix == NULL) {
		SL_system_coredump ();
	} else {
		*fix = (SL_EXIT_FIXUP)SL_system_coredump;
		FlushInstructionCache (SL_system_process(), fix, sizeof (SL_EXIT_FIXUP));
	}
}

static void
statereport (SL_STDBG_INFO_LEVEL level)
{
	SAYSTATE0 ("### Application exit handler state:\n");

	/* basic information */

	/* extended information */
	if (level >= SL_STDBG_LEVEL_EXT) {
	}

	/* deep probe */
	if (level >= SL_STDBG_LEVEL_DEEP) {
	}

	SAYSTATE0 ("\n");
}
