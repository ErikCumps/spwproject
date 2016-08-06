#ifndef	SPWAW_LIB_SAVELIST_H
#define	SPWAW_LIB_SAVELIST_H	1

#include <spwawlib_api.h>
#include <spwawlib_defines.h>

/* SPWAW savegame info struct */
typedef struct s_SPWAW_SAVEGAME_INFO {
	char			stamp[32];			/*!< game turn and date info		*/
	char			location[SPWAW_AZSLOCATION+1];	/*!< game location			*/
	char			comment[SPWAW_AZSCMTTITLE+1];	/*!< game comment			*/
} SPWAW_SAVEGAME_INFO;

/* SPWAW savegame list element */
typedef struct s_SPWAW_SAVELIST_NODE {
	unsigned int		id;				/*!< id					*/
	char			dir[MAX_PATH+1];		/*!< file directory			*/
	char			filename[MAX_PATH+1];		/*!< file name				*/
	char			filepath[MAX_PATH+1];		/*!< full file path			*/
	unsigned int 		filesize;			/*!< file size				*/
	FILETIME		filedate;			/*!< file modification time		*/
	char			filestamp[20];			/*!< file modification time as str	*/
	SPWAW_SAVEGAME_INFO	info;				/*!< game info				*/
} SPWAW_SAVELIST_NODE;

/* SPWAW savegame list */
typedef struct s_SPWAW_SAVELIST {
	unsigned long		cnt;				/*!< number of nodes in list		*/
	SPWAW_SAVELIST_NODE	**list;				/*!< pointer to list array		*/
	unsigned long		len;				/*!< number of available nodes		*/
} SPWAW_SAVELIST;



/*** API ***/

extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savelist		(const char *dir, SPWAW_SAVELIST *ignore, SPWAW_SAVELIST **list);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savelist_new	(SPWAW_SAVELIST **list);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savelist_free	(SPWAW_SAVELIST **list);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savelist_add	(SPWAW_SAVELIST *list, SPWAW_SAVELIST_NODE *node);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savelist_addcpy	(SPWAW_SAVELIST *list, SPWAW_SAVELIST_NODE *node);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savelist_clear	(SPWAW_SAVELIST *list);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savelist_copy	(SPWAW_SAVELIST *list, SPWAW_SAVELIST *src);

#endif	/* SPWAW_LIB_SAVELIST_H */