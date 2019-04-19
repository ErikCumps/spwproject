/** \file
 * The SPWaW Library - game data handling - SPWaW game data.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAMEDATA_SPWAW_H
#define	GAMEDATA_SPWAW_H	1

#include "gamefile/gamedata_secmap.h"

#pragma pack(push, r1, 1)
#include "gamefile/spwaw/section00_spwaw.h"
#include "gamefile/spwaw/section01_spwaw.h"
#include "gamefile/spwaw/section02_spwaw.h"
#include "gamefile/spwaw/section03_spwaw.h"
#include "gamefile/spwaw/section04_spwaw.h"
#include "gamefile/spwaw/section05_spwaw.h"
#include "gamefile/spwaw/section06_spwaw.h"
#include "gamefile/spwaw/section07_spwaw.h"
#include "gamefile/spwaw/section08_spwaw.h"
#include "gamefile/spwaw/section09_spwaw.h"
#include "gamefile/spwaw/section10_spwaw.h"
#include "gamefile/spwaw/section11_spwaw.h"
#include "gamefile/spwaw/section12_spwaw.h"
#include "gamefile/spwaw/section13_spwaw.h"
#include "gamefile/spwaw/section14_spwaw.h"
#include "gamefile/spwaw/section15_spwaw.h"
#include "gamefile/spwaw/section16_spwaw.h"
#include "gamefile/spwaw/section17_spwaw.h"
#include "gamefile/spwaw/section18_spwaw.h"
#include "gamefile/spwaw/section19_spwaw.h"
#include "gamefile/spwaw/section20_spwaw.h"
#include "gamefile/spwaw/section21_spwaw.h"
#include "gamefile/spwaw/section22_spwaw.h"
#include "gamefile/spwaw/section23_spwaw.h"
#include "gamefile/spwaw/section24_spwaw.h"
#include "gamefile/spwaw/section25_spwaw.h"
#include "gamefile/spwaw/section26_spwaw.h"
#include "gamefile/spwaw/section27_spwaw.h"
#include "gamefile/spwaw/section28_spwaw.h"
#include "gamefile/spwaw/section29_spwaw.h"
#include "gamefile/spwaw/section30_spwaw.h"
#include "gamefile/spwaw/section31_spwaw.h"
#include "gamefile/spwaw/section32_spwaw.h"
#include "gamefile/spwaw/section33_spwaw.h"
#include "gamefile/spwaw/section34_spwaw.h"
#include "gamefile/spwaw/section35_spwaw.h"
#include "gamefile/spwaw/section36_spwaw.h"
#include "gamefile/spwaw/section37_spwaw.h"
#include "gamefile/spwaw/section38_spwaw.h"
#include "gamefile/spwaw/section39_spwaw.h"
#include "gamefile/spwaw/section40_spwaw.h"
#include "gamefile/spwaw/section41_spwaw.h"
#include "gamefile/spwaw/section42_spwaw.h"
#include "gamefile/spwaw/section43_spwaw.h"
#include "gamefile/spwaw/section44_spwaw.h"
#include "gamefile/spwaw/section45_spwaw.h"
#include "gamefile/spwaw/section46_spwaw.h"
#include "gamefile/spwaw/section47_spwaw.h"
#include "gamefile/spwaw/section48_spwaw.h"
#include "gamefile/spwaw/section51_spwaw.h"
#pragma pack(pop, r1)

typedef struct s_GAMEDATA_SPWAW {
	SECTION00		sec00;	/* start		*/
	SECTION01		sec01;	/* units		*/
	SECTION02		sec02;	/* map_icon_main	*/
	SECTION03		sec03;	/* map_icon_sec3	*/
	SECTION04		sec04;	/* map_icon_sec4	*/
	SECTION05		sec05;	/* map_icon_sec5	*/
	SECTION06		sec06;	/* map_icon_sec6	*/
	SECTION07		sec07;	/* map_icon_sec7	*/
	SECTION08		sec08;	/* map_terrain		*/
	SECTION09		sec09;	/* map_data0		*/
	SECTION10		sec10;
	SECTION11		sec11;
	SECTION12		sec12;
	SECTION13		sec13;
	SECTION14		sec14;
	SECTION15		sec15;
	SECTION16		sec16;
	SECTION17		sec17;	/* units_pos		*/
	SECTION18		sec18;
	SECTION19		sec19;
	SECTION20		sec20;
	SECTION21		sec21;
	SECTION22		sec22;
	SECTION23		sec23;
	SECTION24		sec24;
	SECTION25		sec25;
	SECTION26		sec26;	/* credits		*/
	SECTION27		sec27;
	SECTION28		sec28;	/* map_data1		*/
	SECTION29		sec29;	/* map_data2		*/
	SECTION30		sec30;
	SECTION31		sec31;
	SECTION32		sec32;
	SECTION33		sec33;
	SECTION34		sec34;	/* leaders		*/
	SECTION35		sec35;	/* formations		*/
	SECTION36		sec36;
	SECTION37		sec37;	/* game			*/
	SECTION38		sec38;	/* map_width		*/
	SECTION39		sec39;	/* map_height		*/
	SECTION40		sec40;
	SECTION41		sec41;
	SECTION42		sec42;
	SECTION43		sec43;
	SECTION44		sec44;
	SECTION45		sec45;
	SECTION46		sec46;
	SECTION47		sec47;
	SECTION48		sec48;	/* rec_header		*/
	SECTION51		sec51;
} GAMEDATA_SPWAW;

extern SECMAP *		gamedata_spwaw_SECMAP		(void);
extern void *		gamedata_spwaw_new_data		(void);
extern void		gamedata_spwaw_free_data	(void **data);

#define	GDSPWAW(src_)	((GAMEDATA_SPWAW *)(src_->data))

#endif	/* GAMEDATA_SPWAW_H */
