/** \file
 * Slay's Library - state debug reporting.
 *
 * Copyright (C) 2003-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "internal_debug.h"
#include <SL/system.h>
#include <SL/safe_mem.h>
#include <SL/stdbg.h>
#include <SL/bool.h>
#include <SL/lock.h>
#include <SL/message.h>
#include "stdbg_private.h"



/* --- forward declarations --- */

static void statereport (SL_STDBG_INFO_LEVEL level);



/* --- private macros --- */

/*! Maximum number of report functions to report in showstate() */
#define	MAX_REPORT	99



/* --- private data types and structures --- */

/*! Linked list with registered report functions */
struct s_report_func_list {
	SL_STDBG_REPORT			func;		/*!< report function */
	const char			*name;		/*!< report function name */
	struct s_report_func_list	*next;		/*!< pointer to next report function */
};
typedef struct s_report_func_list ReportersList;



/* --- private variables --- */

/*! Module name */
static const char	*MODULE = "SL_STDBG";

/*! Initialization status flag */
static SL_BOOL		initialized = SL_false;

/*! Access lock */
static SL_LOCK		*lock = NULL;

/*! Registered report functions */
static ReportersList	*reporters = NULL;



/* --- code --- */

/*! Initializes state debug subsystem
 *
 * \return	initialization status
 */
SL_SINIT
SL_STDBG_init (void)
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

	initialized = SL_true;
	IDBGLOG0 ("done");
	return (SL_SINIT_OK);
}

/*! Shuts down state debug subsystem
 *
 * \return	initialization status
 */
void
SL_STDBG_shutdown (void)
{
	IDBGLOG0 ("start");

	if (!initialized) {
		IDBGLOG0 ("not yet initialized");
		return;
	}
	initialized = SL_false;

	SL_LOCK_free (&lock);

	if (reporters != NULL) {
		unsigned int i = 1;
		ReportersList *q = reporters;
		/* Limited to MAX_REPORT handlers to prevent runaway loop in case of memory corruption */
		while ((q != NULL) && (i <= MAX_REPORT)) {
			IDBGLOG3 ("[%02u] p=0x%8.8x, func=0x%8.8x\n", i, q, q->func);
			i++; q = q->next;
		}
	}

	IDBGLOG0 ("done");
}

/*! Registers state report function
 *
 * \param func	state report function to register
 * \param name	identification of the owner of the state data
 */
void
SL_STDBG_add (SL_STDBG_REPORT func, const char *name)
{
	ReportersList	*p = NULL;

	if (!initialized || !func) return;
	ASSERT (lock != NULL);

	SL_SAFE_CALLOC (p, 1, sizeof (ReportersList));

	p->func = func;
	p->name = name;

	IDBGLOG2 ("p=0x%8.8x, func=0x%8.8x\n", p, p->func);

	SL_LOCK_take (lock);
	p->next = reporters;
	reporters = p;
	SL_LOCK_drop (lock);
}

/*! Unregisters state report function
 *
 * \param func	state report function to remove
 */
void
SL_STDBG_delete (SL_STDBG_REPORT func)
{
	ReportersList	*p, *q;

	if (!initialized || !func) return;
	ASSERT (lock != NULL);

	SL_LOCK_take (lock);
	p = reporters; q = NULL;
	while (p) {
		if (p->func == func) break;
		q = p; p = p->next;
	}
	if (p) {
		if (q) {
			q->next = p->next;
		} else {
			reporters = p->next;
		}
	}
	SL_LOCK_drop (lock);

	if (p) {
		IDBGLOG2 ("p=0x%8.8x, func=0x%8.8x\n", p, p->func);
		SL_SAFE_FREE (p);
	}
}

/*! Invokes all registered state report functions
 *
 * \param level	state report level
 */
void
SL_STDBG_report (SL_STDBG_INFO_LEVEL level)
{
	ReportersList	*p = reporters;
	SLMSG_HQL	*list;
	SLMSG		msg;

	/* Dump header line */
	SL_STDBG_say (NULL, "----------------------------------------------------------------------------\n");

	/* First call all registered reporting functions */
	SL_LOCK_take (lock);
	while (p) {
		p->func (level);
		p = p->next;
	}
	SL_LOCK_drop (lock);

	/* Dump message handling state */
	SLMSG_statereport (level != SL_STDBG_LEVEL_BAS);

	/* Finally dump our own state */
	statereport (level);

	SLMSG_hdlr_query (SLMSG_TYPE_STATE, SLMSG_FROM_ANY, &list);
	if (list && (list->hdlr.attr & SLMSG_ATTR_FILE)) {
		msg.from		= SLMSG_FROM_STDBG;
		msg.type		= SLMSG_TYPE_UI_INFO;
		msg.prio		= SLMSG_PRIO_DEFAULT;
		msg.data.source		= NULL;
		msg.data.type		= SLMSG_DATA_TYPE_STRING;
		msg.data.u.string.str	= "state information stored in log file";
		SLMSG_send (&msg);
	}
	SLMSG_hdlr_query_free (&list);
}

/*! Transmits state info message
 *
 * \param src   pointer to optional extra message source name
 * \param fmt   pointer to format string
 * \param ...   format string arguments
 */
void
SL_STDBG_say (const char *src, char *fmt, ...)
{
	va_list	AP;
	SLMSG	msg;

	ASSERTR (fmt);

	va_start (AP, fmt);

	msg.from = SLMSG_FROM_STDBG;
	msg.type = SLMSG_TYPE_STATE;
	msg.prio = SLMSG_PRIO_DEFAULT;
	msg.data.source      = src;
	msg.data.type        = SLMSG_DATA_TYPE_PRINTF;
	msg.data.u.printf.fmt  = fmt;
	msg.data.u.printf.args = AP;
	SLMSG_send (&msg);

	va_end (AP);
}

/*! Reports state debug reporting state
 *
 * \param level	state report level
 */
static void
statereport (SL_STDBG_INFO_LEVEL level)
{
	unsigned int	i, cnt;
	ReportersList	*q;

	/* Limited to MAX_REPORT handlers to prevent runaway loop in case of memory corruption */
	q = reporters; cnt = 0;
	while ((q != NULL) && (cnt <= MAX_REPORT)) { cnt++; q = q->next; }

	SAYSTATE0 ("### State debug reporting state:\n");

	/* basic information */
	if (level >= SL_STDBG_LEVEL_BAS) {
		SAYSTATE1 ("\tinitialized      = %s\n", SL_BOOL_tostr (initialized));
		SAYSTATE1 ("\treport functions = %u\n", cnt);
	}

	/* extended information */
	if (level >= SL_STDBG_LEVEL_EXT) {
		SAYSTATE1 ("\tlock             = 0x%8.8x\n", lock);
	}

	/* deep probe */
	if (level >= SL_STDBG_LEVEL_DEEP) {
		SAYSTATE0 ("\treport function list:\n");
		if (reporters != NULL) {
			i = 1; q = reporters;
			/* Limited to MAX_REPORT handlers to prevent runaway loop in case of memory corruption */
			while ((q != NULL) && (i <= MAX_REPORT)) {
				SAYSTATE4 ("\t\t[%02u 0x%8.8x] 0x%8.8x: %s\n", i, q, q->func, q->name);
				i++; q = q->next;
			}
		} else {
			SAYSTATE0 ("\t\tnone\n");
		}
	}

	SAYSTATE0 ("\n");
}

