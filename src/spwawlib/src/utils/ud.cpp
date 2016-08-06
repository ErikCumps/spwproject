/** \file
 * The SPWaW Library - utility code: unknown data handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include <spwawlib_ud.h>
#include "common/internal.h"

void
UD_init (SPWAW_UD *ud, DWORD size)
{
	if (!ud) return;

	clear_ptr (ud);
	ud->size = size;
}

SPWAW_UD *
UD_new (DWORD size)
{
	SPWAW_UD	*p = NULL;

	p = safe_malloc (SPWAW_UD);
	if (p) {
		UD_init (p, size);
		p->freeme = true;
	}
	return (p);
}

void
UD_add (SPWAW_UD *ud, DWORD addr, DWORD size, BYTE *data)
{
	DWORD		tlen = 0;
	SPWAW_UDEL	*p = NULL, *q;

	if (!ud) return;

	tlen = sizeof (*p) + size - 1;
	p = safe_smalloc (SPWAW_UDEL, tlen);
	if (!p) return;

	memset (p, 0, tlen);
	p->addr = addr;
	p->size = size;
	memcpy (p->data, data, p->size);

	if (ud->tail == NULL) {
		ud->head = ud->tail = p;
	} else {
		q = ud->head;
		while (q && (q->addr < addr)) q = q->next;
		if (q == NULL) {
			p->prev = ud->tail;
			ud->tail->next = p;
			ud->tail = p;
		} else if (q == ud->head) {
			p->next = ud->head;
			ud->head->prev = p;
			ud->head = p;
		} else {
			p->next = q;
			p->prev = q->prev;
			q->prev = p;
			p->prev->next = p;
		}
	}
	ud->cnt++;
}

void
UD_free (SPWAW_UD *ud)
{
	SPWAW_UDEL	*p, *q;

	if (!ud) return;

	p = ud->head;
	while (p) {
		q = p; p = p->next; safe_free (q);
	}
	if (ud->freeme) safe_free (ud);
}
