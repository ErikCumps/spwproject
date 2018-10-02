/** \file
 * Slay's Library - error handling.
 *
 * Copyright (C) 2001-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "internal_debug.h"
#include <SL/system.h>
#include <SL/safe_mem.h>
#include <SL/error.h>
#include <SL/stdbg.h>
#include <SL/thread.h>
#include <SL/message.h>
#include <SL/app_exit.h>
#include "internal_opts.h"
#include "error_private.h"


// TODO: decide what to do with OOM error data? Use it? Drop it?


/* --- forward declarations --- */
static void	statereport	(SL_STDBG_INFO_LEVEL level);
static void	SL_ERROR_reset	(void);



/* --- private macros --- */



/* --- private data types --- */

/*! Error state TLS */
typedef struct s_SL_ERROR_TLS {
	char		dynerr[SL_ERROR_STRSIZE];	/*!< dynamic error message		*/
	SL_ERROR_DATA	*error;				/*!< pointer to current error stack	*/
	SL_ERROR_DATA	oom;				/*!< current OOM error information	*/
} SL_ERROR_TLS;

/*! Forward declaration of TLS access function */
static SL_ERROR_TLS	*TLS			(void);


/* --- private variables --- */

/*! Module name */
static const char *MODULE = "SL_ERROR";

/*! Initialization status */
static SL_BOOL		initialized = SL_false;

/*! TLS index */
static DWORD		tls = TLS_OUT_OF_INDEXES;

/*! System error messages */
static const char *SL_ERROR_STRINGS[SL_ERR_LAST_ERR_CODE+1] = {
	"no error\0",
	"you should NEVER see this error! Contact developer.\0",
	"system error codes go beyond start of user error codes\0",
	"illegal error code\0",
	"illegal error code and ran out of memory\0",
	"out of memory\0",
	"system panic\0",
	"function got unexpected 'NULL' argument\0",
	"function got invalid argument\0",
	"system call failed\0",
	"C library call failed\0",
	"rereg of user error code attempted\0",
	"invalid fatality type\0",
	"signal repository not yet initialized\0",
	"signal handler not found\0",
	"StringList index out of bounds\0",
	"regular expression not yet matched\0",
	"invalid submatch id for this regular expression\0",
	"truncated submatch\0",
	"regular expression error\0",
	"no match found for this regular expression\0",
	"file does not exist\0",
	"file exists\0",
	"configuration file not found\0",
	"failed to open configuration file\0",
	"failed to find configuration section\0",
	"failed to find configuration value name\0",
	"invalid syntax for section name\0",
	"invalid syntax for entry name\0",
	"entry without a value\0",
	"invalid syntax for entry value\0",
	"value is an invalid number\0",
	"invalid user interface type\0",
	"user interface type already registered\0",
	"user interface type not registered\0",
	"user interface field already registered\0",
	"logging not yet initialized\0",
	"logging already initialized\0",
	"no log file specified\0",
	"generic error condition\0",
	"generic action failure\0"
};

/*! Error fatality type strings, with uppercased initial letter */
static const char *SL_ERROR_FATAL_STRINGS[SL_ERR_FATAL_LAST_FATAL+1] = {
	"Error condition <type not set>",
	"Ignored error",
	"Warning",
	"Soft error",
	"Error",
	"Panic",
	"Usage error"
};

/*! Error fatality type strings, all lowercase */
static const char *SL_ERROR_FATAL_STRINGS_LC[SL_ERR_FATAL_LAST_FATAL+1] = {
	"error condition <type not set>",
	"ignored error",
	"warning",
	"soft error",
	"error",
	"panic",
	"usage error"
};

/*! Static storage for dynamic error message */
#define	SL_ERROR_DYNERRSTR	(TLS())->dynerr

/*! Current OOM error */
#define	SL_ERROR_OOM		(TLS())->oom

/*! Pointer to current error stack */
#define	SL_ERROR_ERROR		(TLS())->error

/*! Error code collection */
static SL_ERROR_SET	error_set;



/* --- code --- */

/*! Thread local storage allocation
 *
 * \param tls	TLS index
 */
static void
thread_tls_init (DWORD tls)
{
	SL_ERROR_TLS	*p;
	BOOL		b;

	SL_SAFE_CALLOC (p, 1, sizeof (SL_ERROR_TLS));
	b = TlsSetValue (tls, p); ASSERT (b == TRUE);
}

/*! Thread local storage cleanup
 *
 * \param tls	TLS index
 */
static void
thread_tls_exit (DWORD tls)
{
	SL_ERROR_TLS	*p;

	p = (SL_ERROR_TLS *)TlsGetValue (tls); ASSERT (p != NULL);
	TlsSetValue (tls, NULL);
	SL_SAFE_FREE (p);
}

static SL_ERROR_TLS *
TLS (void)
{
	SL_ERROR_TLS	*p;

	p = (SL_ERROR_TLS *)TlsGetValue (tls); ASSERT (p != NULL);
	return (p);
}


/*! Looks up error message for error code
 * \param ec   Error code
 * \return     Error message, NULL if not found
 */
const char *
SL_ERROR_code_lookup (int ec)
{
	SL_ERROR_CODE	*p;

	if (ec <= SL_ERR_LAST_ERR_CODE) {
		return SL_ERROR_STRINGS[ec];
	}

	if ((ec >= SL_ERR_USER_START) && (ec <= error_set.max)) {
		p = error_set.head;
		while (p != NULL) {
			if (p->ec == ec) return p->msg;
			p = p->next;
		}
	}

	return NULL;
}

/*! Looks up fatality type string
 * \param fatal   Fatality type
 * \param lc      Fetch all-lowercase string?
 * \return        Fatality type string
 */
const char *
SL_ERROR_fatality_lookup (SL_ERROR_FATAL fatal, unsigned int lc)
{
	if (lc)
		return SL_ERROR_FATAL_STRINGS_LC[fatal];
	else
		return SL_ERROR_FATAL_STRINGS[fatal];
}

/*! Registers user error code and message
 * \param ec    User error code
 * \param msg   User error message
 * \return      Error status
 */
SL_ERROR
SL_ERROR_code_register (int ec, const char *msg)
{
	const char	*e;
	SL_ERROR_CODE	*p;

	SL_CWENULLARG (msg);

	e = SL_ERROR_code_lookup (ec);
	if (e != NULL) { RETURN_ERR_FUNCTION (SL_ERR_EC_DBLREG, e); }

	SL_SAFE_CALLOC (p, 1, sizeof (SL_ERROR_CODE));

	p->ec = ec;
	strncpy (p->msg, msg, SL_ERROR_STRSIZE);

	if (error_set.head == NULL) {
		error_set.head = error_set.tail = p;
	} else {
		error_set.tail->next = p;
		error_set.tail = p;
	}

	if (ec > error_set.max) error_set.max = ec;

	RETURN_OK;
}

/*! Initializes error handling subsystem
 * \return            Error status
 */
SL_SINIT
SL_ERROR_init (void)
{
	SL_BOOL	b;

	IDBGLOG0 ("start");

	if (initialized) {
		IDBGLOG0 ("already initialized");
		return (SL_SINIT_REINIT);
	}

	ASSERT (SL_ERR_LAST_ERR_CODE < SL_ERR_USER_START);

	b = SL_THREAD_tlsalloc (&tls, thread_tls_init, thread_tls_exit);
	ASSERT (b != SL_false);
	thread_tls_init (tls);

	memset (&error_set, '\0', sizeof (error_set));
	memset (&SL_ERROR_OOM, 0, sizeof (SL_ERROR_OOM));
	SL_ERROR_ERROR = NULL;

	SL_STDBG_add (statereport, MODULE);

	initialized = SL_true;
	IDBGLOG0 ("done");
	return (SL_SINIT_OK);
}

/*! Shuts down error system
 *
 * \todo implement
 */
void
SL_ERROR_shutdown (void)
{
	SL_ERROR_CODE	*p;

	IDBGLOG0 ("start");

	if (!initialized) {
		IDBGLOG0 ("not yet initialized");
		return;
	}
	initialized = SL_false;

	SL_STDBG_delete (statereport);

	p = error_set.head;
	while (p) {
		SL_ERROR_CODE *q = p; p = p->next;
		SL_SAFE_FREE (q);
	}

	thread_tls_exit (tls);

	IDBGLOG0 ("done");
}

/*! Looks up error message for error code
 * \param ec   Error code
 * \return     Error message
 */
static char *
SL_ERROR_strerror (SL_ERROR ec)
{
	const char	*str;

	str = SL_ERROR_code_lookup (ec);
	if (str == NULL) {
		memset (SL_ERROR_DYNERRSTR, 0, sizeof(SL_ERROR_DYNERRSTR));
		snprintf (SL_ERROR_DYNERRSTR, SL_ERROR_STRSIZE, "illegal error code [%d]", ec);
		return SL_ERROR_DYNERRSTR;
	} else {
		return (char *)str;
	}
}

/*! Builds strings for error
 * \param err		Pointer to error info structure to fill
 * \param file		Optional file string
 * \param line		Optional line number
 * \param func		Optional function string
 * \param extra		Optional extra error information
 */
static void
SL_ERROR_BuildStrings (SL_ERROR_DATA *err, const char *file, unsigned long line, const char *func, char *extra)
{
	snprintf (err->func_err_msg , SL_ERROR_STRSIZE, "%s", SL_ERROR_strerror(err->func_err));
	snprintf (err->sys_err_msg, SL_ERROR_STRSIZE, "%s", strerror(err->sys_err));

	SL_CODELOC_set (&(err->location), file, line, func);

	memset (err->extra, 0, sizeof (err->extra));
	if (extra != NULL) snprintf (err->extra, SL_ERROR_STRSIZE, "%s", extra);
}

/*! Builds strings for error
 * \param err     Pointer to error info structure to fill
 * \param where   Optional code location
 * \param fmt     Optional format string
 * \param AP      Optional extra error information arguments
 */
static void
SL_ERROR_BuildStrings_alt (SL_ERROR_DATA *err, SL_CODELOC *where, char *fmt, va_list AP)
{
	snprintf (err->func_err_msg , SL_ERROR_STRSIZE, "%s", SL_ERROR_strerror(err->func_err));
	snprintf (err->sys_err_msg, SL_ERROR_STRSIZE, "%s", strerror(err->sys_err));

	SL_CODELOC_copy (&(err->location), where);

	memset (err->extra, 0, sizeof (err->extra));
	if (fmt != NULL) vsnprintf (err->extra, SL_ERROR_STRSIZE, fmt, AP);
}

/*! Sets error to no error
 */
void
SL_ERROR_none (void)
{
	SL_ERROR_reset ();
}

/*! Stores current error state
 * \param handle	Pointer to storage for pointer to dynamically allocated error state copy
 *
 * \note This routine will overwrite whatever is present in *handle!
 */
void
SL_ERROR_store (SL_ERROR_DATA **handle)
{
	ASSERT (handle != NULL);

	*handle = SL_ERROR_ERROR;
	SL_ERROR_ERROR = NULL;
}

/*! Restores current error state
 * \param handle	Pointer to storage for pointer to dynamically allocated error state copy
 *
 * \note A NULL pointer for the stored error state is not an error.
 *
 * \note This routine will clean up the current error stack. Even when there is no stored error state!
 */
void
SL_ERROR_restore (SL_ERROR_DATA **handle)
{
	ASSERT (handle != NULL);

	SL_ERROR_reset ();
	SL_ERROR_ERROR = *handle;
	*handle = NULL;
}

/*! Creates new error info structure
 */
static SL_ERROR_DATA *
SL_new_error (void)
{
	SL_ERROR_DATA	*e;

	SL_SAFE_CALLOC (e, 1, sizeof (SL_ERROR_DATA));
	e->fatal    = SL_ERR_FATAL_UNSET;
	e->stackloc = SL_ERR_STACK_UNSET;
	return (e);
}

/*! Push error on error stack
 * \param e   Pointer to new error
 *
 * \todo add error tracing when TRACE is defined
 */
static void
SL_push_error (SL_ERROR_DATA *e)
{
	e->prev = SL_ERROR_ERROR;
	SL_ERROR_ERROR = e;
	if (e->prev) e->idx  = e->prev->idx+1;

	if (SL_OPTIONS.hook.error != NULL) SL_OPTIONS.hook.error ();
}

/*! Sets function error
 * \param rc		Error code
 * \param file		File name
 * \param line		File line number
 * \param func		Function name
 * \param extra		Optional extra error info
 */
void
SL_ERROR_function (SL_ERROR rc, const char *file, unsigned long line, const char *func, char *extra)
{
	SL_ERROR_DATA	*e;

	e = SL_new_error ();

	e->func_err = rc;
	e->sys_err  = 0;
	SL_ERROR_BuildStrings (e, file, line, func, extra);

	SL_push_error (e);
}

/*! Sets function error
 * \param rc      Error code
 * \param where   Code location
 * \param fmt     Optional extra error info format string
 * \param ...     Optional extra error info arguments
 *
 * \note Consumes code location
 */
void
SL_ERROR_function_alt (SL_ERROR rc, SL_CODELOC *where, char *fmt, ...)
{
	SL_ERROR_DATA	*e;
        va_list		AP;

	e = SL_new_error ();

	e->func_err = rc;
	e->sys_err  = 0;

	va_start (AP, fmt);
	SL_ERROR_BuildStrings_alt (e, where, fmt, AP);
	va_end (AP);
	SL_FREE_CODELOC (where);

	SL_push_error (e);
}

/*! Sets function error
 * \param rc      Error code
 * \param where   Code location
 * \param fmt     Optional extra error info format string
 * \param AP      Optional extra error info arguments
 *
 * \note Consumes code location
 */
void
SL_ERROR_function_alt2 (SL_ERROR rc, SL_CODELOC *where, char *fmt, va_list AP)
{
	SL_ERROR_DATA	*e;

	e = SL_new_error ();

	e->func_err = rc;
	e->sys_err  = 0;
	SL_ERROR_BuildStrings_alt (e, where, fmt, AP);
	SL_FREE_CODELOC (where);

	SL_push_error (e);
}

/*! Sets system error
 * \param rc		Error code
 * \param file		File name
 * \param line		File line number
 * \param func		Function name
 * \param extra		Optional extra error info
 */
void
SL_ERROR_system (SL_ERROR rc, const char *file, unsigned long line, const char *func, char *extra)
{
	DWORD		gle = GetLastError();
	SL_ERROR_DATA	*e;

	e = SL_new_error ();

	e->func_err = rc;
	e->sys_err  = gle;
	SL_ERROR_BuildStrings (e, file, line, func, extra);

	SL_push_error (e);
}

/*! Sets system error
 * \param rc      Error code
 * \param where   Code location
 * \param fmt     Optional extra error info format string
 * \param ...     Optional extra error info arguments
 *
 * \note Consumes code location
 */
void
SL_ERROR_system_alt (SL_ERROR rc, SL_CODELOC *where, char *fmt, ...)
{
	DWORD		gle = GetLastError();
	SL_ERROR_DATA	*e;
        va_list 	AP;

	e = SL_new_error ();

	e->func_err = rc;
	e->sys_err  = gle;

	va_start (AP, fmt);
	SL_ERROR_BuildStrings_alt (e, where, fmt, AP);
	va_end (AP);
	SL_FREE_CODELOC (where);

	SL_push_error (e);
}

/*! Resets current error */
static void
SL_ERROR_reset (void)
{
	SL_ERROR_DATA	*p;

	if (SL_ERROR_OOM.func_err != 0) {
		memset (&SL_ERROR_OOM, 0, sizeof (SL_ERROR_OOM));
	}

	if (SL_ERROR_ERROR == NULL) return;
	p = SL_ERROR_ERROR;
	while (p != NULL) {
		SL_ERROR_DATA *e = p; p = p->prev;
		SL_SAFE_FREE (e);
	}
	SL_ERROR_ERROR = NULL;
}

static void
SL_send_stream (char *fmt, ...)
{
	va_list	AP;
	SLMSG	msg;

	ASSERT (fmt);

	va_start (AP, fmt);

	msg.from		= SLMSG_FROM_ERROR;
	msg.type		= SLMSG_TYPE_ERRSTR;
	msg.prio		= SLMSG_PRIO_DEFAULT;
	msg.data.source		= NULL;
	msg.data.type		= SLMSG_DATA_TYPE_PRINTF;
	msg.data.u.printf.fmt	= fmt;
	msg.data.u.printf.args	= AP;
	SLMSG_send (&msg);

	va_end (AP);
}

/*! Provides basic error reporting on stderr
 * \param stack   Pointer to error report information stack
 */
static void
SL_ERROR_report_stream (SL_ERROR_DATA *stack)
{
	SL_ERROR_DATA *info = stack;

	ASSERT (stack != NULL);

	while (info) {
		switch (info->stackloc) {
			case SL_ERR_STACK_BOTTOM:
				SL_send_stream ("> *** [%u] Initial error:\n", info->idx);
				break;
			case SL_ERR_STACK_MIDDLE:
				SL_send_stream ("> *** [%u] Previous error:\n", info->idx);
				break;
			case SL_ERR_STACK_TOP:
			case SL_ERR_STACK_SINGLE:
				if (info->msg[0] != '\0') {
					if (info->fatal != SL_ERR_FATAL_UNSET) {
						SL_send_stream ("> *** [%u] %s: %s\n", info->idx, info->fatality, info->msg);
					} else {
						SL_send_stream ("> *** [%u] Error: %s\n", info->idx, info->msg);
					}
				} else {
					if (info->fatal != SL_ERR_FATAL_UNSET) {
						SL_send_stream ("> *** [%u] %s:\n", info->idx, info->fatality);
					} else {
						SL_send_stream ("> *** [%u] Error:\n", info->idx);
					}
				}
				SL_send_stream ("> ***     Reported from %s:%lu %s()\n", info->reporter.file, info->reporter.line, info->reporter.func);
				SL_send_stream ("> ***\n");
				break;
		}
		SL_send_stream ("> ***    Function return code [%u] \"%s\"\n", info->func_err, info->func_err_msg);
		if (info->sys_err) {
			SL_send_stream ("> ***    System error code [%d], \"%s\"\n", info->sys_err, info->sys_err_msg);
		}
		if (info->extra[0] != '\0') {
			SL_send_stream ("> ***    Extra information: %s\n", info->extra);
		}
		SL_send_stream ("> ***    Detected at %s:%lu %s()\n", info->location.file, info->location.line, info->location.func);
		SL_send_stream ("> ***\n");

		info = info->prev;
	}
}

/*! Handles error
 * \param fatal		Error fatality type
 * \param msg		Optional extra error info string
 * \param file		Name of file of reporting function
 * \param line		Line in reporting function
 * \param func		Name of function that reports this error
 *
 * \return		Error resolution user request code
 */
SL_ERROR_REQUEST
SL_ERROR_handle (SL_ERROR_FATAL fatal, char *msg, const char *file, unsigned long line, const char *func)
{
	SL_ERROR_DATA		*stack = NULL;
	SL_ERROR_DATA		*e;
	SL_ERROR_REQUEST	req = SL_ERR_REQUEST_ACCEPT;

	/* Get error stack. Must be present! */
	stack = SL_ERROR_ERROR;
	if (stack == NULL) ABORT ("Wanted to report error but lost error stack");

	/* Update the error information structure stack. Stack modifications are
	 * limited to pushes, there is no need to build a private copy of the strings */
	e = stack;
	while (e != NULL) {
		e->stackloc = SL_ERR_STACK_MIDDLE;

		if (e == stack) {
			if (msg) snprintf (e->msg, SL_ERROR_STRSIZE, "%s", msg);
			e->fatal = fatal;
			snprintf (e->fatality, SL_ERROR_STRSIZE, "%s", SL_ERROR_fatality_lookup (fatal, 0));
			SL_CODELOC_set (&(e->reporter), file, line, func);

			if (!e->prev) {
				e->stackloc = SL_ERR_STACK_SINGLE;
			} else {
				e->stackloc = SL_ERR_STACK_TOP;
			}
		} else {
			if (!e->prev) e->stackloc = SL_ERR_STACK_BOTTOM;
		}

		e = e->prev;
	}

	/* Call panic hook if needed */
	if ((fatal == SL_ERR_FATAL_PANIC) && (SL_OPTIONS.hook.panic != NULL)) SL_OPTIONS.hook.panic ();

	/* Report the error */
	if (SL_OPTIONS.ecb.error != NULL)
		req = SL_OPTIONS.ecb.error (stack);
	else
		SL_ERROR_report_stream (stack);

	/* Handle fatality type */
	switch (fatal) {
		case SL_ERR_FATAL_IGNORE:
			break;
		case SL_ERR_FATAL_WARN:
			break;
		case SL_ERR_FATAL_SOFTERR:
			if (req == SL_ERR_REQUEST_ACCEPT) SL_DIE (SL_EXIT_FATAL_ERROR, "Terminated because of soft error");
			break;
		case SL_ERR_FATAL_ERR:
			SL_DIE (SL_EXIT_FATAL_ERROR, "Terminated because of fatal error");
			break;
		case SL_ERR_FATAL_PANIC:
			ABORT ("SL_ERR_FATAL_PANIC error");
			break;
		case SL_ERR_FATAL_USAGE:
			SL_DIE (SL_EXIT_USAGE_ERROR, "Terminated because of usage error");
			break;
		default:
			DEV_PANIC1 ("unknown SL_ERROR fatality type (%u)", fatal);
			break;
	}

	/* Clean up */
	SL_ERROR_reset ();

	return (req);
}

/*! Handles error (alternate version using code location)
 * \param fatal   Error fatality type
 * \param msg     Optional extra error info string
 * \param where   Code location
 *
 * \return		Error resolution user request code
 *
 * \note Consumes code location
 */
SL_ERROR_REQUEST
SL_ERROR_handle_alt (SL_ERROR_FATAL fatal, char *msg, SL_CODELOC *where)
{
	SL_ERROR_REQUEST	req;

	if (where == NULL) {
		req = SL_ERROR_handle (fatal, msg, NULL, 0, NULL);
	} else {
		req = SL_ERROR_handle (fatal, msg, where->file, where->line, where->func);
	}
	SL_FREE_CODELOC (where);

	return (req);
}

/*! Developer panic
 * \param file	source file
 * \param line	source line
 * \param func	source fuction
 * \param fmt	panic msg format
 * \param ...	additional arguments
 */
void
SL_ERROR_devpanic (const char *file, unsigned long line, const char *func, char *fmt, ...)
{
	va_list AP;

	char		*format;
	char		newfmt[1024];
	char		buffer[2048];
	SL_CODELOC	*here;

	if (fmt == NULL) {
		format = (char *)"<no info>";
	} else {
		format = fmt;
	}

	memset (newfmt, 0, sizeof (newfmt));
	snprintf (newfmt, sizeof (newfmt) - 1, "%s", format);

	memset (buffer, 0, sizeof (buffer));
	va_start (AP, fmt);
	vsnprintf (buffer, sizeof (buffer) - 1, newfmt, AP);
	va_end (AP);
	here = SL_HERE_I_AM_core (file, line, func);

	SetLastError(0);
	SET_ERR_SYSTEM_ALT0 (SL_ERR_PANIC, here, buffer);

	/* does not return */
	SL_ERROR_HANDLE (SL_ERR_FATAL_PANIC, "Developer panic!");
}


/* --- debugging API --- */

/*! Shows particular error info
 * \param err   Pointer to particular error info struct
 */
static void
SL_showcurrent (SL_ERROR_DATA *err)
{
	SAYSTATE1 ("\t\tmessage                 = %s\n", err->msg);
	SAYSTATE1 ("\t\tfatality type           = %d\n", err->fatal);
	SAYSTATE1 ("\t\tfatality description    = %s\n", err->fatality);
	SAYSTATE1 ("\t\treporter: file          = %s\n", err->reporter.file);
	SAYSTATE1 ("\t\treporter: line          = %lu\n", err->reporter.line);
	SAYSTATE1 ("\t\treporter: func          = %s\n", err->reporter.func);
	SAYSTATE1 ("\t\terror location: file    = %s\n", err->location.file);
	SAYSTATE1 ("\t\terror location: line    = %lu\n", err->location.line);
	SAYSTATE1 ("\t\terror location: func    = %s\n", err->location.func);
	SAYSTATE1 ("\t\tfunction error          = %d\n", err->func_err);
	SAYSTATE1 ("\t\tfunction error message  = %s\n", err->func_err_msg);
	SAYSTATE1 ("\t\tsystem error            = %d\n", err->sys_err);
	SAYSTATE1 ("\t\tsystem error message    = %s\n", err->sys_err_msg);
	SAYSTATE1 ("\t\toptional extra info     = %s\n", err->extra);
	SAYSTATE1 ("\t\tstack location code     = %d\n", err->stackloc);
}

static void
statereport_errorlist (SL_ERROR_DATA *e, char *name)
{
	SL_ERROR_DATA	*p;

	p = e;
	while (p) {
		if (!p->prev) {
			if (p == e) {
				SAYSTATE1 ("\tCurrent %s:\n", name);
			} else {
				SAYSTATE1 ("\tInitial %s:\n", name);
			}
		} else {
			if (p == e) {
				SAYSTATE1 ("\tLast %s:", name);
			} else {
				SAYSTATE1 ("\tPrevious %s:", name);
			}
		}
		SL_showcurrent (p);
		SAYSTATE0 ("\n");
		p = p->prev;
	}
}

/*! Shows error handler state
 * \param level	Level of detail to show
 */
static void
statereport (SL_STDBG_INFO_LEVEL level)
{
	unsigned int	i;
	SL_ERROR_CODE	*p;

	SAYSTATE0 ("### Error handler state:\n");

	/* basic information */
	SAYSTATE1 ("\tinitialized = %s\n", SL_BOOL_tostr (initialized));

	if (SL_ERROR_ERROR == NULL) {
		SAYSTATE0 ("\tno current error\n");
	} else {
		statereport_errorlist (SL_ERROR_ERROR, "error");
	}
	if (SL_ERROR_OOM.func_err == 0) {
		SAYSTATE0 ("\tno current OOM error\n");
	} else {
		statereport_errorlist (&(SL_ERROR_OOM), "OOM error");
	}

	/* extended information */
	if (level >= SL_STDBG_LEVEL_EXT) {
		SAYSTATE1 ("\tcurrent dynamic error string = <%s>\n", SL_ERROR_DYNERRSTR);
	}

	/* deep probe */
	if (level >= SL_STDBG_LEVEL_DEEP) {
		SAYSTATE0 ("\tregistered error codes:\n");
		for (i = 0; i <= SL_ERR_LAST_ERR_CODE; i++) {
			SAYSTATE2 ("\t\t(%4d) %s\n", i, SL_ERROR_STRINGS[i]);
		}
		SAYSTATE0 ("\tregistered user error codes:\n");
		p = error_set.head;
		if (error_set.max < SL_ERR_USER_START) {
			SAYSTATE0 ("\t\tnone\n");
		} else {
			for (i = 0; i <= (unsigned int)(error_set.max - SL_ERR_USER_START); i++) {
				if (p == NULL) {
					SAYSTATE1 ("\t\t[%4u] (?) ?\n", SL_ERR_USER_START + i);
				} else {
					SAYSTATE2 ("\t\t(%4d) %s\n", p->ec, p->msg);
				}
				if (p != NULL) p = p->next;
			}
		}
		SAYSTATE0 ("\tsupported fatality types:\n");
		for (i = 0; i <= SL_ERR_FATAL_LAST_FATAL; i++) {
			SAYSTATE2 ("\t\t(%4d) %s\n", i, SL_ERROR_FATAL_STRINGS_LC[i]);
		}
	}

	SAYSTATE0 ("\n");
}
