/** \file
 * The Aggregate Data API: private data integrity validation code.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	AD_MAGIC_H
#define	AD_MAGIC_H	1

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define	ADMAGIC_NODE	0x534c4200
#define	ADMAGIC_HASH	0x534c4201
#define	ADMAGIC_LIST	0x534c4202

typedef struct {
	unsigned int	magic;
} AD_magic_t;

extern void *check_magic (void *ptr, unsigned int magic);

typedef struct {
	void		*data;
	unsigned int	magic;
} AD_node_magic_t;

extern void *check_node_magic (void *ptr, unsigned int magic);

extern int call_OOM_hook (char *file, char *func, unsigned long line, unsigned long size);

#define	NEW(var)											\
	do {												\
		var = malloc (sizeof (*var));								\
		if (var == NULL) call_OOM_hook (__FILE__, __FUNCTION__, __LINE__, (sizeof (*var)));	\
		if (var != NULL) memset (var, 0, sizeof (*var));					\
	} while (0)

#define	NEW_ROF(var,failed)										\
	do {												\
		var = malloc (sizeof (*var));								\
		if (var == NULL) {									\
			call_OOM_hook (__FILE__, __FUNCTION__, __LINE__, (sizeof (*var)));		\
			return (failed);								\
		}											\
		memset (var, 0, sizeof (*var));								\
	} while (0)

#ifdef __cplusplus
}
#endif

#endif	/* !AD_MAGIC_H */

