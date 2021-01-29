/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION01_WINSPMBT_H
#define	SECTION01_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

typedef struct s_WINSPMBT_AMMO {
	BYTE			HE;			/* HE ammo					*/
	BYTE			AP;			/* AP ammo					*/
	BYTE			APCR;			/* APCR ammo					*/
	BYTE			HEAT;			/* HEAT ammo					*/
} WINSPMBT_AMMO;

typedef struct s_WINSPMBT_ARMOR {
	BYTE			HE;			/* Armor strength against HE ammo		*/
	BYTE			AP;			/* Armor strength against AP ammo		*/
	BYTE			APCR;			/* Armor strength against APCR ammo		*/
	BYTE			HEAT;			/* Armor strength against HEAT ammo		*/
} WINSPMBT_ARMOR;

typedef struct s_WINSPMBT_ARMORST {
	WINSPMBT_ARMOR		HFront;			/* Hull front strength				*/
	WINSPMBT_ARMOR		HSide1;			/* Hull side1 strength				*/
	WINSPMBT_ARMOR		HSide2;			/* Hull side2 strength				*/
	WINSPMBT_ARMOR		HRear;			/* Hull rear strength				*/
	WINSPMBT_ARMOR		TFront;			/* Turret front strength			*/
	WINSPMBT_ARMOR		TSide1;			/* Turret side1 strength			*/
	WINSPMBT_ARMOR		TSide2;			/* Turret side2 strength			*/
	WINSPMBT_ARMOR		TRear;			/* Turret rear strength				*/
	WINSPMBT_ARMOR		TTop;			/* Turret top strength				*/
} WINSPMBT_ARMORST;

typedef struct s_WINSPMBT_ARMORSL {
	BYTE			HFront;			/* Hull front slope				*/
	BYTE			HSide;			/* Hull side slope				*/
	BYTE			HRear;			/* Hull rear slope				*/
	BYTE			TFront;			/* Turret front slope				*/
	BYTE			TSide;			/* Turret side slope				*/
	BYTE			TRear;			/* Turret rear slope				*/
} WINSPMBT_ARMORSL;

typedef struct s_WINSPMBT_UNIT {
	char			name[WINSPMBT_AZSNAME];	/* Unit name (ASCIIZ)				*/
	BYTE			classID;		/* Unit class ID				*/
	char			__data00[1];
	USHORT			icon;			/* Unit icon number				*/
	USHORT			formID;			/* Unit formation record ID			*/
	BYTE			minform;		/* Unit minor formation number			*/
	char			__data01[2];
	BYTE			size;			/* Unit size					*/
	USHORT			wp0_type;		/* Unit slot0 weapon type ID			*/
	USHORT			wp1_type;		/* Unit slot1 weapon type ID			*/
	USHORT			wp2_type;		/* Unit slot2 weapon type ID			*/
	USHORT			wp3_type;		/* Unit slot3 weapon type ID			*/
	char			__data02[12];
	WINSPMBT_AMMO		wp0_ammo;		/* Unit slot0 weapon ammunition			*/
	WINSPMBT_AMMO		wp1_ammo;		/* Unit slot1 weapon ammunition			*/
	WINSPMBT_AMMO		wp2_ammo;		/* Unit slot2 weapon ammunition			*/
	WINSPMBT_AMMO		wp3_ammo;		/* Unit slot3 weapon ammunition			*/
	WINSPMBT_ARMORST	armor;			/* Unit armor strength				*/
	char			__data03[9];
	WINSPMBT_ARMORSL	slopes;			/* Unit armor slopes				*/
	char			__data04[3];
	BYTE			men;			/* Unit manpower				*/
	BYTE			speed;			/* Unit speed					*/
	BYTE			supp;			/* Unit suppression				*/
	BYTE			exp;			/* Unit experience				*/
	BYTE			morale;			/* Unit morale					*/
	BYTE			status;			/* Unit status (pinned/ready/...)		*/
	USHORT			damage;			/* Unit damage					*/
	USHORT			target;			/* Current target unit number			*/
	char			__data05[1];
	BYTE			range;			/* Unit range setting				*/
	BYTE			irvision;		/* Unit night vision				*/
	USHORT			SPECIAL1;
	char			__data06[2];
	BYTE			firec;			/* Unit fire control				*/
	USHORT			cost;			/* Unit cost					*/
	USHORT			leader;			/* Unit leader number (0xFFFF if abandoned)	*/
	char			__data08[5];
	BYTE			contact;		/* Unit contact status				*/
	char			__data09[4];
	USHORT			rotation1;		/* Unit rotation (1?) (degrees, 0 = east)	*/
	USHORT			rotation2;		/* Unit rotation (2?) (degrees, 0 = east)	*/
	char			__data10[3];
	USHORT			SPECIAL_OU;
	char			__data11[5];
	BYTE			OOBid;			/* Unit OOB country ID				*/
	USHORT			OOBnum;			/* Unit OOB record ID				*/
	BYTE			UTGidx;			/* Unit type group index?			*/
	char			__data12[1];
	USHORT			SPECIAL2;
	USHORT			SPECIAL3;
	USHORT			SPECIAL4;
	USHORT			SPECIAL5;
	USHORT			loader;			/* Unit loader unit number			*/
	BYTE			valid;			/* Unit validity flag				*/
	BYTE			entr;			/* Entrenchment status				*/
	char			__data14[4];
	BYTE			rof;			/* Unit rate of fire				*/
	BYTE			targetting;		/* Unit targetting?				*/
	BYTE			rangefinder;		/* Unit rangefinder value			*/
	BYTE			ew;			/* Unit Electronic Warfare value		*/
	BYTE			load_cap;		/* Unit loading capacity			*/
	char			__data16[1];
	BYTE			load_cost;		/* Unit loading cost				*/
	USHORT			crew;			/* Crew unit number if abandoned (0xFFFF if not)*/
	BYTE			swim;			/* Unit water movement points			*/
	char			__data17[2];
	BYTE			survive;		/* Unit survivability				*/
	char			__data18[12];
	BYTE			wp0_oob;		/* Unit slot0 weapon OOB record ID		*/
	BYTE			wp1_oob;		/* Unit slot1 weapon OOB record ID		*/
	BYTE			wp2_oob;		/* Unit slot2 weapon OOB record ID		*/
	BYTE			wp3_oob;		/* Unit slot3 weapon OOB record ID		*/
	BYTE			smoke_dev;		/* Unit smoke device flag			*/
	char			__data19[35];
	USHORT			icon_winter;		/* Unit winter icon number			*/
	USHORT			icon_desert;		/* Unit desert icon number			*/
	char			__data20[9];
} WINSPMBT_UNIT;

typedef struct s_WINSPMBT_SECTION01 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC01];
		struct s_d {
			WINSPMBT_UNIT	units[WINSPMBT_UNITCOUNT];
		} d;
	} u;
} WINSPMBT_SECTION01;

#endif	/* SECTION01_WINSPMBT_H */
