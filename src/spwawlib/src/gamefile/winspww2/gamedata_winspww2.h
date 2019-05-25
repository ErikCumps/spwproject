/** \file
 * The SPWaW Library - game data handling - winSPWW2 game data.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAMEDATA_WINSPWW2_H
#define	GAMEDATA_WINSPWW2_H	1

#include "gamefile/gamedata_secmap.h"

#pragma pack(push, r1, 1)
#include "gamefile/winspww2/section00_winspww2.h"
#include "gamefile/winspww2/section01_winspww2.h"
#include "gamefile/winspww2/section02_winspww2.h"
#include "gamefile/winspww2/section03_winspww2.h"
#include "gamefile/winspww2/section04_winspww2.h"
#include "gamefile/winspww2/section05_winspww2.h"
#include "gamefile/winspww2/section06_winspww2.h"
#include "gamefile/winspww2/section07_winspww2.h"
#include "gamefile/winspww2/section08_winspww2.h"
#include "gamefile/winspww2/section09_winspww2.h"
#include "gamefile/winspww2/section10_winspww2.h"
#include "gamefile/winspww2/section11_winspww2.h"
#include "gamefile/winspww2/section12_winspww2.h"
#include "gamefile/winspww2/section13_winspww2.h"
#include "gamefile/winspww2/section14_winspww2.h"
#include "gamefile/winspww2/section15_winspww2.h"
#include "gamefile/winspww2/section16_winspww2.h"
#include "gamefile/winspww2/section17_winspww2.h"
#include "gamefile/winspww2/section18_winspww2.h"
#include "gamefile/winspww2/section19_winspww2.h"
#include "gamefile/winspww2/section20_winspww2.h"
#include "gamefile/winspww2/section21_winspww2.h"
#include "gamefile/winspww2/section22_winspww2.h"
#include "gamefile/winspww2/section23_winspww2.h"
#include "gamefile/winspww2/section24_winspww2.h"
#include "gamefile/winspww2/section25_winspww2.h"
#include "gamefile/winspww2/section26_winspww2.h"
#include "gamefile/winspww2/section27_winspww2.h"
#include "gamefile/winspww2/section28_winspww2.h"
#include "gamefile/winspww2/section29_winspww2.h"
#include "gamefile/winspww2/section30_winspww2.h"
#include "gamefile/winspww2/section31_winspww2.h"
#include "gamefile/winspww2/section32_winspww2.h"
#include "gamefile/winspww2/section33_winspww2.h"
#include "gamefile/winspww2/section34_winspww2.h"
#include "gamefile/winspww2/section35_winspww2.h"
#include "gamefile/winspww2/section36_winspww2.h"
#include "gamefile/winspww2/section37_winspww2.h"
#include "gamefile/winspww2/section38_winspww2.h"
#include "gamefile/winspww2/section39_winspww2.h"
#include "gamefile/winspww2/section40_winspww2.h"
#include "gamefile/winspww2/section41_winspww2.h"
#include "gamefile/winspww2/section42_winspww2.h"
#include "gamefile/winspww2/section43_winspww2.h"
#include "gamefile/winspww2/section48_winspww2.h"
#include "gamefile/winspww2/section51_winspww2.h"
#include "gamefile/winspww2/section52_winspww2.h"
#include "gamefile/winspww2/section53_winspww2.h"
#include "gamefile/winspww2/section54_winspww2.h"
#include "gamefile/winspww2/section56_winspww2.h"
#include "gamefile/winspww2/section57_winspww2.h"
#include "gamefile/winspww2/section59_winspww2.h"
#pragma pack(pop, r1)

typedef struct s_GAMEDATA_WINSPWW2 {
	WINSPWW2_SECTION00	sec00;	/* start		*/
	WINSPWW2_SECTION01	sec01;	/* units		*/
	WINSPWW2_SECTION02	sec02;	/* map_icon_main	*/
	WINSPWW2_SECTION03	sec03;	/* map_icon_sec3	*/
	WINSPWW2_SECTION04	sec04;	/* map_icon_sec4	*/
	WINSPWW2_SECTION05	sec05;	/* map_icon_sec5	*/
	WINSPWW2_SECTION06	sec06;	/* map_icon_sec6	*/
	WINSPWW2_SECTION07	sec07;	/* map_icon_sec7	*/
	WINSPWW2_SECTION08	sec08;	/* map_terrain		*/
	WINSPWW2_SECTION09	sec09;	/* map_data0		*/
	WINSPWW2_SECTION10	sec10;
	WINSPWW2_SECTION11	sec11;
	WINSPWW2_SECTION12	sec12;
	WINSPWW2_SECTION13	sec13;
	WINSPWW2_SECTION14	sec14;
	WINSPWW2_SECTION15	sec15;
	WINSPWW2_SECTION16	sec16;
	WINSPWW2_SECTION17	sec17;	/* units_pos		*/
	WINSPWW2_SECTION18	sec18;
	WINSPWW2_SECTION19	sec19;
	WINSPWW2_SECTION20	sec20;
	WINSPWW2_SECTION21	sec21;
	WINSPWW2_SECTION22	sec22;
	WINSPWW2_SECTION23	sec23;
	WINSPWW2_SECTION24	sec24;
	WINSPWW2_SECTION25	sec25;
	WINSPWW2_SECTION26	sec26;	/* credits		*/
	WINSPWW2_SECTION27	sec27;
	WINSPWW2_SECTION28	sec28;	/* map_data1		*/
	WINSPWW2_SECTION29	sec29;	/* map_data2		*/
	WINSPWW2_SECTION30	sec30;
	WINSPWW2_SECTION31	sec31;
	WINSPWW2_SECTION32	sec32;
	WINSPWW2_SECTION33	sec33;
	WINSPWW2_SECTION34	sec34;	/* leaders		*/
	WINSPWW2_SECTION35	sec35;	/* formations		*/
	WINSPWW2_SECTION36	sec36;
	WINSPWW2_SECTION37	sec37;	/* game			*/
	WINSPWW2_SECTION38	sec38;	/* map_width		*/
	WINSPWW2_SECTION39	sec39;	/* map_height		*/
	WINSPWW2_SECTION40	sec40;
	WINSPWW2_SECTION41	sec41;
	WINSPWW2_SECTION42	sec42;
	WINSPWW2_SECTION43	sec43;
	WINSPWW2_SECTION48	sec48;	/* rec_header		*/
	WINSPWW2_SECTION51	sec51;
	WINSPWW2_SECTION52	sec52;
	WINSPWW2_SECTION53	sec53;
	WINSPWW2_SECTION54	sec54;
	WINSPWW2_SECTION56	sec56;
	WINSPWW2_SECTION57	sec57;
	WINSPWW2_SECTION59	sec59;
} GAMEDATA_WINSPWW2;

extern SECMAP *		gamedata_winspww2_SECMAP		(void);
extern void *		gamedata_winspww2_new_data		(void);
extern void		gamedata_winspww2_free_data	(void **data);

#define	GDWINSPWW2(src_)	((GAMEDATA_WINSPWW2 *)(src_->data))

#endif	/* GAMEDATA_WINSPWW2_H */
