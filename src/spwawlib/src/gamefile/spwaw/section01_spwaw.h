/** \file
 * The SPWaW Library - SPWaW gamefile handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION01_SPWAW_H
#define	SECTION01_SPWAW_H	1

#include "gamefile/spwaw/defines_spwaw.h"

typedef struct s_AMMO {
	BYTE	HE;	/* HE ammo	*/
	BYTE	AP;	/* AP ammo	*/
	BYTE	APCR;	/* APCR ammo	*/
	BYTE	HEAT;	/* HEAT ammo	*/
} AMMO;

typedef struct s_ARMOR {
	BYTE	HE;	/* Armor strength against HE ammo	*/
	BYTE	AP;	/* Armor strength against AP ammo	*/
	BYTE	APCR;	/* Armor strength against APCR ammo	*/
	BYTE	HEAT;	/* Armor strength against HEAT ammo	*/
} ARMOR;

typedef struct s_ARMORST {
	ARMOR	HFront;	/* Hull front strength		*/
	ARMOR	HSide1;	/* Hull side1 strength		*/
	ARMOR	HSide2;	/* Hull side2 strength		*/
	ARMOR	HRear;	/* Hull rear strength		*/
	ARMOR	TFront;	/* Turret front strength	*/
	ARMOR	TSide1;	/* Turret side1 strength	*/
	ARMOR	TSide2;	/* Turret side2 strength	*/
	ARMOR	TRear;	/* Turret rear strength		*/
	ARMOR	TTop;	/* Turret top strength		*/
} ARMORST;

typedef struct s_ARMORSL {
	BYTE	HFront;	/* Hull front slope	*/
	BYTE	HSide;	/* Hull side slope	*/
	BYTE	HRear;	/* Hull rear slope	*/
	BYTE	TFront;	/* Turret front slope	*/
	BYTE	TSide;	/* Turret side slope	*/
	BYTE	TRear;	/* Turret rear slope	*/
} ARMORSL;

typedef struct s_UNIT {
	char	name[SPWAW_AZSNAME];	/* Unit name (ASCIIZ)				*/
	BYTE	classID;		/* Unit class ID				*/
	char	__data000[1];
	USHORT	image;			/* Unit image number				*/
	BYTE	formID;			/* Unit formation record ID			*/
	BYTE	minform;		/* Unit minor formation number			*/
	char	__data001[2];
	BYTE	size;			/* Unit size					*/
	char	__data01[1];
	USHORT	wp0_type;		/* Unit slot0 weapon type ID			*/
	USHORT	wp1_type;		/* Unit slot1 weapon type ID			*/
	USHORT	wp2_type;		/* Unit slot2 weapon type ID			*/
	USHORT	wp3_type;		/* Unit slot3 weapon type ID			*/
	char	__data02[4];
	char	__data03[8];
	AMMO	wp0_ammo;		/* Unit slot0 weapon ammunition			*/
	AMMO	wp1_ammo;		/* Unit slot1 weapon ammunition			*/
	AMMO	wp2_ammo;		/* Unit slot2 weapon ammunition			*/
	AMMO	wp3_ammo;		/* Unit slot3 weapon ammunition			*/
	ARMORST	armor;			/* Unit armor strength				*/
	char	__data04[9];
	ARMORSL	slopes;			/* Unit armor slopes				*/
	char	__data05[3];
	BYTE	men;			/* Unit manpower				*/
	BYTE	speed;			/* Unit speed					*/
	BYTE	supp;			/* Unit suppression				*/
	BYTE	exp;			/* Unit experience				*/
	BYTE	morale;			/* Unit morale					*/
	BYTE	status;			/* Unit status (pinned/ready/...)		*/
	USHORT	damage;			/* Unit damage					*/
	USHORT	target;			/* Current target unit number			*/
	char	__data07[1];
	BYTE	range;			/* Unit range setting				*/
	char	__data08[1];
	BYTE	irvision;		/* Unit night vision				*/
	USHORT	SPECIAL1;
	char	__data09[2];
	BYTE	firec;			/* Unit fire control				*/
	char	__data10[1];
	BYTE	cost;			/* Unit cost					*/
	char	__data11[1];
	USHORT	leader;			/* Unit leader number (0xFFFF if abandoned)	*/
	char	__data12[6];
	BYTE	contact;		/* Unit contact status				*/
	BYTE	smoke_ammo;		/* Unit smoke ammunition			*/
	char	__data13[4];
	USHORT	rotation1;		/* Unit rotation (1?) (degrees, 0 = east)	*/
	USHORT	rotation2;		/* Unit rotation (2?) (degrees, 0 = east)	*/
	char	__data140[4];
	USHORT	SPECIAL_OU;
	char	__data141[2];
	char	__data15[3];
	BYTE	OOBid;			/* Unit OOB country ID				*/
	BYTE	OOBnum;			/* Unit OOB number				*/
	char	__data160[1];
	BYTE	UTGidx;			/* Unit type group index			*/
	char	__data161[1];
	USHORT	SPECIAL2;
	USHORT	SPECIAL3;
	USHORT	SPECIAL4;
	USHORT	SPECIAL5;
	USHORT	loader;			/* Unit loader unit number			*/
	BYTE	valid;			/* Unit validity flag				*/
	BYTE	entr;			/* Entrenchment status				*/
	char	__data171[5];
	BYTE	rof;			/* Unit rate of fire				*/
	BYTE	targetting;		/* Unit targetting				*/
	BYTE	rangefinder;		/* Unit rangefinder value			*/
	char	__data18[1];
	BYTE	load_cap;		/* Unit loading capacity			*/
	char	__data19[1];
	BYTE	load_cost;		/* Unit loading cost				*/
	USHORT	crew;			/* Crew unit number if abandoned (0xFFFF if not)*/
	BYTE	swim;			/* Unit water movement points			*/
	char	__data21[7];
	BYTE	survive;		/* Unit survivability				*/
	char	__data22[16];
	BYTE	smoke_dev;		/* Unit smoke device flag			*/
	char	__data23[7];
	BYTE	ori_men;		/* Unit original manpower			*/
	BYTE	stance_x;		/* Unit stance X-coord				*/
	BYTE	stance_y;		/* Unit stance Y-coord				*/
	char	__data24[3];
	BYTE	moves;			/* Unit available speed?			*/
	char	__data25[12];
	BYTE	skirt;			/* Unit armo skirt value			*/
	char	__data26[7];
} UNIT;

typedef struct s_SECTION01 {
	union u_u {
		char	raw[SPWAW_SIZESEC01];
		struct s_d {
			UNIT	units[SPWAW_UNITCOUNT];
		} d;
	} u;
} SECTION01;

#endif	/* SECTION01_SPWAW_H */
