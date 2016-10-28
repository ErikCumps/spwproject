/** \file
 * The SPWaW war cabinet - GUI.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "../common.h"
#include "gui_private.h"



/* --- forward declarations --- */

static void	statereport	(SL_STDBG_INFO_LEVEL level);



/* --- private types  --- */

typedef struct s_ERRINF {
	const char	*file;
	unsigned long	line;
	const char	*func;
	const char	*msg;
} ERRINF;



/* --- private macros  --- */



/* --- private variables --- */

/*! Module name */
static const char	*MODULE = "GUI";

/*! Initialization status flag */
static SL_BOOL		initialized = SL_false;

/*! QT application object */
QApplication		*GUI_APP = NULL;

/*! QT main window object */
GuiMainWindow		*GUI_WIN = NULL;



/* --- code --- */

void
GUI_init (int *argc, char **argv)
{
	/* Do not use DEVASSERT here, SL not yet initialized! */

	if (initialized) return;

	/* Initialize QT application */
	GUI_APP = new QApplication (*argc, argv); ASSERT (GUI_APP != NULL);
	GUI_APP->setQuitOnLastWindowClosed (false);

	initialized = SL_true;
}

void
GUI_exit (void)
{
	QApplication	*p;

	if (!initialized) return;

	p = GUI_APP; GUI_APP = NULL;
	delete p;

	initialized = SL_false;
}

int
GUI_exec (void)
{
	HANDLE		timer = NULL;
	QSplashScreen	splash;
	LARGE_INTEGER	splashtime;
	GuiMainWindow	*p;
	int		rc;

	DBG_log ("GUI: initializing\n");

	/* Create and show splash screen */
	DBG_log ("GUI: creating a waitable timer for splash screen display\n");
	/* Failure to create the timer is no error - we just ignore this and do not use the timer */
	timer = CreateWaitableTimer (NULL, TRUE, NULL);

	DBG_log ("GUI: creating and showing splash screen\n");
	splash.setPixmap (*RES_pixmap(RID_SPLASH));
	splash.show();
	GUI_FIXME;

	if (timer) {
		DBG_log ("GUI: starting timer for splash screen\n");
		splashtime.QuadPart = -GUI_SPLASH_TIME * 1000 * 10;
		if (!SetWaitableTimer (timer, &splashtime, 0, NULL, NULL, 0)) {
			DBG_log ("GUI: failed to start splash screen timer, timer will be ignored\n");
			CloseHandle (timer); timer = NULL;
		}
	}

	/* Add SL state report callback now that SL is initialized */
	SL_STDBG_add (statereport, MODULE);

	/* Create main window */
	DBG_log ("GUI: creating main window\n");
	GUI_WIN = new GuiMainWindow (); DEVASSERT (GUI_WIN != NULL);
	if (SL_HAS_ERROR (GUI_WIN->error_code)) {
		SL_ERROR_HANDLE (SL_ERR_FATAL_ERR, "Failed to create application window!");
	}

	/* Set memtrace marker to indicate end of application startup */
	MEMTRACE_MARK ("APPLICATION RUNTIME");

	/* Make sure splash screen is shown long enough */
	DBG_log ("GUI: waiting for splash screen display\n");
	if (timer) {
		WaitForSingleObject (timer, GUI_SPLASH_TIME);
		CloseHandle (timer); timer = NULL;
	}

	/* Show window */
	DBG_log ("GUI: showing main window\n");
	GUI_WIN->show ();
	GUI_FIXME;

	/* Finish showing splash screen */
	DBG_log ("GUI: finishing splash screen\n");
	splash.finish (GUI_WIN);

	/* Post initialization event if required */
	if (WARCAB->needs_postedinit()) {
		DBG_log ("GUI: posting initialization event\n");
		//TODO: create custom event?
		QEvent *event = new QEvent (GUI_EVENT_INIT);
		GUI_APP->postEvent (GUI_WIN, event);
	}

	/* Execute application event loop */
	DBG_log ("GUI: starting event loop\n");
	rc = GUI_APP->exec ();

	/* Destroy main window */
	p = GUI_WIN; GUI_WIN = NULL; p->setVisible (false);
	delete p;

	/* Remove SL state report callback */
	SL_STDBG_delete (statereport);

	/* Return with result code */
	return (rc);
}

static void
statereport (SL_STDBG_INFO_LEVEL level)
{
	SAYSTATE0 ("### GUI state:\n");

	/* basic information */
	if (level >= SL_STDBG_LEVEL_BAS) {
		SAYSTATE1 ("\tinitialized = %s\n", SL_BOOL_tostr (initialized));
	}

	/* extended information */
	if (level >= SL_STDBG_LEVEL_EXT) {
		SAYSTATE1 ("\tapplication = 0x%8.8x\n", GUI_APP);
		SAYSTATE1 ("\tmain window = 0x%8.8x\n", GUI_WIN);
	}

	/* deep probe */
	if (level >= SL_STDBG_LEVEL_DEEP) {
	}

	SAYSTATE0 ("\n");
}

/* Qt GUI objects are only supported in main thread! */
static inline bool
GUI_isGUIthread (void)
{
	QObject	q;

	if (!GUI_APP || (q.thread() != GUI_APP->thread())) return (false);
	return (true);
}

static bool
GUI_abort (ERRINF *inf)
{
	const char	*fmt = "ABORT: %s!\n\nFile\t: %s\nLine\t: %lu\nFunction\t: %s";
	char		buffer[1024];
	int		sb;

	/* Do not use DEVASSERT here, we will end up in a recursive loop! */
	ASSERT (inf != NULL);

	memset (buffer, 0, sizeof (buffer));
	if (inf->msg) {
		snprintf (buffer, sizeof (buffer) - 1, fmt, inf->msg, inf->file, inf->line, inf->func);
	} else {
		snprintf (buffer, sizeof (buffer) - 1, fmt, "ABORT DETECTED", inf->file, inf->line, inf->func);
	}

#if	ALLOW_ABORT_IGNORE

	sb = QMessageBox::critical (0, "Application abort", buffer, "Abort", "Debug", "Ignore", 0, 0);
	ASSERT ((sb >= 0) && (sb <= 2));

#else	/* !ALLOW_ABORT_IGNORE */

	sb = QMessageBox::critical (0, "Application abort", buffer, "Abort", "Debug", NULL, 0, 0);
	ASSERT ((sb >= 0) && (sb <= 2));

#endif	/* !ALLOW_ABORT_IGNORE */

	switch (sb) {
		case 0:
			GUI_APP->exit (42);
			break;
		case 1:
			DebugBreak ();
			break;
		case 2:
			return (false);
			break;
	}
	return (true);
}

SL_EVT_RC
GUI_EVT_abort (const char *file, unsigned long line, const char *func, const char *msg)
{
	SL_EVT_RC	rc = SL_EVT_HANDLE;
	QObject		q;
	ERRINF		inf;

	if (!GUI_isGUIthread()) return (rc);

	inf.file = file;
	inf.line = line;
	inf.func = func;
	inf.msg  = msg;
	if (!GUI_abort (&inf)) rc = SL_EVT_CONTINUE;

	return (rc);
}

void
GUI_HOOK_exit (unsigned int ec, char *msg, SL_BOOL dump)
{
	const char	*rfmt = "%s\n\nExit code: %d";
	const char	*efmt = "%s\n\nExit code: %d\n\n[application dumped core]";
	char		buffer[1024];

	if (!GUI_isGUIthread()) return;

	memset (buffer, 0, sizeof (buffer));
	if (!dump) {
		snprintf (buffer, sizeof (buffer) - 1, rfmt, msg ? msg : "none", ec);
		QMessageBox::information (0, "Application termination", buffer, QMessageBox::Ok, 0, 0);
	} else {
		snprintf (buffer, sizeof (buffer) - 1, efmt, msg ? msg : "none", ec);
		QMessageBox::critical (0, "Application termination", buffer, QMessageBox::Ok, 0, 0);
	}
}

static void
gui_error_handle_stack_entry_basic (SL_ERROR_DATA *e, UtilStrbuf &sb)
{
	if (e->func_err) {
		sb.printf ("[%u] Error (%u) %s!\n", e->idx, e->func_err, e->func_err_msg);
		if (e->sys_err) {
			sb.printf ("    system error code (%d), %s\n", e->sys_err, e->sys_err_msg);
		}
	} else if (e->sys_err) {
		sb.printf ("[%u] System error (%d) %s!\n", e->idx, e->sys_err, e->sys_err_msg);
	} else {
		if (e->fatality[0] != '\0') {
			sb.printf ("[%u] %s!\n", e->idx, e->fatality);
		} else {
			sb.printf ("[%u] Error!\n", e->idx);
		}
	}
	if (e->extra[0] != '\0') sb.printf ("    info msg: %s\n", e->extra);
}

static void
gui_error_handle_stack_entry_full (SL_ERROR_DATA *s, UtilStrbuf &sb)
{
	gui_error_handle_stack_entry_basic (s, sb);

	if (s->reporter.func[0] != '\0') {
		sb.printf ("    reported: %s() at %s:%lu\n", s->reporter.func, s->reporter.file, s->reporter.line);
	}
	if (s->location.func[0] != '\0') {
		sb.printf ("    detected: %s() at %s:%lu\n", s->location.func, s->location.file, s->location.line);
	}
}

static void
GUI_error_handle_stack_basic (SL_ERROR_DATA *stack, UtilStrbuf &sb)
{
	SL_ERROR_DATA	*e = stack;

	while ((e != NULL) && !sb.full()) {
		gui_error_handle_stack_entry_basic (e, sb);
		sb.printf ("\n");
		e = e->prev;
	}
}

static void
GUI_error_handle_stack_full (SL_ERROR_DATA *stack, UtilStrbuf &sb)
{
	SL_ERROR_DATA	*e = stack;

	while ((e != NULL) && !sb.full()) {
		gui_error_handle_stack_entry_full (e, sb);
		sb.printf ("\n");
		e = e->prev;
	}
}

static SL_ERROR_REQUEST
GUI_error (SL_ERROR_DATA *stack)
{
	char			*fmt1 = "%s";
	char			buf1[SL_ERROR_STRSIZE+1];
	UtilStrbuf		sb1 (buf1, sizeof (buf1), true, true);
	char			*fmt2 = "*** Error: %s\n*** Error stack:\n";
	char			buf2[SL_ERROR_STRSIZE+1];
	UtilStrbuf		sb2 (buf2, sizeof (buf2), true, true);
	char			buf3[4096];
	UtilStrbuf		sb3 (buf3, sizeof (buf3), true, true);
	SL_ERROR_REQUEST	req = SL_ERR_REQUEST_ACCEPT;

	/* Do not use DEVASSERT here, we will end up in a recursive loop! */
	ASSERT (stack != NULL);

	if (!GUI_isGUIthread()) return (req);

	if (stack->msg[0] == '\0') {
		sb1.printf (fmt1, stack->extra);
	} else {
		sb1.printf (fmt1, stack->msg);
	}
	sb2.printf (fmt2, buf1);

#ifdef	_DEBUG
	GUI_error_handle_stack_full (stack, sb3);
#else	/* !_DEBUG */
	GUI_error_handle_stack_basic (stack, sb3);
#endif	/* !_DEBUG */

	DBG_logbuf (buf2, strlen (buf2));
	DBG_logbuf (buf3, strlen (buf3));

	req = GUI_errorbox (stack->fatal, buf1, "Error stack", buf3);

	return (req);
}

SL_ERROR_REQUEST
GUI_EVT_error (SL_ERROR_DATA *stack)
{
	return (GUI_error (stack));
}
