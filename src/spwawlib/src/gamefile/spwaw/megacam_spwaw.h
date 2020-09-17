/** \file
 * The SPWaW Library - gamefile handling - SPWaW Mega Campaign data.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAMEDATA_MEGACAM_SPWAW_H
#define	GAMEDATA_MEGACAM_SPWAW_H	1

#include <spwawlib_defines.h>
#include "gamefile/gamefile.h"
#include "gamefile/gamedata.h"
#include "gamefile/spwaw/defines_spwaw.h"

#pragma pack(push, r1, 1)

typedef BYTE MEGACAM_COREFLAGS[SPWAW_UNITPCOUNT];

typedef struct s_MEGACAM_LDRNAME {
	char	name[SPWAW_AZSANAME];
} MEGACAM_LDRNAME;

typedef struct s_MEGACAMDATA_SPWAW {
	union u_u {
		char	raw[11256];
		struct s_d {
			/* section #0 */
			char			campaign[SPWAW_AZSGECAMPAIGN];	/* General's Edition Campaign	*/
			char			__data00[1];
			char			title[SPWAW_AZSGECAMTITLE];	/* Savegame title		*/
			char			__data01[3];
			DWORD			turn;				/* Current battle turn		*/
			char			__data02[4];
			DWORD			battle;				/* Current battle?		*/
			char			__data03[4];
			char			__data04[136];
			/* section #1 */
			MEGACAM_COREFLAGS	coreflags;			/* Core unit flags?		*/
			/* section #2 */
			char			__data20[SPWAW_UNITPCOUNT];
			/* section #3 */
			char			__data30[SPWAW_UNITPCOUNT];
			/* section #4 */
			MEGACAM_LDRNAME		ldrnames[SPWAW_UNITPCOUNT];	/* Core unit leader names?	*/
			/* section #5 */
			char			__data50[SPWAW_UNITPCOUNT];
			/* section #6 */
			char			__data60[SPWAW_UNITPCOUNT];
			/* section #7 */
			char			__data70[3*SPWAW_UNITPCOUNT];
			/* section #8 */
			char			__data80[SPWAW_UNITPCOUNT];
			/* section #9 */
			char			__data90[SPWAW_UNITPCOUNT];
			/* section #10 */
			char			__data100[1032];
		} d;
	} u;
} MEGACAMDATA_SPWAW;

#pragma pack(pop, r1)

extern bool	gamedata_init_spwaw_megacam	(METADATA *data);
extern void	gamedata_free_spwaw_megacam	(METADATA *data);

extern bool	gamedata_load_spwaw_megacam	(GAMEFILE *file, METADATA *dst);
extern bool	gamedata_save_spwaw_megacam	(METADATA *src, GAMEFILE *file);


#endif	/* GAMEDATA_MEGACAM_SPWAW_H */
