/** \file
 * The SPWaW Library - game data handling - winSPMBT game data.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GAMEDATA_WINSPMBT_H
#define	GAMEDATA_WINSPMBT_H	1

#include "gamefile/gamedata_secmap.h"

#pragma pack(push, r1, 1)
#include "gamefile/winspmbt/section00_winspmbt.h"
#include "gamefile/winspmbt/section01_winspmbt.h"
#include "gamefile/winspmbt/section02_winspmbt.h"
#include "gamefile/winspmbt/section03_winspmbt.h"
#include "gamefile/winspmbt/section04_winspmbt.h"
#include "gamefile/winspmbt/section05_winspmbt.h"
#include "gamefile/winspmbt/section06_winspmbt.h"
#include "gamefile/winspmbt/section07_winspmbt.h"
#include "gamefile/winspmbt/section08_winspmbt.h"
#include "gamefile/winspmbt/section09_winspmbt.h"
#include "gamefile/winspmbt/section10_winspmbt.h"
#include "gamefile/winspmbt/section11_winspmbt.h"
#include "gamefile/winspmbt/section12_winspmbt.h"
#include "gamefile/winspmbt/section13_winspmbt.h"
#include "gamefile/winspmbt/section14_winspmbt.h"
#include "gamefile/winspmbt/section15_winspmbt.h"
#include "gamefile/winspmbt/section16_winspmbt.h"
#include "gamefile/winspmbt/section17_winspmbt.h"
#include "gamefile/winspmbt/section18_winspmbt.h"
#include "gamefile/winspmbt/section19_winspmbt.h"
#include "gamefile/winspmbt/section20_winspmbt.h"
#include "gamefile/winspmbt/section21_winspmbt.h"
#include "gamefile/winspmbt/section22_winspmbt.h"
#include "gamefile/winspmbt/section23_winspmbt.h"
#include "gamefile/winspmbt/section24_winspmbt.h"
#include "gamefile/winspmbt/section25_winspmbt.h"
#include "gamefile/winspmbt/section26_winspmbt.h"
#include "gamefile/winspmbt/section27_winspmbt.h"
#include "gamefile/winspmbt/section28_winspmbt.h"
#include "gamefile/winspmbt/section29_winspmbt.h"
#include "gamefile/winspmbt/section30_winspmbt.h"
#include "gamefile/winspmbt/section31_winspmbt.h"
#include "gamefile/winspmbt/section32_winspmbt.h"
#include "gamefile/winspmbt/section33_winspmbt.h"
#include "gamefile/winspmbt/section34_winspmbt.h"
#include "gamefile/winspmbt/section35_winspmbt.h"
#include "gamefile/winspmbt/section36_winspmbt.h"
#include "gamefile/winspmbt/section37_winspmbt.h"
#include "gamefile/winspmbt/section38_winspmbt.h"
#include "gamefile/winspmbt/section39_winspmbt.h"
#include "gamefile/winspmbt/section40_winspmbt.h"
#include "gamefile/winspmbt/section41_winspmbt.h"
#include "gamefile/winspmbt/section42_winspmbt.h"
#include "gamefile/winspmbt/section43_winspmbt.h"
#include "gamefile/winspmbt/section48_winspmbt.h"
#include "gamefile/winspmbt/section51_winspmbt.h"
#include "gamefile/winspmbt/section52_winspmbt.h"
#include "gamefile/winspmbt/section53_winspmbt.h"
#include "gamefile/winspmbt/section54_winspmbt.h"
#include "gamefile/winspmbt/section56_winspmbt.h"
#include "gamefile/winspmbt/section57_winspmbt.h"
#include "gamefile/winspmbt/section59_winspmbt.h"
#pragma pack(pop, r1)

typedef struct s_GAMEDATA_WINSPMBT {
	WINSPMBT_SECTION00	sec00;	/* start		*/
	WINSPMBT_SECTION01	sec01;	/* units		*/
	WINSPMBT_SECTION02	sec02;	/* map_icon_main	*/
	WINSPMBT_SECTION03	sec03;	/* map_icon_sec3	*/
	WINSPMBT_SECTION04	sec04;	/* map_icon_sec4	*/
	WINSPMBT_SECTION05	sec05;	/* map_icon_sec5	*/
	WINSPMBT_SECTION06	sec06;	/* map_icon_sec6	*/
	WINSPMBT_SECTION07	sec07;	/* map_icon_sec7	*/
	WINSPMBT_SECTION08	sec08;	/* map_terrain		*/
	WINSPMBT_SECTION09	sec09;	/* map_data0		*/
	WINSPMBT_SECTION10	sec10;
	WINSPMBT_SECTION11	sec11;
	WINSPMBT_SECTION12	sec12;
	WINSPMBT_SECTION13	sec13;
	WINSPMBT_SECTION14	sec14;
	WINSPMBT_SECTION15	sec15;
	WINSPMBT_SECTION16	sec16;
	WINSPMBT_SECTION17	sec17;	/* units_pos		*/
	WINSPMBT_SECTION18	sec18;
	WINSPMBT_SECTION19	sec19;
	WINSPMBT_SECTION20	sec20;
	WINSPMBT_SECTION21	sec21;
	WINSPMBT_SECTION22	sec22;
	WINSPMBT_SECTION23	sec23;
	WINSPMBT_SECTION24	sec24;
	WINSPMBT_SECTION25	sec25;
	WINSPMBT_SECTION26	sec26;	/* credits		*/
	WINSPMBT_SECTION27	sec27;
	WINSPMBT_SECTION28	sec28;	/* map_data1		*/
	WINSPMBT_SECTION29	sec29;	/* map_data2		*/
	WINSPMBT_SECTION30	sec30;
	WINSPMBT_SECTION31	sec31;
	WINSPMBT_SECTION32	sec32;
	WINSPMBT_SECTION33	sec33;
	WINSPMBT_SECTION34	sec34;	/* leaders		*/
	WINSPMBT_SECTION35	sec35;	/* formations		*/
	WINSPMBT_SECTION36	sec36;
	WINSPMBT_SECTION37	sec37;	/* game			*/
	WINSPMBT_SECTION38	sec38;	/* map_width		*/
	WINSPMBT_SECTION39	sec39;	/* map_height		*/
	WINSPMBT_SECTION40	sec40;
	WINSPMBT_SECTION41	sec41;
	WINSPMBT_SECTION42	sec42;
	WINSPMBT_SECTION43	sec43;
	WINSPMBT_SECTION48	sec48;	/* rec_header		*/
	WINSPMBT_SECTION51	sec51;
	WINSPMBT_SECTION52	sec52;
	WINSPMBT_SECTION53	sec53;
	WINSPMBT_SECTION54	sec54;
	WINSPMBT_SECTION56	sec56;
	WINSPMBT_SECTION57	sec57;
	WINSPMBT_SECTION59	sec59;
} GAMEDATA_WINSPMBT;

extern SECMAP *		gamedata_winspmbt_SECMAP		(void);
extern void *		gamedata_winspmbt_new_data		(void);
extern void		gamedata_winspmbt_free_data	(void **data);

#define	GDWINSPMBT(src_)	((GAMEDATA_WINSPMBT *)(src_->data))

#endif	/* GAMEDATA_WINSPMBT_H */
