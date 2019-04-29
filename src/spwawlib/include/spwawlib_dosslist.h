/** \file
 * The SPWaW Library - dossier list API.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 *
 * \addtogroup API
 * @{
 */

#ifndef	SPWAW_LIB_DOSSLIST_H
#define	SPWAW_LIB_DOSSLIST_H	1

#include <spwawlib_api.h>
#include <spwawlib_defines.h>
#include <spwawlib_dossier.h>

/* SPWAW dossier list element */
typedef struct s_SPWAW_DOSSLIST_NODE {
	char			dir[MAX_PATH+1];		/*!< file directory			*/
	char			filename[MAX_PATH+1];		/*!< file name				*/
	char			filepath[MAX_PATH+1];		/*!< full file path			*/
	unsigned int 		filesize;			/*!< file size				*/
	FILETIME		filedate;			/*!< file modification time		*/
	char			filestamp[20];			/*!< file modification time as str	*/
	SPWAW_DOSSIER_INFO	info;				/*!< dossier info struct		*/
} SPWAW_DOSSLIST_NODE;

/* SPWAW dossier list */
typedef struct s_SPWAW_DOSSLIST {
	unsigned long		cnt;				/*!< number of nodes in list		*/
	SPWAW_DOSSLIST_NODE	**list;				/*!< pointer to list array		*/
	unsigned long		len;				/*!< number of available nodes		*/
} SPWAW_DOSSLIST;



/*** API ***/

extern SPWAWLIB_API SPWAW_ERROR		SPWAW_dosslist		(const char *dir, SPWAW_DOSSLIST *ignore, SPWAW_DOSSLIST **list);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_dosslist_new	(SPWAW_DOSSLIST **list);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_dosslist_free	(SPWAW_DOSSLIST **list);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_dosslist_add	(SPWAW_DOSSLIST *list, SPWAW_DOSSIER *doss);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_dosslist_addcpy	(SPWAW_DOSSLIST *list, SPWAW_DOSSLIST_NODE *node);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_dosslist_clear	(SPWAW_DOSSLIST *list);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_dosslist_copy	(SPWAW_DOSSLIST *list, SPWAW_DOSSLIST *src);

#endif	/* SPWAW_LIB_DOSSLIST_H */

/** @} */
