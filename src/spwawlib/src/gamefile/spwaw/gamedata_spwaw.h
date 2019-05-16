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
	SPWAW_SECTION00		sec00;	/* start		*/
	SPWAW_SECTION01		sec01;	/* units		*/
	SPWAW_SECTION02		sec02;	/* map_icon_main	*/
	SPWAW_SECTION03		sec03;	/* map_icon_sec3	*/
	SPWAW_SECTION04		sec04;	/* map_icon_sec4	*/
	SPWAW_SECTION05		sec05;	/* map_icon_sec5	*/
	SPWAW_SECTION06		sec06;	/* map_icon_sec6	*/
	SPWAW_SECTION07		sec07;	/* map_icon_sec7	*/
	SPWAW_SECTION08		sec08;	/* map_terrain		*/
	SPWAW_SECTION09		sec09;	/* map_data0		*/
	SPWAW_SECTION10		sec10;
	SPWAW_SECTION11		sec11;
	SPWAW_SECTION12		sec12;
	SPWAW_SECTION13		sec13;
	SPWAW_SECTION14		sec14;
	SPWAW_SECTION15		sec15;
	SPWAW_SECTION16		sec16;
	SPWAW_SECTION17		sec17;	/* units_pos		*/
	SPWAW_SECTION18		sec18;
	SPWAW_SECTION19		sec19;
	SPWAW_SECTION20		sec20;
	SPWAW_SECTION21		sec21;
	SPWAW_SECTION22		sec22;
	SPWAW_SECTION23		sec23;
	SPWAW_SECTION24		sec24;
	SPWAW_SECTION25		sec25;
	SPWAW_SECTION26		sec26;	/* credits		*/
	SPWAW_SECTION27		sec27;
	SPWAW_SECTION28		sec28;	/* map_data1		*/
	SPWAW_SECTION29		sec29;	/* map_data2		*/
	SPWAW_SECTION30		sec30;
	SPWAW_SECTION31		sec31;
	SPWAW_SECTION32		sec32;
	SPWAW_SECTION33		sec33;
	SPWAW_SECTION34		sec34;	/* leaders		*/
	SPWAW_SECTION35		sec35;	/* formations		*/
	SPWAW_SECTION36		sec36;
	SPWAW_SECTION37		sec37;	/* game			*/
	SPWAW_SECTION38		sec38;	/* map_width		*/
	SPWAW_SECTION39		sec39;	/* map_height		*/
	SPWAW_SECTION40		sec40;
	SPWAW_SECTION41		sec41;
	SPWAW_SECTION42		sec42;
	SPWAW_SECTION43		sec43;
	SPWAW_SECTION44		sec44;
	SPWAW_SECTION45		sec45;
	SPWAW_SECTION46		sec46;
	SPWAW_SECTION47		sec47;
	SPWAW_SECTION48		sec48;	/* rec_header		*/
	SPWAW_SECTION51		sec51;
} GAMEDATA_SPWAW;

extern SECMAP *		gamedata_spwaw_SECMAP		(void);
extern void *		gamedata_spwaw_new_data		(void);
extern void		gamedata_spwaw_free_data	(void **data);

#define	GDSPWAW(src_)	((GAMEDATA_SPWAW *)(src_->data))

#endif	/* GAMEDATA_SPWAW_H */
