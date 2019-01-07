/** \file
 * The SPWaW Library - savegame type definitions.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_TYPES_H
#define	INTERNAL_TYPES_H	1

#define	T_DESERT	0x00
#define	T_SUMMER	0x01
#define	T_WINTER	0x02
#define	T_JUNGLE	0x03
#define	T_ROUGH		0x05

#define	W_BEST		0x01
#define	W_GOOD		0x02
#define	W_MEDIUM	0x03
#define	W_BAD		0x04
#define	W_VERYBAD	0x05
#define	W_WORST		0x06

#define	M_ASSAULT	0x00
#define	M_ADVANCE	0x01
#define	M_DELAY		0x02
#define	M_DEFEND	0x03

#define	F_REG		0x00
#define	F_CORE		0x01
#define	F_AUX		0x02

#define	U_READY		0x00
#define	U_PINNED	0x01
#define	U_RETREATING	0x02
#define	U_ROUTED	0x03
#define	U_ABANDONED	0x04
#define	U_DESTROYED	0x05
#define	U_BURNING	0x06
#define	U_BUTTONED	0x07
#define	U_IMMOBILIZED	0x08
#define	U_PASSENGER	0x09

#define	C_NONE		0x00
#define	C_RADIO		0x01
#define	C_DIRECT	0x02

#define	E_NONE		0x00
#define	E_COVER		0x01
#define	E_DUGIN		0x02

#define	R_KIA		0x00
#define	R_PVT		0x01
#define	R_CPL		0x02
#define	R_SGT		0x03
#define	R_2LT		0x04
#define	R_1LT		0x05
#define	R_CPT		0x06
#define	R_MAJ		0x07
#define	R_LTC		0x08
#define	R_COL		0x09
#define	R_GEN		0x0A

#define	VH_PLAYER1	0x00
#define	VH_PLAYER2	0x01
#define	VH_NEUTRAL	0x02

#define	BR_DD		-2
#define	BR_MD		-1
#define	BR_DB		0
#define	BR_MV		1
#define	BR_DV		2
#define	BR_UNKNOWN	42

#define	TFS1_WATER	0x08
#define	TFS2_WATER	0x0C
#define	TFS3_WATER	0x20
#define	TFS4_WATER	0x00

#define	TFS1_BRIDGE	0x00
#define	TFS2_BRIDGE	0x03
#define	TFS3_BRIDGE	0x00
#define	TFS4_BRIDGE	0x00

#define	TFS1_ROAD	0xC0
#define	TFS2_ROAD	0x00
#define	TFS3_ROAD	0x01
#define	TFS4_ROAD	0x02

#define	UT_UNIT		0x01
#define	UT_CREW		0x02
#define	UT_SPAU		0x03
#define	UT_UNKNOWN	0xFF

#define	AS_STAY		0x01
#define	AS_LEFT		0x02
#define	AS_NONE		0xFF

#endif	/* INTERNAL_TYPES_H */
