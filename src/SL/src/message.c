/** \file
 * Slay's Library - internal message handling.
 *
 * Copyright (C) 2006-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "internal_debug.h"
#include <SL/system.h>
#include <SL/safe_mem.h>
#include <SL/message.h>
#include <SL/bool.h>
#include <SL/lock.h>



/* --- private data types and structures --- */

/*! Linked list with registered handlers */
typedef struct s_list {
	const char	*name;	/*!< handlername				*/
	SLMSG_HDLR_FUNC	func;	/*!< handler function				*/
	SLMSG_MODE	mode;	/*!< handler mode				*/
	SLMSG_ATTR	attr;	/*!< handler attributes				*/
	struct s_list	*prev;	/*!< pointer to previous registered handler	*/
	struct s_list	*next;	/*!< pointer to next registered handler		*/
} LIST;

/*! Linked list anchor */
typedef struct s_anchor {
	SL_LOCK		*lock;		/*!< List access lock		*/
	SLMSG_TYPE	type;		/*!< List message type id	*/
	SLMSG_FROM	from;		/*!< List message source id	*/
	LIST		*list;		/*!< Pointer to linked list	*/
} ANCHOR;

/*! array containing handler lists per SLMSG_FROM */
typedef ANCHOR	DSTLST[SLMSG_FROM_LIMIT+1];

/*! array containing DSTLST arrays per SLMSG_TYPE */
typedef DSTLST	DISPATCH[SLMSG_TYPE_LIMIT+1];



/* --- private variables --- */

/*! Initialization status flag */
static SL_BOOL	initialized = SL_false;

/*! Message dispatch data */
static DISPATCH	dispatch;



/* --- code --- */

/* \note No locking here, this is done by the caller! */
#define	LISTDUMP(_head)	\
do {															\
	LIST *p = _head->list;												\
	IDBGLOG3 ("LISTDUMP (%s from %s) 0x%8.8x",									\
		SLMSG_TYPE_lookup_code (_head->type), SLMSG_FROM_lookup_code (_head->from), _head->list);		\
	while (p != NULL) {												\
		IDBGLOG7 ("[0x%8.8x] name=\"%s\", func=0x%8.8x, mode=%u, attr=0x%8.8lx, prev=0x%8.8x, next=0x%8.8x\n",	\
				(unsigned int)p, p->name, (unsigned int)p->func, p->mode, p->attr,			\
				(unsigned int)p->prev, (unsigned int)p->next);						\
		p = p->next;												\
	}														\
} while (0)

/*! Initializes internal message subsystem
 *
 * \return	initialization status
 */
SL_SINIT
SLMSG_init (void)
{
	SLMSG_TYPE	type;
	SLMSG_FROM	from;
	ANCHOR		*head;

	IDBGLOG0 ("start");

	if (initialized) {
		IDBGLOG0 ("already initialized");
		return (SL_SINIT_REINIT);
	}

	memset (dispatch, 0, sizeof (dispatch));
	for (type = SLMSG_TYPE_START; type <= SLMSG_TYPE_LIMIT; type++) {
		for (from = SLMSG_FROM_START; from <= SLMSG_FROM_LIMIT; from++) {
			head = &(dispatch[type][from]);
			ASSERT ((head->lock = SL_LOCK_new ()) != NULL);
			head->type = type;
			head->from = from;
		}
	}

	initialized = SL_true;
	IDBGLOG0 ("done");
	return (SL_SINIT_OK);
}

/*! Shuts down internal message subsystem
 *
 * \todo implement
 */
void
SLMSG_shutdown (void)
{
	SLMSG_TYPE	type;
	SLMSG_FROM	from;
	ANCHOR		*head;

	IDBGLOG0 ("start");

	if (!initialized) {
		IDBGLOG0 ("not yet initialized");
		return;
	}
	initialized = SL_false;

	for (type = SLMSG_TYPE_START; type <= SLMSG_TYPE_LIMIT; type++) {
		for (from = SLMSG_FROM_START; from <= SLMSG_FROM_LIMIT; from++) {
			head = &(dispatch[type][from]);
			SL_LOCK_free (&(head->lock));
		}
	}

	IDBGLOG0 ("done");
}

/*! Message dispatcher core
 *
 * \param head	pointer to anchor of linked list with registered handlers
 * \param msg	pointer to message
 *
 * \return	message handling return code
 *
 * \note No argument checking needed (static function only invoked by callers in this file)
 */
static int
slmsg_send_core (ANCHOR *head, SLMSG *msg)
{
	int	rc = SLMSG_RC_FAIL;
	LIST	*p, *q;

	SL_LOCK_take (head->lock);

	p = head->list;
	while (p != NULL) {
		q = p; p = p->next;
		if (q->func == NULL) continue;

		rc = q->func (msg);
		//IDBGLOG5 ("msg (%s, %s) sent via (%s, %s): rc=%s",
		//	SLMSG_TYPE_lookup_code (msg->type), SLMSG_FROM_lookup_code (msg->from),
		//	SLMSG_TYPE_lookup_code (head->type), SLMSG_FROM_lookup_code (head->from),
		//	SLMSG_RC_lookup_code (rc));

		if ((q->mode == SLMSG_MODE_EXCLUSIVE) && (rc != SLMSG_RC_PASS))	break;
		if (rc == SLMSG_RC_OK) break;
	}

	SL_LOCK_drop (head->lock);
	return (rc);
}

/*! Message dispatcher
 *
 * \param msg	pointer to message
 */
void
SLMSG_send (SLMSG *msg)
{
	int		rc;

	if (!initialized) return;
	if (!msg) return;
	ASSERT (msg->from != SLMSG_FROM_ANY);

	time (&(msg->time));
	msg->thid = SL_system_threadid();

	/* Try type -> from */
	rc = slmsg_send_core (&(dispatch[msg->type][msg->from]), msg);
	if (rc != SLMSG_RC_OK) {
		/* Else try type -> ANY */
		rc = slmsg_send_core (&(dispatch[msg->type][SLMSG_FROM_ANY]), msg);
	}
	if (rc != SLMSG_RC_OK) {
		/* Else try from -> ANY */
		rc = slmsg_send_core (&(dispatch[SLMSG_TYPE_ANY][msg->from]), msg);
	}
	if (rc != SLMSG_RC_OK) {
		/* Else try ANY -> ANY */
		slmsg_send_core (&(dispatch[SLMSG_TYPE_ANY][SLMSG_FROM_ANY]), msg);
	}
}

/*! Returns handler registered for handler func
 *
 * \param head  pointer to anchor of linked list with registered handlers
 * \param func	pointer to handler function
 * \return	pointer to registered handler
 * \retval NULL	if registered handler not found
 *
 * \note No argument checking needed (static function only invoked by callers in this file)
 */
static LIST *
list_lookup (ANCHOR *head, SLMSG_HDLR_FUNC func)
{
	LIST	*p = head->list;

	if (func == NULL) return (NULL);

	SL_LOCK_take (head->lock);
	while (p != NULL) {
		if (p->func == func) break;
		p = p->next;
	}
	SL_LOCK_drop (head->lock);

	return (p);
}

/*! Register handler
 *
 * \param from	message source for which to register handler hook
 * \param type	message type for which to register handler hook
 * \param hdlr  pointer to handler
 */
void
SLMSG_hdlr_add (SLMSG_TYPE type, SLMSG_FROM from, const SLMSG_HDLR *hdlr)
{
	ANCHOR	*head;
	LIST	*p;

	if (!hdlr || !initialized) return;
	head = &(dispatch[type][from]);

	if (list_lookup (head, hdlr->func) != NULL) return;

	IDBGLOG6 ("(type=%s, from=%s, func=0x%8.8x, mode=0x%8.8x, attr=0x%8.8lx, name=\"%s\")\n",
		SLMSG_TYPE_lookup_code (type), SLMSG_FROM_lookup_code (from),
		(unsigned int)hdlr->func, hdlr->mode, hdlr->attr, hdlr->name);

	SL_SAFE_CALLOC (p, 1, sizeof (LIST));

	SL_SAFE_STRDUP (p->name, hdlr->name);
	p->func = hdlr->func;
	p->mode = hdlr->mode;
	p->attr = hdlr->attr;

	SL_LOCK_take (head->lock);
	p->next = head->list;
	if (p->next != NULL) p->next->prev = p;
	head->list = p;

	//LISTDUMP (head);

	SL_LOCK_drop (head->lock);
}

/*! Replaces registered handler
 *
 * \param from	message source for which to register handler hook
 * \param type	message type for which to register handler hook
 * \param hdlr  pointer to handler
 */
void
SLMSG_hdlr_replace (SLMSG_TYPE type, SLMSG_FROM from, const SLMSG_HDLR *hdlr)
{
	if (!hdlr || !initialized) return;

	SLMSG_hdlr_delete (type, from, hdlr);
	SLMSG_hdlr_add    (type, from, hdlr);
}

/*! Replaces registered handler list with single handler
 *
 * \param from	message source for which to register handler hook
 * \param type	message type for which to register handler hook
 * \param hdlr  pointer to handler
 */
void
SLMSG_hdlr_set (SLMSG_TYPE type, SLMSG_FROM from, const SLMSG_HDLR *hdlr)
{
	ANCHOR	*head;
	LIST	*p, *q;

	if (!hdlr || !initialized) return;
	head = &(dispatch[type][from]);

	IDBGLOG6 ("(type=%s, from=%s, func=0x%8.8x, mode=0x%8.8x, attr=0x%8.8lx, name=\"%s\")\n",
		SLMSG_TYPE_lookup_code (type), SLMSG_FROM_lookup_code (from),
		(unsigned int)hdlr->func, hdlr->mode, hdlr->attr, hdlr->name);

	SL_SAFE_CALLOC (p, 1, sizeof (LIST));

	SL_SAFE_STRDUP (p->name, hdlr->name);
	p->func = hdlr->func;
	p->mode = hdlr->mode;
	p->attr = hdlr->attr;

	SL_LOCK_take (head->lock);
	q = head->list;
	head->list = p;

	//LISTDUMP (head);

	SL_LOCK_drop (head->lock);

	while (q != NULL) {
		p = q; q = q->next;
		SL_SAFE_FREE (p->name);
		SL_SAFE_FREE (p);
	}
}

/*! Unregisters handler
 *
 * \param from	message source for which to register handler hook
 * \param type	message type for which to register handler hook
 * \param hdlr  pointer to handler
 */
void
SLMSG_hdlr_delete (SLMSG_TYPE type, SLMSG_FROM from, const SLMSG_HDLR *hdlr)
{
	ANCHOR	*head;
	LIST	*p;

	if (!hdlr || !initialized) return;
	head = &(dispatch[type][from]);

	if ((p = list_lookup (head, hdlr->func)) == NULL) return;

	IDBGLOG3 ("(type=%s, from=%s, func=0x%8.8x)\n",
		SLMSG_TYPE_lookup_code (type), SLMSG_FROM_lookup_code (from), (unsigned int)hdlr->func);

	SL_LOCK_take (head->lock);
	if (p->prev != NULL)
		p->prev->next = p->next;
	else
		head->list = p->next;

	if (p->next != NULL)
		p->next->prev = p->prev;

	//LISTDUMP (head);

	SL_LOCK_drop (head->lock);

	SL_SAFE_FREE (p->name);
	SL_SAFE_FREE (p);
}

/*! Returns handler information for first registered handler
 *
 * \param from	message source for which to query handler hook
 * \param type	message type for which to query handler hook
 * \param list  pointer to storage for pointer to handler query list
 */
void
SLMSG_hdlr_query (SLMSG_TYPE type, SLMSG_FROM from, SLMSG_HQL **list)
{
	ANCHOR		*head;
	LIST		*p;
	SLMSG_HQL	*l, *e;

	if (!list || !initialized) return;
	*list = l = NULL;
	head = &(dispatch[type][from]);

	SL_LOCK_take (head->lock);
	if (head->list) {
		p = head->list;
		while (p) {
			SL_SAFE_CALLOC (e, 1, sizeof (SLMSG_HQL));

			SL_SAFE_STRDUP (e->hdlr.name, p->name);
			e->hdlr.func = p->func;
			e->hdlr.mode = p->mode;
			e->hdlr.attr = p->attr;

			if (l) l->next = e; else *list = e;
			l = e;

			p = p->next;
		}
	}
	SL_LOCK_drop (head->lock);
}

/*! Frees handler query list
 *
 * \param list	pointer to storage with pointer to query list
 */
void
SLMSG_hdlr_query_free (SLMSG_HQL **list)
{
	SLMSG_HQL	*p, *q;

	if (!list || !*list) return;

	p = *list; *list = NULL;
	while (p) {
		q = p; p = p->next;
                SL_SAFE_FREE (q->hdlr.name);
		SL_SAFE_FREE (q);
	}
}

/*! Returns attribute info string
 *
 * \param attr	attribute value
 * \return	pointer to dynamically allocated string with attribute info
 */
static char *
lookup_SLMSG_ATTR (SLMSG_ATTR attr)
{
	const char	*list[SLMSG_ATTR_ID_LIMIT+1];
	SLMSG_ATTR_ID	id;
	unsigned int	idx, cnt, len;
	char		*str = NULL;

	memset ((char *)list, 0, sizeof (list)); idx = 0;
	for (id=SLMSG_ATTR_ID_START+1; id <= SLMSG_ATTR_ID_LIMIT; id++) {
		if (attr & (1 << id)) {
			list[idx] = SLMSG_ATTR_ID_lookup (id);
			idx++;
		}
	}

	cnt = idx;
	if (cnt == 0) {
		list[0] = SLMSG_ATTR_ID_lookup (SLMSG_ATTR_ID_START);
		cnt = 1;
	}

	len = 0;
	for (idx = 0; idx < cnt; idx++) {
		len += strlen (list[idx]);
	}
	len += (cnt - 1) * 2;

	SL_SAFE_CALLOC (str, len + 1, 1);

	strcat (str, list[0]);
	for (idx = 1; idx < cnt; idx++) {
		strcat (str, ", ");
		strcat (str, list[idx]);
	}
	return (str);
}

/*! Sends state information message
 *
 * \param fmt	pointer to format string
 * \param ...	format string arguments
 */
static void
saystate (char *fmt, ...)
{
	va_list	AP;
	SLMSG	msg;

	va_start (AP, fmt);

	msg.type	= SLMSG_TYPE_STATE;
	msg.from	= SLMSG_FROM_SLMSG;
	msg.prio	= SLMSG_PRIO_DEFAULT;
	msg.data.source      = NULL;
	msg.data.type        = SLMSG_DATA_TYPE_PRINTF;
	msg.data.u.printf.fmt  = fmt;
	msg.data.u.printf.args = AP;
	SLMSG_send (&msg);

	va_end (AP);
}

/*! Reports messaging subsystem state
 *
 * \param extended	extended state report level flag
 */
void
SLMSG_statereport (SL_BOOL extended)
{
	SLMSG_TYPE	type;
	SLMSG_FROM	from;
	ANCHOR		*head;
	LIST		*p;
	char		*s;

	saystate ("### Messaging subsystem state:\n");
	saystate ("\tinitialized = %s\n", SL_BOOL_tostr (initialized));

	for (type = SLMSG_TYPE_START; type <= SLMSG_TYPE_LIMIT; type++) {
		for (from = SLMSG_FROM_START; from <= SLMSG_FROM_LIMIT; from++) {
			head = &(dispatch[type][from]);
			if (head->list == NULL) continue;

			if (extended) {
				saystate ("\t[%u]%s from [%u]%s\n", type, SLMSG_TYPE_lookup_code (type), from, SLMSG_FROM_lookup_code (from));
				saystate ("\t\tlock = 0x%8.8x\n", head->lock);
				saystate ("\t\tlist = 0x%8.8x\n", head->list);
			} else {
				saystate ("\t%s from %s\n", SLMSG_TYPE_lookup_code (type), SLMSG_FROM_lookup_code (from));
			}

			/* Don't use locking to prevent deadlock here */
			p = head->list;
			while (p != NULL) {
				s = lookup_SLMSG_ATTR (p->attr);
				if (extended) {
					saystate ("\t\taddr = 0x%8.8x\n", p);
					saystate ("\t\t\tname = \"%s\"\n", p->name);
					saystate ("\t\t\tfunc = 0x%8.8x\n", p->func);
					saystate ("\t\t\tmode = 0x%8.8x (%s)\n", p->mode, SLMSG_MODE_lookup (p->mode), p->mode);
					saystate ("\t\t\tattr = 0x%8.8x (%s)\n", p->attr, s);
					saystate ("\t\t\tprev = 0x%8.8x\n", p->prev);
					saystate ("\t\t\tnext = 0x%8.8x\n", p->next);
				} else {
					saystate ("\t\tname = \"%s\"\n", p->name);
					saystate ("\t\t\tmode = %s\n", SLMSG_MODE_lookup (p->mode));
					saystate ("\t\t\tattr = %s\n", s);
				}
				SL_SAFE_FREE (s);
				p = p->next;
			}
		}
	}
	saystate ("\n");
}
