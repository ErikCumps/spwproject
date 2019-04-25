/** \file
 * The SPWaW Library - snapshot list API.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 *
 * \addtogroup API
 * @{
 */

#ifndef	SPWAW_LIB_SNAPLIST_H
#define	SPWAW_LIB_SNAPLIST_H	1

#include <spwawlib_api.h>
#include <spwawlib_defines.h>
#include <spwawlib_snapshot.h>

/* SPWAW snapshot list element */
typedef struct s_SPWAW_SNAPLIST_NODE {
	char			dir[MAX_PATH+1];		/*!< file directory			*/
	char			filename[MAX_PATH+1];		/*!< file name				*/
	char			filepath[MAX_PATH+1];		/*!< full file path			*/
	unsigned int 		filesize;			/*!< file size				*/
	FILETIME		filedate;			/*!< file modification time		*/
	char			filestamp[20];			/*!< file modification time as str	*/
	SPWAW_SNAPSHOT_INFO	info;				/*!< snapshot info struct		*/
} SPWAW_SNAPLIST_NODE;

/* SPWAW snapshot list */
typedef struct s_SPWAW_SNAPLIST {
	unsigned long		cnt;				/*!< number of nodes in list		*/
	SPWAW_SNAPLIST_NODE	**list;				/*!< pointer to list array		*/
	unsigned long		len;				/*!< number of available nodes		*/
} SPWAW_SNAPLIST;



/*** API ***/

extern SPWAWLIB_API SPWAW_ERROR		SPWAW_snaplist		(const char *dir, SPWAW_SNAPLIST *ignore, SPWAW_SNAPLIST **list);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_snaplist_new	(SPWAW_SNAPLIST **list);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_snaplist_free	(SPWAW_SNAPLIST **list);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_snaplist_add	(SPWAW_SNAPLIST *list, SPWAW_SNAPLIST_NODE *node);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_snaplist_addcpy	(SPWAW_SNAPLIST *list, SPWAW_SNAPSHOT *snap);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_snaplist_clear	(SPWAW_SNAPLIST *list);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_snaplist_copy	(SPWAW_SNAPLIST *list, SPWAW_SNAPLIST *src);

#endif	/* SPWAW_LIB_SNAPLIST_H */

/** @} */
