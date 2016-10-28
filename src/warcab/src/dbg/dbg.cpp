/** \file
 * The SPWaW war cabinet - debugging support.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "../common.h"
#include "dbg_private.h"



/* --- forward declarations --- */

static SLMSG_RC	out_console	(SLMSG *msg);
static SLMSG_RC	out_file	(SLMSG *msg);
static SLMSG_RC	out_both	(SLMSG *msg);
static void	DBG_qtlogger	(QtMsgType type, const char *msg);



/* --- private macros  --- */

/*! Maximum buffer size for SLMSG_DATA_TYPE_PRINTF data */
#define	MAXOUTLEN	512



/* --- private variables --- */

/*! Initialization status flag */
static SL_BOOL	initialized = SL_false;

/*! Activation status flag */
static SL_BOOL	activated = SL_false;

/*! SLMSG handler sending data to stdout */
static const SLMSG_HDLR	hdlr_console = { "DBG: console", out_console, SLMSG_MODE_EXCLUSIVE, SLMSG_ATTR_NONE };
static const SLMSG_HDLR	hdlr_file = { "DBG: file", out_file, SLMSG_MODE_EXCLUSIVE, SLMSG_ATTR_NONE|SLMSG_ATTR_FILE };
static const SLMSG_HDLR	hdlr_both = { "DBG: both", out_both, SLMSG_MODE_EXCLUSIVE, SLMSG_ATTR_NONE|SLMSG_ATTR_FILE };



/* --- code --- */

void
DBG_init (void)
{
	if (initialized) return;

#ifdef	_DEBUG

	DBG_CONS_init ();

	SLMSG_hdlr_add (SLMSG_TYPE_ANY, SLMSG_FROM_ANY, &hdlr_console);
	SLMSG_hdlr_add (SLMSG_TYPE_STATE, SLMSG_FROM_ANY, &hdlr_file);
	SLMSG_hdlr_add (SLMSG_TYPE_EXIT, SLMSG_FROM_ANY, &hdlr_both);
	SLMSG_hdlr_add (SLMSG_TYPE_ERRSTR, SLMSG_FROM_ANY, &hdlr_file);
	SLMSG_hdlr_add (SLMSG_TYPE_UI_INFO, SLMSG_FROM_ANY, &hdlr_console);

	activated = SL_true;

#endif	/* _DEBUG */

	/* Intercept Qt internal logging */
	qInstallMsgHandler (DBG_qtlogger);

	atexit (DBG_shutdown);

	initialized = SL_true;

	DBG_log ("DBG subsystem initialized\n");
}

void
DBG_shutdown (void)
{
	if (!initialized) return;
	initialized = activated = SL_false;

	qInstallMsgHandler (0);

#ifdef	_DEBUG

	SLMSG_hdlr_delete (SLMSG_TYPE_ANY, SLMSG_FROM_ANY, &hdlr_console);
	SLMSG_hdlr_delete (SLMSG_TYPE_STATE, SLMSG_FROM_ANY, &hdlr_file);
	SLMSG_hdlr_delete (SLMSG_TYPE_EXIT, SLMSG_FROM_ANY, &hdlr_both);
	SLMSG_hdlr_delete (SLMSG_TYPE_ERRSTR, SLMSG_FROM_ANY, &hdlr_file);
	SLMSG_hdlr_delete (SLMSG_TYPE_UI_INFO, SLMSG_FROM_ANY, &hdlr_console);

	DBG_CONS_shutdown ();

#endif	/* _DEBUG */
}

void
DBG_log (char *fmt, ...)
{
	va_list	AP;

	if (!initialized || !activated) return;

	va_start (AP, fmt);
	if (AP != NULL)
		DBG_CONS_vlog (SL_true, SL_true, fmt, AP);
	else
		DBG_CONS_log (SL_true, SL_true, fmt);
	va_end (AP);
}

void
DBG_logbuf (char *buf, int len)
{
	if (!initialized || !activated) return;

	DBG_CONS_logbuf (SL_true, SL_true, buf, len);
}

static void
log_core (SL_BOOL cons, SL_BOOL file, char *stamp, char *head, char *fmt, va_list AP)
{
	char		*p = fmt;
	va_list		AP_copy;
	char		buffer[MAXOUTLEN+1];
	unsigned int	size;
	char		*q;

	/* Do not use DEVASSERT here, we will end up in a recursive loop! */
	ASSERT (stamp != NULL); ASSERT (head != NULL); ASSERT (fmt != NULL);

	while (*p == '\n') {
		DBG_CONS_log (cons, file, "%s\n", stamp);
		p++;
	}

	/* early bailout if empty string */
	if (*p == '\0') return;

	/* Create formatted string in buffer */
	memset (buffer, 0, sizeof (buffer));
	if (AP != NULL) {
		SL_UTIL_va_copy (&AP_copy, &AP);
		size = vsnprintf (buffer, sizeof (buffer) - 1, p, AP_copy); size++;
		va_end (AP_copy);
	} else {
		size = snprintf (buffer, sizeof (buffer) - 1, "%s", p); size++;
	}
	va_end (AP);

	/* Log NL-separated strings from formatted string */
	while ((q = strchr (p, '\n')) != NULL) {
		*q = 0; q++;
		DBG_CONS_log (cons, file, "%s %s%s\n", stamp, head, p);
		p = q;
	}
	/* And log final NL-less part if any */
	if (*p != '\0') {
		DBG_CONS_log (cons, file, "%s %s%s\n", stamp, head, p);
	}
}

static SLMSG_RC
out_core (SLMSG *msg, SL_BOOL cons, SL_BOOL file)
{
	char		stamp[64];
	char		head[64];
	const char	*from;
	const char	*prio;
	char		buffer[MAXOUTLEN+1];

	/* Handle commands first */
	if (msg->data.type == SLMSG_DATA_TYPE_CMD) {
		switch (msg->data.u.cmd) {
			case SLMSG_CMD_FLUSH:
				DBG_CONS_flush ();
				break;
			default:
				break;
		}
		return (SLMSG_RC_OK);
	}

	/* Format timestamp */
	memset (stamp, 0, sizeof (stamp));
	if (msg->prio == SLMSG_PRIO_DEEPTRACE) stamp[0] = '@'; else stamp[0] = ' ';
	if (msg->type == SLMSG_TYPE_STATE) {
		snprintf (stamp+1, sizeof (stamp) - 2, "[0x%8.8x:STATE]", msg->thid);
	} else {
		SL_UTIL_timestamp (&(msg->time), stamp + 1, sizeof (stamp) - 1);
	}

	/* Format header */
	memset (head, 0, sizeof (head));
	if (msg->type != SLMSG_TYPE_STATE) {
		from = msg->data.source;
		if (!from) from = SLMSG_FROM_lookup_code (msg->from);

		if (msg->prio == SLMSG_PRIO_DEEPTRACE) {
			prio = SLMSG_DATA_CODE_DT_lookup_code (msg->data.code.dt);
		} else {
			prio = SLMSG_PRIO_lookup_code (msg->prio);
		}

		snprintf (head, sizeof (head) - 1, "%s%s[0x%8.8x] %s: ", prio, (prio[0] != '\0') ? " " : "", msg->thid, from);
	}

	/* Log data */
	switch (msg->data.type) {
		case SLMSG_DATA_TYPE_STRING:
			log_core (cons, file, stamp, head, msg->data.u.string.str, NULL);
			break;
		case SLMSG_DATA_TYPE_PRINTF:
			memset (buffer, 0, sizeof (buffer));
			vsnprintf (buffer, sizeof (buffer) - 1, msg->data.u.printf.fmt, msg->data.u.printf.args);
			log_core (cons, file, stamp, head, buffer, NULL);
			break;
		case SLMSG_DATA_TYPE_CMD:
			/* already handled */
			break;
		default:
			DEV_PANIC1 ("unknown SLMSG data type (0x%08.8x)", msg->data.type);
			break;
	}
	return (SLMSG_RC_OK);
}

static SLMSG_RC
out_console (SLMSG *msg)
{
	if (!initialized) return (SLMSG_RC_FAIL);
	ASSERTRV (msg != NULL, SLMSG_RC_FAIL);

	return (out_core (msg, SL_true, SL_false));
}

static SLMSG_RC
out_file (SLMSG *msg)
{
	if (!initialized) return (SLMSG_RC_FAIL);
	ASSERTRV (msg != NULL, SLMSG_RC_FAIL);

	return (out_core (msg, SL_false, SL_true));
}

static SLMSG_RC
out_both (SLMSG *msg)
{
	if (!initialized) return (SLMSG_RC_FAIL);
	ASSERTRV (msg != NULL, SLMSG_RC_FAIL);

	return (out_core (msg, SL_true, SL_true));
}

void
DBG_oom_hook (const char *file, unsigned long line, const char *func, unsigned long size)
{
	DBG_log ("*** OOM condition: %s:%lu %s: %lu bytes\n", file, line, func, size);
}

void
DBG_panic_hook (void)
{
	SL_STDBG_report (SL_STDBG_LEVEL_DEEP);
}

void
DBG_qtlogger (QtMsgType type, const char *msg)
{
	switch (type) {
		case QtDebugMsg:
			DBG_log ("<QT-DEBUG> %s\n", msg);
			break;
		case QtWarningMsg:
			DBG_log ("<QT-WARNING> %s\n", msg);
			break;
		case QtCriticalMsg:
			DBG_log ("<QT-CRITICAL> %s\n", msg);
			break;
		case QtFatalMsg:
			DBG_log ("<QT-FATAL> %s\n", msg);
			break;
		default:
			DBG_log ("<QT-%d> %s\n", type, msg);
			break;
	}
}

