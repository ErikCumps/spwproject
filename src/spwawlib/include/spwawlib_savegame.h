/** \file
 * The SPWaW Library - savegame API.
 *
 * Copyright (C) 2016-2020 Erik Cumps <erik.cumps@gmail.com>
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
#include <spwawlib_types.h>

/*! SPWAW savegame metadata */
typedef struct s_SPWAW_SAVEGAME_METADATA {
	void	*data;				/*!< pointer to data buffer		*/
	DWORD	size;				/*!< size of data buffer		*/
} SPWAW_SAVEGAME_METADATA;

/*! SPWAW uncompressed savegame section data */
typedef struct s_SPWAW_SAVEGAME_SECTION {
	void	*data;				/*!< pointer to data buffer		*/
	DWORD	size;				/*!< size of data buffer		*/
	int	idx;				/*!< section index number		*/
} SPWAW_SAVEGAME_SECTION;

/*! SPWAW savegame content structure */
typedef struct s_SPWAW_SAVEGAME {
	SPWAW_GAME_TYPE		gametype;	/*!< savegame game type			*/
	SPWAW_SAVE_TYPE		savetype;	/*!< savegame save type			*/
	SPWAW_SAVEGAME_METADATA	metadata;	/*!< savegame metadata content		*/
	int			seccnt;		/*!< number of savegame sections	*/
	SPWAW_SAVEGAME_SECTION	*seclst;	/*!< savegame sections content list	*/
} SPWAW_SAVEGAME;



/*** API ***/

extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savegame_new	(SPWAW_GAME_TYPE gametype, SPWAW_SAVE_TYPE savetype, SPWAW_SAVEGAME **savegame);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savegame_free	(SPWAW_SAVEGAME **savegame);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savegame_load	(SPWAW_SAVEGAME_DESCRIPTOR *sgd, SPWAW_SAVEGAME **savegame);
extern SPWAWLIB_API SPWAW_ERROR		SPWAW_savegame_save	(SPWAW_SAVEGAME **savegame, SPWAW_SAVEGAME_DESCRIPTOR *sgd);

#endif	/* SPWAW_LIB_SAVEGAME_H */

/** @} */