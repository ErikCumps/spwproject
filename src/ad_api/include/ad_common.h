/** \file
 * The Aggregate Data API: common types.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	AD_COMMON_H
#define	AD_COMMON_H	1

#include <system/independence.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long	AD_size_t;
typedef signed long	AD_off_t;

struct AD_node_struct {
	void	*data;
};
typedef struct AD_node_struct *	AD_node_t;

typedef void	(*AD_destructor_t)	(AD_node_t);

typedef int	(*AD_inspector_t)	(AD_node_t);

typedef int	(*AD_comparator_t)	(AD_node_t, AD_node_t);

typedef void *	(*AD_duplicator_t)	(AD_node_t);

typedef void *	AD_iterator_t;

typedef int	(*AD_OOM_hook_t)	(char *, char *, unsigned long, unsigned long);

extern AD_OOM_hook_t	AD_OOM_set_hook	(AD_OOM_hook_t hook);

#ifdef __cplusplus
}
#endif

#endif	/* !AD_COMMON_H */

