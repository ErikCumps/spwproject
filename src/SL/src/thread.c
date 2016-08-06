/** \file
 * Slay's Library - thread handling.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "internal_debug.h"
#include <SL/system.h>
#include <SL/safe_mem.h>
#include <SL/thread.h>
#include <SL/bool.h>
#include <SL/lock.h>
#include <SL/stdbg.h>

/* --- forward declarations --- */

static void statereport (SL_STDBG_INFO_LEVEL level);



/* --- private data types and structures --- */

/*! TLS management structure */
typedef struct s_THREADTLS{
	DWORD			tls;	/*!< TLS allocation index		*/
	SL_THREAD_TLS_INIT	init;	/*!< TLS init function			*/
	SL_THREAD_TLS_EXIT	exit;	/*!< TLS exit function			*/
	struct s_THREADTLS	*prev;	/*!< pointer to previous TLS structure	*/
	struct s_THREADTLS	*next;	/*!< pointer to next TLS structure	*/
} THREADTLS;

typedef struct s_THREADINFO {
	LPTHREAD_START_ROUTINE	func;
	void			*arg;
} THREADINFO;



/* --- private variables --- */

/*! Module name */
static const char	*MODULE = "SL_THREAD";

/*! Initialization status */
static SL_BOOL		initialized = SL_false;

/*! TLS registration */
static THREADTLS	*tls_head = NULL;
static THREADTLS	*tls_tail = NULL;

/*! TLS registration access lock */
static SL_LOCK		*tls_lock = NULL;

/* --- code --- */

/* \note No locking here, this is done by the caller! */
#define	LISTDUMP	\
do {															\
	THREADTLS *p = tls_head;											\
	IDBGLOG2 ("LISTDUMP (tls_head=0x%8.8x, tls_tail=0x%8.8x)", tls_head, tls_tail);					\
	while (p != NULL) {												\
		IDBGLOG6 ("[0x%8.8x] tls=0x%8.8x, init=0x%8.8x, exit=0x%8.8x, prev=0x%8.8x, next=0x%8.8x",		\
				(unsigned int)p, p->tls, (unsigned int)p->init, (unsigned int)p->exit,			\
				(unsigned int)p->prev, (unsigned int)p->next);						\
		p = p->next;												\
	}														\
} while (0)

SL_SINIT
SL_THREAD_init (void)
{
	IDBGLOG0 ("start");

	if (initialized) {
		IDBGLOG0 ("already initialized");
		return (SL_SINIT_REINIT);
	}

	tls_head = tls_tail = NULL;
	ASSERT ((tls_lock = SL_LOCK_new ()) != NULL);

	SL_STDBG_add (statereport, MODULE);

	initialized = SL_true;
	IDBGLOG0 ("done");
	return (SL_SINIT_OK);
}

/*! Shuts down thread system
 *
 * \todo implement
 */
void
SL_THREAD_shutdown (void)
{
	THREADTLS	*p, *q;

	IDBGLOG0 ("start");

	if (!initialized) {
		IDBGLOG0 ("not yet initialized");
		return;
	}
	initialized = SL_false;

	SL_STDBG_delete (statereport);

	p = tls_head;
	while (p) {
		q = p; p = p->next;
		TlsFree (q->tls);
		SL_SAFE_FREE (q);
	}

	SL_LOCK_free (&tls_lock);

	IDBGLOG0 ("done");
}

SL_BOOL
SL_THREAD_tlsalloc (DWORD *tls, SL_THREAD_TLS_INIT init, SL_THREAD_TLS_EXIT exit)
{
	THREADTLS	*p, *q;

	ASSERT (tls != NULL);
	ASSERT (initialized == SL_true);

	IDBGLOG3 ("(tls=0x%8.8x, init=0x%8.8x, exit=0x%8.8x)", tls, init, exit);

	if (!init || !exit) return (SL_false);

	*tls = TlsAlloc ();
	if (*tls == TLS_OUT_OF_INDEXES) return (SL_false);

	IDBGLOG1 ("tls index=0x%8.8x", *tls);

	SL_SAFE_CALLOC (p, 1, sizeof (THREADTLS));

	p->tls  = *tls;
	p->init = init;
	p->exit = exit;

	SL_LOCK_take (tls_lock);

	if (!tls_head) {
		tls_head = tls_tail = p;
	} else {
		q = tls_head;
		while (q->next) q = q->next;
		q->next = p;
		p->prev = q;
		tls_tail = p;
	}
	LISTDUMP;

	SL_LOCK_drop (tls_lock);

	return (SL_true);
}

void
SL_THREAD_tlsfree (DWORD tls)
{
	THREADTLS	*p;

	ASSERT (initialized == SL_true);

	IDBGLOG1 ("(tls=0x%8.8x)", tls);

	SL_LOCK_take (tls_lock);

	if (!tls_head) return;

	p = tls_head;
	if (tls_head->tls == tls) {
		tls_head = tls_head->next;
		if (tls_head) {
			tls_head->prev = NULL;
		} else {
			tls_tail = NULL;
		}
	} else {
		while (p && (p->tls != tls)) { p = p->next; }
		if (p) {
			if (!p->next) tls_tail = p->prev;
			if (p->prev) p->prev->next = p->next;
			if (p->next) p->next->prev = p->prev;
		}
	}

	LISTDUMP;
	SL_LOCK_drop (tls_lock);

	if (p) {
		IDBGLOG4 ("free 0x%8.8x (tls=0x%8.8x, init=0x%8.8x, exit=0x%8.8x)", p, p->tls, p->init, p->exit);
		TlsFree (p->tls);
		SL_SAFE_FREE (p);
	}
}

/*! thread wrapper */
static DWORD WINAPI
thread_wrapper (void *data)
{
	THREADINFO	*td = (THREADINFO *)data;
	THREADTLS	*p;
	DWORD		rc;

	ASSERT (td != NULL);

	SL_LOCK_take (tls_lock);

	p = tls_head;
	while (p) {
		if (p->init) p->init (p->tls);
		p = p->next;
	}

	SL_LOCK_drop (tls_lock);

	rc = td->func (td->arg);

	SL_LOCK_take (tls_lock);

	p = tls_tail;
	while (p) {
		if (p->exit) p->exit (p->tls);
		p = p->prev;
	}

	SL_LOCK_drop (tls_lock);

	SL_SAFE_FREE (td);
	return (rc);
}


/*! Creates new thread
 *
 * \param func	thread function
 * \param arg	thread argument
 * \param stack	stack size (auto if 0)
 * \param flags	creation flags
 * \param tid	optional pointer to storage for thread id
 *
 * \return thread handle or NULL if failed
 */
HANDLE
SL_THREAD_create (LPTHREAD_START_ROUTINE func, void *arg, DWORD stack, DWORD flags, DWORD *tid)
{
	THREADINFO	*ti;

	ASSERT (initialized == SL_true);

	SL_SAFE_CALLOC (ti, 1, sizeof (THREADINFO));
	ti->func = func;
	ti->arg  = arg;
	return (CreateThread (NULL, stack, thread_wrapper, ti, flags, tid));
}

/*! Shows error handler state
 * \param level	Level of detail to show
 */
static void
statereport (SL_STDBG_INFO_LEVEL level)
{
	THREADTLS	*p;

	SAYSTATE0 ("### Thread handler state:\n");

	/* basic information */
	if (level >= SL_STDBG_LEVEL_BAS) {
		SAYSTATE1 ("\tinitialized = %s\n", SL_BOOL_tostr (initialized));
	}

	/* extended information */
	if (level >= SL_STDBG_LEVEL_EXT) {
		SAYSTATE1 ("\ttls_lock    = 0x%8.8x\n", tls_lock);
		SAYSTATE1 ("\ttls_head    = 0x%8.8x\n", tls_head);
		SAYSTATE1 ("\ttls_tail    = 0x%8.8x\n", tls_tail);
	}

	/* deep probe */
	if (level >= SL_STDBG_LEVEL_DEEP) {
		SAYSTATE0 ("\ttls registration list:\n");
		if (tls_head != NULL) {
			p = tls_head;
			while (p) {
				SAYSTATE1 ("\t\taddr = 0x%8.8x\n", p);
				SAYSTATE1 ("\t\t\ttls  = 0x%8.8x\n", p->tls);
				SAYSTATE1 ("\t\t\tinit = 0x%8.8x\n", p->init);
				SAYSTATE1 ("\t\t\texit = 0x%8.8x\n", p->exit);
				SAYSTATE1 ("\t\t\tprev = 0x%8.8x\n", p->prev);
				SAYSTATE1 ("\t\t\tnext = 0x%8.8x\n", p->next);
				p = p->next;
			}
		} else {
			SAYSTATE0 ("\t\tnone\n");
		}
	}

	SAYSTATE0 ("\n");
}
