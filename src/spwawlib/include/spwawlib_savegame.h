/** \file
 * The SPWaW Library - savegame API.
 *
 * Copyright (C) 2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 *
 * \addtogroup API
 * @{
 */

#ifndef	SPWAW_LIB_SAVEGAME_H
#define	SPWAW_LIB_SAVEGAME_H	1

#include <spwawlib_api.h>
#include <spwawlib_defines.h>

/*! SPWAW savegame comment data */
typedef struct s_SPWAW_SAVEGAME_COMMENT {
	void	*data;		/*!< pointer to data buffer	*/
	DWORD	size;		/*!< size of data buffer	*/
} SPWAW_SAVEGAME_COMMENT;

/*! SPWAW uncompressed savegame section data */
typedef struct s_SPWAW_SAVEGAME_SECTION {
	void	*data;		/*!< pointer to data buffer	*/
	DWORD	size;		/*!< size of data buffer	*/
	int	idx;		/*!< section index number	*/
} SPWAW_SAVEGAME_SECTION;

/*! SPWAW savegame content structure */
typedef struct s_SPWAW_SAVEGAME {
	SPWAW_SAVEGAME_COMMENT	comment;			/*!< savegame comment content	*/
	SPWAW_SAVEGAME_SECTION	sections[SPWAW_SECTION_COUNT];	/*!< savegame sections content	*/
} SPWAW_SAVEGAME;



/*** API ***/

extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savegame_new	(SPWAW_SAVEGAME **game);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savegame_free	(SPWAW_SAVEGAME **game);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savegame_load	(const char *dir, int id, SPWAW_SAVEGAME **game);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savegame_save	(SPWAW_SAVEGAME **game, const char *dir, int id);

#endif	/* SPWAW_LIB_SAVEGAME_H */

/** @} */