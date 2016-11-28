/** \file
 * The SPWaW war cabinet - data model handling - filters (EXPERIMENTAL).
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "model_filter.h"

#define	SIZE	8

static void
filter_alloc_list (MDL_MF_TGTLST *list, int size)
{
	if (!list) return;

	list->size = size; list->cnt = 0;
	if (!size) return;

	SL_SAFE_CALLOC (list->list, list->size, sizeof (MDL_MF_TGT));
}

static void
filter_free_list (MDL_MF_TGTLST *list)
{
	if (!list) return;

	if (list->list) {
		for (int i=0; i<list->cnt; i++) {
			SL_SAFE_FREE (list->list[i].name);
		}
		SL_SAFE_FREE (list->list);
	}
	memset (list, 0, sizeof (*list));
}

static void
filter_grow_list (MDL_MF_TGTLST *list, unsigned int size)
{
	MDL_MF_TGT	*nptr = NULL;

	if (!list || !size) return;
	list->size += size;

	SL_SAFE_CALLOC (nptr, list->size, sizeof (MDL_MF_TGT));
	memcpy (nptr, list->list, list->cnt * sizeof (MDL_MF_TGT));

	SL_SAFE_FREE (list->list); list->list = nptr;
}

void
MDL_MF_TGTLST_new (MDL_MF_TGTLST **list)
{
	MDL_MF_TGTLST	*ptr = NULL;

	CAPNULLARG (list);
	*list = NULL;

	SL_SAFE_CALLOC (ptr, 1, sizeof (MDL_MF_TGTLST));

	/* Preallocate SIZE list entries */
	filter_alloc_list (ptr, SIZE);

	*list = ptr;
}

void
MDL_MF_TGTLST_free (MDL_MF_TGTLST **list)
{
	MDL_MF_TGTLST	*ptr = NULL;

	CAPNULLARG (list);
	ptr = *list; *list = NULL;
	if (!ptr) return;

	filter_free_list (ptr);
	SL_SAFE_FREE (ptr);
}

void
MDL_MF_TGTLST_clear (MDL_MF_TGTLST *list)
{
	filter_free_list (list);
	filter_alloc_list (list, SIZE);
}

void
MDL_MF_TGTLST_add (MDL_MF_TGTLST *list, MDL_MF_TGTID id, char *name)
{
	int	i;

	if (!list || !name) return;

	if (list->cnt >= list->size) filter_grow_list (list, SIZE);

	i = list->cnt++;
	list->list[i].id = id;
	SL_SAFE_STRDUP (list->list[i].name, name);
}

//////////////////////////////////////////////////////////////////////

static void
filter_alloc_list (xMDL_FILTER_LIST *list, int size)
{
	if (!list) return;

	list->size = size; list->used = 0;
	if (!size) return;

	SL_SAFE_CALLOC (list->data, list->size, sizeof (xMDL_FILTER_TARGET));
}

static void
filter_free_list (xMDL_FILTER_LIST *list)
{
	if (!list) return;

	if (list->size) {
		for (unsigned int i=0; i<list->used; i++) {
			SL_SAFE_FREE (list->data[i].name);
			delete list->data[i].value;
		}
		SL_SAFE_FREE (list->data);
	}
	list->size = list->used = 0; list->data = NULL;
}

static void
filter_grow_list (xMDL_FILTER_LIST *list, unsigned int size)
{
	xMDL_FILTER_TARGET	*nptr = NULL;

	if (!list || !size) return;

	list->size += size;
	SL_SAFE_CALLOC (nptr, list->size, sizeof (xMDL_FILTER_TARGET));
	memcpy (nptr, list->data, list->used * sizeof (xMDL_FILTER_TARGET));
	SL_SAFE_FREE (list->data); list->data = nptr;
}

void
xMDL_FILTER_new (xMDL_FILTER_LIST **list)
{
	xMDL_FILTER_LIST	*ptr = NULL;

	CAPNULLARG (list);
	*list = NULL;

	SL_SAFE_CALLOC (ptr, 1, sizeof (xMDL_FILTER_LIST));

	/* Preallocate SIZE list entries */
	filter_alloc_list (ptr, SIZE);

	/* First entry always used for 'none' value */
	ptr->used = 1;
	SL_SAFE_STRDUP (ptr->data[0].name, "none");
	ptr->data[0].value = new QVariant();

	*list = ptr;
}

void
xMDL_FILTER_free (xMDL_FILTER_LIST **list)
{
	xMDL_FILTER_LIST	*ptr = NULL;

	CAPNULLARG (list);
	ptr = *list; *list = NULL;
	if (!ptr) return;

	filter_free_list (ptr);
	SL_SAFE_FREE (ptr);
}

void
xMDL_FILTER_clear (xMDL_FILTER_LIST *list)
{
	filter_free_list (list);
	filter_alloc_list (list, SIZE);

	/* Reinit first entry */
	list->used = 1;
	SL_SAFE_STRDUP (list->data[0].name, "none");
	list->data[0].value = new QVariant();

}

void
xMDL_FILTER_add (xMDL_FILTER_LIST *list, char *name, const QVariant &value)
{
	int	i;

	if (!list || !name) return;

	if (list->used >= list->size) filter_grow_list (list, SIZE);

	i = list->used++;
	SL_SAFE_STRDUP (list->data[i].name, name);
	list->data[i].value = new QVariant (value);
}
