/** \file
 * The Aggregate Data API: the double-linked list implementation.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include <malloc.h>
#include <stdio.h>

#include <stdlib.h>

/* \TODO allow concurrent access */

#include <ad_list.h>
#include "ad_magic.h"
#include <debug_memtrace.h>

struct list_element_t;

struct list_t {
	unsigned int		magic;		/* Used to check validity of hash structure pointer */
	AD_destructor_t		destruct;	/* Optional payload destructor */
	struct list_element_t	*head;		/* Pointer to head of linked list */
	struct list_element_t	*tail;		/* Pointer to tail of linked list */
	signed long		size;		/* Current number of elements in list */

	struct list_element_t	*curr_ptr;	/* Pointer to current element used with ADlist_read() */
	signed long		curr_off;	/* Offset of current element */
};

struct list_element_t {
	void			*data;		/* Hash element: payload */
	unsigned int		magic;
	struct list_t		*list;		/* Pointer to parent list */
	struct list_element_t	*next;		/* Pointer to next hash element in list */
	struct list_element_t	*prev;		/* Pointer to prev element in list */
};

ADlist_t
ADlist_create (AD_destructor_t df)
{
	struct list_t	*list = NULL;

	/* Create new list structure */
	NEW_ROF (list, NULL);
	list->magic    = ADMAGIC_LIST;
	list->destruct = df;
	list->size     = 0;
	list->head = list->tail = NULL;

	list->curr_ptr = NULL;
	list->curr_off = -1;

	return ((ADlist_t)list);
}

AD_size_t
ADlist_destroy_safe (ADlist_t *list)
{
	struct list_t		*l;
	struct list_element_t	*p, *q;
	AD_size_t		count = 0;

	/* Check list structure validity */
	if ((l = check_magic (*list, ADMAGIC_LIST)) == NULL) return (count);

	/* Invalidate list structure */
	l->magic = 0;

	/* Free list elements */
	p = l->head;
	while (p != NULL) {
		q = p; p = p->next; count++;
		if (l->destruct != NULL) l->destruct (q->data); else free (q->data);
		free (q);
	}

	/* Free list structure */
	memset (l, 0, sizeof (struct list_t)); free (l);
	*list = NULL;

	return (count);
}

AD_size_t
ADlist_clear (ADlist_t list)
{
	struct list_t		*l;
	struct list_element_t	*p, *q;
	AD_size_t		count = 0;

	/* Check list structure validity */
	if ((l = check_magic (list, ADMAGIC_LIST)) == NULL) return (count);

	/* Free list elements */
	p = l->head;
	while (p != NULL) {
		q = p; p = p->next; count++;
		if (l->destruct != NULL) l->destruct (q->data); else free (q->data);
		free (q);
	}

	return (count);
}

AD_size_t
ADlist_size (ADlist_t list)
{
	struct list_t		*l;

	/* Check list structure validity */
	if ((l = check_magic (list, ADMAGIC_LIST)) == NULL) return (0);

	return (l->size);
}

AD_node_t
ADlist_add (ADlist_t list, void *data)
{
	struct list_t		*l;
	struct list_element_t	*p;

	/* Check list structure validity */
	if ((l = check_magic (list, ADMAGIC_LIST)) == NULL) return (NULL);

	/* Create new list element */
	NEW_ROF (p, NULL);
	p->magic = ADMAGIC_NODE;
	p->data  = data;
	p->list  = l;
	p->next  = p->prev = NULL;

	/* Add element to list */
	if (l->tail == NULL) {
		l->tail = l->head = p;
	} else {
		p->prev = l->tail;
		p->prev->next = p;
		l->tail = p;

	}
	l->size++;

	/* Return node */
	return ((AD_node_t) p);
}

void *
ADlist_del (ADlist_t list, AD_node_t node)
{
	struct list_t		*l;
	struct list_element_t	*p;
	void			*data;

	/* Check list structure validity */
	if ((l = check_magic (list, ADMAGIC_LIST)) == NULL) return (NULL);

	/* Check node validity */
	if ((p = check_node_magic (node, ADMAGIC_NODE)) == NULL) return (NULL);

	/* Check list - node connection */
	if (p->list != l) return (NULL);

	/* Check list is non-empty */
	if (l->size == 0) return (NULL);

	/* Remove element from list */
	if (l->head == l->tail) {	/* single element in list */
		// \todo SANITY: if (l->head == NULL)
		l->head = l->tail = NULL;
	} else if (l->head == p) {	/* element at head of list */
		l->head = p->next;
		if (l->head == NULL) {
				/* OOPS! */
				fprintf (stderr, "*** OOPS: %s:%u:%s() list head/tail pointer corruption! ***\n\n",
					 __FILE__, __LINE__, __FUNCTION__);
				exit (42);
		}
		l->head->prev = NULL;
	} else if (l->tail == p) {	/* element at tail of list */
		l->tail = p->prev;
		if (l->tail == NULL) {
				/* OOPS! */
				fprintf (stderr, "*** OOPS: %s:%u:%s() list head/tail pointer corruption! ***\n\n",
					 __FILE__, __LINE__, __FUNCTION__);
				exit (42);
		}
		l->tail->next = NULL;
	} else {			/* element at other place in list */
		p->prev->next = p->next;
		p->next->prev = p->prev;
	}
	l->size--;

	if (l->curr_ptr == p) {
		l->curr_off = -1;
		l->curr_ptr = NULL;
	}

	/* Clean up */
	data = p->data;
	if (l->destruct != NULL) { l->destruct (data); data = NULL; }
	free (p);

	return (data);
}

AD_node_t
ADlist_read (ADlist_t list)
{
	struct list_t		*l;
	struct list_element_t	*e;

	/* Check list structure validity */
	if ((l = check_magic (list, ADMAGIC_LIST)) == NULL) return (NULL);

	/* Empty list? */
	if (l->size == 0) return (NULL);

	/* First read? */
	if (l->curr_off == -1) {
		l->curr_off = 1;
		l->curr_ptr = l->head->next;
		e = l->head;
	} else {
		e = l->curr_ptr;
		if (e != NULL) {
			l->curr_off++;
			l->curr_ptr = l->curr_ptr->next;
		}
	}

	return ((AD_node_t) e);
}

void
ADlist_rewind (ADlist_t list)
{
	struct list_t		*l;

	/* Check list structure validity */
	if ((l = check_magic (list, ADMAGIC_LIST)) == NULL) return;

	l->curr_off = -1;
	l->curr_ptr = NULL;
}

AD_off_t
ADlist_tell (ADlist_t list)
{
	struct list_t		*l;

	/* Check list structure validity */
	if ((l = check_magic (list, ADMAGIC_LIST)) == NULL) return (-1);

	return (l->curr_off);
}

void
ADlist_seek (ADlist_t list, AD_off_t offset)
{
	struct list_t		*l;
	signed long		off_dir;
	signed long		off_abs;

	/* Check list structure validity */
	if ((l = check_magic (list, ADMAGIC_LIST)) == NULL) return;

	off_dir = (offset < 0) ? 1 : 0;
	off_abs = off_dir ? -offset : offset;

	/* Check list size:
	 * if off >= 0 valid range is  0 ... (size - 1) :: +off_dir(0) = 0 ... (size - 1)
	 * if off <  0 valid range is -1 ... (0 - size) :: +off_dir(1) = 0 ... (1 - size)
	 */
	if ((off_abs - off_dir) >= l->size) return;

	/* Move to offset */
	if (off_dir == 0) {
		l->curr_off = 0;
		l->curr_ptr = l->head;
		while ((l->curr_off < off_abs) && (l->curr_ptr != NULL)) {
			l->curr_off++;
			l->curr_ptr = l->curr_ptr->next;
		}
	} else {
		l->curr_off = l->size - 1;
		l->curr_ptr = l->tail;
		while ((l->curr_off > (l->size - off_abs)) && (l->curr_ptr != NULL)) {
			l->curr_off--;
			l->curr_ptr = l->curr_ptr->prev;
		}
	}
}
