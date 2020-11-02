/** \file
 * The SPWaW Library - savegame list API.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 *
 * \addtogroup API
 * @{
 */

#ifndef	SPWAW_LIB_SAVELIST_H
#define	SPWAW_LIB_SAVELIST_H	1

#include <spwawlib_api.h>
#include <spwawlib_defines.h>
#include <spwawlib_types.h>
#include <spwawlib_snapshot.h>

/* SPWAW savegame info struct */
typedef struct s_SPWAW_SAVEGAME_INFO {
	char				stamp[32];			/*!< game turn and date info		*/
	char				location[SPWAW_AZSLOCATION+1];	/*!< game location			*/
	char				title[SPWAW_AZSCMTTITLE+1];	/*!< game title				*/
	SPWAW_BATTLE_TYPE		type;				/*!< game battle type			*/
	SPWAW_GAME_TYPE			gametype;			/*!< game type				*/
	SPWAW_SAVE_TYPE			savetype;			/*!< save type				*/
} SPWAW_SAVEGAME_INFO;

/* SPWAW savegame list element */
typedef struct s_SPWAW_SAVELIST_NODE {
	SPWAW_SAVEGAME_DESCRIPTOR	sgd;				/*!< Savegame descriptor		*/
	char				filename[MAX_PATH+1];		/*!< file name				*/
	char				filepath[MAX_PATH+1];		/*!< full file path			*/
	unsigned int 			filesize;			/*!< file size				*/
	FILETIME			filedate;			/*!< file modification time		*/
	char				filestamp[20];			/*!< file modification time as str	*/
	SPWAW_SAVEGAME_INFO		info;				/*!< game info				*/
} SPWAW_SAVELIST_NODE;

/* SPWAW savegame list target */
typedef struct s_SPWAW_SAVELIST_TARGET {
	SPWAW_GAME_TYPE			gametype;			/*!< game type				*/
	SPWAW_DOSSIER_TYPE		type;				/*!< game dossier type			*/
	char				*oobdir;			/*!< game OOB directory			*/
} SPWAW_SAVELIST_TARGET;

/* SPWAW savegame list */
typedef struct s_SPWAW_SAVELIST {
	SPWAW_SAVELIST_TARGET		target;				/*!< savegame list target purpose	*/
	unsigned long			cnt;				/*!< number of nodes in list		*/
	SPWAW_SAVELIST_NODE		**list;				/*!< pointer to list array		*/
	unsigned long			len;				/*!< number of available nodes		*/
} SPWAW_SAVELIST;



/*** API ***/

extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savelist		(SPWAW_SAVELIST_TARGET *target, const char *dir, SPWAW_SAVELIST *ignore, SPWAW_SAVELIST **list);

extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savelist_new	(SPWAW_SAVELIST_TARGET *target, SPWAW_SAVELIST **list);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savelist_free	(SPWAW_SAVELIST **list);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savelist_add	(SPWAW_SAVELIST *list, SPWAW_SNAPSHOT *snap);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savelist_addcpy	(SPWAW_SAVELIST *list, SPWAW_SAVELIST_NODE *node);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savelist_clear	(SPWAW_SAVELIST *list);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savelist_copy	(SPWAW_SAVELIST *list, SPWAW_SAVELIST *src);

#endif	/* SPWAW_LIB_SAVELIST_H */

/** @} */
