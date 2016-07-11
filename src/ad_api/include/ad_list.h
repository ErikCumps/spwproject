/** \file
 * The Aggregate Data API: the double-linked list interface.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	AD_LIST_H
#define	AD_LIST_H	1

#include "ad_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void *		ADlist_t;

extern ADlist_t		ADlist_create		(AD_destructor_t df);
extern AD_size_t	ADlist_destroy_safe	(ADlist_t *list);
#define	ADlist_destroy(list)	ADlist_destroy_safe (&(list))
extern AD_size_t	ADlist_clear		(ADlist_t list);

extern AD_size_t	ADlist_size		(ADlist_t list);

extern AD_node_t	ADlist_add		(ADlist_t list, void *data);
extern void *		ADlist_del		(ADlist_t list, AD_node_t node);

extern AD_node_t	ADlist_read		(ADlist_t list);
extern void		ADlist_rewind		(ADlist_t list);
extern AD_off_t		ADlist_tell		(ADlist_t list);
extern void		ADlist_seek		(ADlist_t list, AD_off_t offset);

#ifdef __cplusplus
}
#endif

#endif	/* !AD_LIST_H */

