/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAMEFILE_H
#define	GAMEFILE_H	1

#include <spwawlib_snapshot.h>

/* Forward declaration */
typedef struct s_GAMEDATA GAMEDATA;

#pragma pack(push, r1, 1)

#include "gamefile/struct00.h"
#include "gamefile/struct01.h"
#include "gamefile/struct02.h"
#include "gamefile/struct03.h"
#include "gamefile/struct04.h"
#include "gamefile/struct05.h"
#include "gamefile/struct06.h"
#include "gamefile/struct07.h"
#include "gamefile/struct08.h"
#include "gamefile/struct09.h"
#include "gamefile/struct10.h"
#include "gamefile/struct11.h"
#include "gamefile/struct12.h"
#include "gamefile/struct13.h"
#include "gamefile/struct14.h"
#include "gamefile/struct15.h"
#include "gamefile/struct16.h"
#include "gamefile/struct17.h"
#include "gamefile/struct18.h"
#include "gamefile/struct19.h"
#include "gamefile/struct20.h"
#include "gamefile/struct21.h"
#include "gamefile/struct22.h"
#include "gamefile/struct23.h"
#include "gamefile/struct24.h"
#include "gamefile/struct25.h"
#include "gamefile/struct26.h"
#include "gamefile/struct27.h"
#include "gamefile/struct28.h"
#include "gamefile/struct29.h"
#include "gamefile/struct30.h"
#include "gamefile/struct31.h"
#include "gamefile/struct32.h"
#include "gamefile/struct33.h"
#include "gamefile/struct34.h"
#include "gamefile/struct35.h"
#include "gamefile/struct36.h"
#include "gamefile/struct37.h"
#include "gamefile/struct38.h"
#include "gamefile/struct39.h"
#include "gamefile/struct40.h"
#include "gamefile/struct41.h"
#include "gamefile/struct42.h"
#include "gamefile/struct43.h"
#include "gamefile/struct44.h"
#include "gamefile/struct45.h"
#include "gamefile/struct46.h"
#include "gamefile/struct47.h"
#include "gamefile/struct48.h"
#include "gamefile/struct51.h"

#pragma pack(pop, r1)

typedef struct s_CMTDATA {
	char	title[SPWAW_AZSCMTTITLE];	/* Savegame title	*/
	char	mapsrc[SPWAW_AZSCMTMAPSRC];	/* Savegame map source?	*/
} CMTDATA;

typedef struct s_SECMAP {
	int	idx;	/* index	*/
	void	*ptr;	/* offset	*/
	DWORD	size;	/* size		*/
} SECMAP;

typedef struct s_GAMEDATA {
	CMTDATA		cmt;
	SECMAP		MAP[SECMAXID+1];
	STRUCT00	sec00;	/* start		*/
	STRUCT01	sec01;	/* units		*/
	STRUCT02	sec02;	/* map_icon_main	*/
	STRUCT03	sec03;	/* map_icon_sec3	*/
	STRUCT04	sec04;	/* map_icon_sec4	*/
	STRUCT05	sec05;	/* map_icon_sec5	*/
	STRUCT06	sec06;	/* map_icon_sec6	*/
	STRUCT07	sec07;	/* map_icon_sec7	*/
	STRUCT08	sec08;	/* map_terrain		*/
	STRUCT09	sec09;	/* map_data0		*/
	STRUCT10	sec10;
	STRUCT11	sec11;
	STRUCT12	sec12;
	STRUCT13	sec13;
	STRUCT14	sec14;
	STRUCT15	sec15;
	STRUCT16	sec16;
	STRUCT17	sec17;	/* units_pos		*/
	STRUCT18	sec18;
	STRUCT19	sec19;
	STRUCT20	sec20;
	STRUCT21	sec21;
	STRUCT22	sec22;
	STRUCT23	sec23;
	STRUCT24	sec24;
	STRUCT25	sec25;
	STRUCT26	sec26;	/* credits		*/
	STRUCT27	sec27;
	STRUCT28	sec28;	/* map_data1		*/
	STRUCT29	sec29;	/* map_data2		*/
	STRUCT30	sec30;
	STRUCT31	sec31;
	STRUCT32	sec32;
	STRUCT33	sec33;
	STRUCT34	sec34;	/* leaders		*/
	STRUCT35	sec35;	/* formations		*/
	STRUCT36	sec36;
	STRUCT37	sec37;	/* game			*/
	STRUCT38	sec38;	/* map_width		*/
	STRUCT39	sec39;	/* map_height		*/
	STRUCT40	sec40;
	STRUCT41	sec41;
	STRUCT42	sec42;
	STRUCT43	sec43;
	STRUCT44	sec44;
	STRUCT45	sec45;
	STRUCT46	sec46;
	STRUCT47	sec47;
	STRUCT48	sec48;	/* rec_header		*/
	STRUCT51	sec51;
};

typedef struct s_GAMEFILE {
	char		*cmt_name;
	int		cmt_fd;
	FILETIME	cmt_date;
	char		*dat_name;
	int		dat_fd;
	FILETIME	dat_date;
} GAMEFILE;

typedef struct s_GAMEINFO {
	char		path[MAX_PATH+1];
	char		file[MAX_PATH+1];
	FILETIME	date;
	char		stamp[32];
	char		location[SPWAW_AZSLOCATION+1];
	char		comment[SPWAW_AZSCMTTITLE+1];
} GAMEINFO;

extern SECMAP	*gamedata_secmap		(void);

extern bool	gamedata_load_cmt	(GAMEFILE *file, CMTDATA *dst);
extern bool	gamedata_load_all	(GAMEFILE *file, GAMEDATA *data);
extern bool	gamedata_load_section	(GAMEFILE *file, DWORD sec, void *dst, unsigned long len);

extern GAMEDATA	*game_load_full		(const char *dir, unsigned int id, GAMEINFO *info);
extern bool	game_load_cmt		(const char *dir, unsigned int id, CMTDATA *dst);
extern bool	game_load_section	(const char *dir, unsigned int id, int sec, void *dst, unsigned long len);
extern bool	game_load_info		(const char *dir, unsigned int id, GAMEINFO *info);
extern void	game_free		(GAMEDATA *game);



#endif	/* GAMEFILE_H */
