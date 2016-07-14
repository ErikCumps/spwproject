/** \file
 * Slay's Library - internal basic output handling.
 *
 * Copyright (C) 2006-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "internal_debug.h"
#include <SL/system.h>
#include <SL/bool.h>
#include <SL/lock.h>
#include <SL/util.h>
#include <SL/message.h>
#include <SL/stdbg.h>
#include "basic_output.h"



/* --- forward declarations --- */

static SLMSG_RC	out_stdout	(SLMSG *msg);
static SLMSG_RC	out_stderr	(SLMSG *msg);
static void	statereport	(SL_STDBG_INFO_LEVEL level);


/* --- private macros  --- */

/*! Maximum buffer size for SLMSG_DATA_TYPE_PRINTF data */
#define	MAXOUTLEN	512



/* --- private variables --- */

/*! Module name */
static const char	*MODULE = "SLBO";

/*! Initialization status flag */
static SL_BOOL		initialized = SL_false;

/*! SLMSG handler sending data to stdout */
static const SLMSG_HDLR	hdlr_stdout = { "SLBO: stdout", out_stdout, SLMSG_MODE_EXCLUSIVE, SLMSG_ATTR_NONE };

/*! SLMSG handler sending data to stderr */
static const SLMSG_HDLR	hdlr_stderr = { "SLBO: stderr", out_stderr, SLMSG_MODE_EXCLUSIVE, SLMSG_ATTR_NONE|SLMSG_ATTR_ERR };

/*! Access lock */
static SL_LOCK		*lock = NULL;


/* --- code --- */

/*! Initializes basic output subsystem
 *
 * \return	initialization status
 */
SL_SINIT
SLBO_init (void)
{
	IDBGLOG0 ("start");

	if (initialized) {
		IDBGLOG0 ("already initialized");
		return (SL_SINIT_REINIT);
	}

	lock = SL_LOCK_new ();
	if (!lock) {
		IDBGLOG0 ("failed to create lock");
		return (SL_SINIT_FAIL);
	}

	SLMSG_hdlr_add (SLMSG_TYPE_ANY, SLMSG_FROM_ANY, &hdlr_stdout);
	SLMSG_hdlr_add (SLMSG_TYPE_STATE, SLMSG_FROM_ANY, &hdlr_stderr);
	SLMSG_hdlr_add (SLMSG_TYPE_EXIT, SLMSG_FROM_ANY, &hdlr_stderr);
	SLMSG_hdlr_add (SLMSG_TYPE_ERRSTR, SLMSG_FROM_ANY, &hdlr_stderr);
	SLMSG_hdlr_add (SLMSG_TYPE_UI_INFO, SLMSG_FROM_ANY, &hdlr_stdout);

	SL_STDBG_add (statereport, MODULE);

	initialized = SL_true;
	IDBGLOG0 ("done");
	return (SL_SINIT_OK);
}

/*! Shuts down basic output subsystem
 */
void
SLBO_shutdown (void)
{
	IDBGLOG0 ("start");

	if (!initialized) {
		IDBGLOG0 ("not yet initialized");
		return;
	}
	initialized = SL_false;

	SL_STDBG_delete (statereport);

	SLMSG_hdlr_delete (SLMSG_TYPE_ANY, SLMSG_FROM_ANY, &hdlr_stdout);
	SLMSG_hdlr_delete (SLMSG_TYPE_STATE, SLMSG_FROM_ANY, &hdlr_stderr);
	SLMSG_hdlr_delete (SLMSG_TYPE_EXIT, SLMSG_FROM_ANY, &hdlr_stderr);
	SLMSG_hdlr_delete (SLMSG_TYPE_ERRSTR, SLMSG_FROM_ANY, &hdlr_stderr);
	SLMSG_hdlr_delete (SLMSG_TYPE_UI_INFO, SLMSG_FROM_ANY, &hdlr_stdout);

	SL_LOCK_free (&lock);

	IDBGLOG0 ("done");
}

/*! Core output message handler
 *
 * \param stream	output stream
 * \param msg		pointer to message
 * \return		message handling return code
 *
 * \note No argument checking needed (static function only invoked by callers in this file)
 */
static SLMSG_RC
out_core (FILE *stream, SLMSG *msg)
{
	char		stamp[64];
	const char	*from;
	const char	*prio;
	char		buffer[MAXOUTLEN+1];

	SL_UTIL_timestamp (&(msg->time), stamp, sizeof (stamp));

	from = SLMSG_FROM_lookup_code (msg->from);
	prio = SLMSG_PRIO_lookup_code (msg->prio);

	switch (msg->data.type) {
		case SLMSG_DATA_TYPE_STRING:
			fprintf (stream, "%s %s [0x%8.8x] %s: %s", stamp, prio, msg->thid, from, msg->data.u.string.str);
			break;
		case SLMSG_DATA_TYPE_PRINTF:
			memset (buffer, 0, sizeof (buffer));
			vsnprintf (buffer, sizeof (buffer) - 1, msg->data.u.printf.fmt, msg->data.u.printf.args);
			fprintf (stream, "%s %s [0x%8.8x] %s: %s", stamp, prio, msg->thid, from, buffer);
			break;
		case SLMSG_DATA_TYPE_CMD:
			switch (msg->data.u.cmd) {
				case SLMSG_CMD_FLUSH:
					fflush (stream);
					break;
				default: break;
			}
			break;
		default:
			ABORT ("unknown message data type");
			break;
	}
	return (SLMSG_RC_OK);
}

/*! Stdout output message handler
 *
 * \param msg	pointer to message
 * \return	message handling return code
 */
static SLMSG_RC
out_stdout (SLMSG *msg)
{
	SLMSG_RC	rc;

	if (!initialized) return (SLMSG_RC_FAIL);

	ASSERT (lock != NULL);
	ASSERTRV (msg != NULL, SLMSG_RC_FAIL);

	SL_LOCK_take (lock);
	rc = out_core (stdout, msg);
	SL_LOCK_drop (lock);
	return (rc);
}

/*! Stderr output message handler
 *
 * \param msg	pointer to message
 * \return	message handling return code
 */
static SLMSG_RC
out_stderr (SLMSG *msg)
{
	SLMSG_RC	rc;

	if (!initialized) return (SLMSG_RC_FAIL);

	ASSERT (lock != NULL);
	ASSERTRV (msg != NULL, SLMSG_RC_FAIL);

	SL_LOCK_take (lock);
	rc = out_core (stderr, msg);
	SL_LOCK_drop (lock);
	return (rc);
}

/*! Reports state debug reporting state
 *
 * \param level	state report level
 */
static void
statereport (SL_STDBG_INFO_LEVEL level)
{
	SAYSTATE0 ("### Basic output subsystem state:\n");

	/* basic information */
	if (level >= SL_STDBG_LEVEL_BAS) {
		SAYSTATE1 ("\tinitialized = %s\n", SL_BOOL_tostr (initialized));
	}

	/* extended information */
	if (level >= SL_STDBG_LEVEL_EXT) {
		SAYSTATE1 ("\tlock        = 0x%8.8x\n", lock);
	}

	/* deep probe */
	if (level >= SL_STDBG_LEVEL_DEEP) {
	}

	SAYSTATE0 ("\n");
}
