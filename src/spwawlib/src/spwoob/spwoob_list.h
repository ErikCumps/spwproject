/** \file
 * The SPWaW Library - SPWaW OOB list handling.
 *
 * Copyright (C) 2018-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
	 */

#ifndef	INTERNAL_SPWOOB_LIST_H
#define	INTERNAL_SPWOOB_LIST_H	1

#include <spwawlib_api.h>
#include <spwawlib_spwoob.h>
#include <spwawlib_spwoob_list.h>

/* SPWAW SPWOOB list element */
typedef struct s_SPWAW_SPWOOB_LIST_NODE {
	SPWOOB			*data;				/*!< SPWOOB data			*/
	unsigned long		refcnt;				/*!< reference count			*/
} SPWAW_SPWOOB_LIST_NODE;

/* SPWAW SPWOOB list */
typedef struct s_SPWAW_SPWOOB_LIST {
	unsigned long		cnt;				/*!< number of SPWOOBs in list		*/
	SPWAW_SPWOOB_LIST_NODE	**list;				/*!< pointer to list array		*/
	unsigned long		len;				/*!< number of available nodes		*/
} SPWAW_SPWOOB_LIST;



/*** API ***/

extern SPWAW_ERROR	SPWOOB_LIST_new		(SPWAW_SPWOOB_LIST **list);
extern SPWAW_ERROR	SPWOOB_LIST_free	(SPWAW_SPWOOB_LIST **list);

extern SPWAW_ERROR	SPWOOB_LIST_add		(SPWAW_SPWOOB_LIST *list, SPWOOB *spwoob, unsigned long *idx = NULL);
extern SPWAW_ERROR	SPWOOB_LIST_del		(SPWAW_SPWOOB_LIST *list, SPWOOB *spwoob);
extern SPWAW_ERROR	SPWOOB_LIST_match	(SPWAW_SPWOOB_LIST *list, SPWOOB *spwoob, unsigned long *idx);

extern SPWAW_ERROR	SPWOOB_LIST_clear	(SPWAW_SPWOOB_LIST *list);
extern SPWAW_ERROR	SPWOOB_LIST_compact	(SPWAW_SPWOOB_LIST *list);

extern SPWAW_ERROR	SPWOOB_LIST_load	(SPWAW_SPWOOB_LIST *list, int fd);
extern SPWAW_ERROR	SPWOOB_LIST_save	(SPWAW_SPWOOB_LIST *list, int fd, bool compress);


extern SPWAW_ERROR	SPWOOB_LIST_idx2spwoob	(SPWAW_SPWOOB_LIST *list, unsigned long idx, SPWOOB **spwoob);
extern SPWAW_ERROR	SPWOOB_LIST_spwoob2idx	(SPWAW_SPWOOB_LIST *list, SPWOOB *spwoob, unsigned long *idx);

extern SPWAW_ERROR	SPWOOB_LIST_takeref	(SPWAW_SPWOOB_LIST *list, unsigned long idx, SPWOOB **spwoob);

extern SPWAW_ERROR	SPWOOB_LIST_debug_log	(SPWAW_SPWOOB_LIST *list, char *msg);

#endif	/* INTERNAL_SPWOOB_LIST_H */
