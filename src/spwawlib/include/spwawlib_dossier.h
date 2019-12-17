/** \file
 * The SPWaW Library - dossier API.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 *
 * \addtogroup API
 * @{
 */

#ifndef	SPWAW_LIB_DOSSIER_H
#define	SPWAW_LIB_DOSSIER_H	1

#include <spwawlib_api.h>
#include <spwawlib_defines.h>
#include <spwawlib_snapshot.h>
#include <spwawlib_savelist.h>
#include <spwawlib_snaplist.h>
#include <spwawlib_spwoob_list.h>
#include <spwawlib_uht.h>

/* Forward declarations for convenience */

typedef struct s_SPWAW_DOSSIER_UIR	SPWAW_DOSSIER_UIR;
typedef struct s_SPWAW_BTURN		SPWAW_BTURN;
typedef struct s_SPWAW_BATTLE		SPWAW_BATTLE;
typedef struct s_SPWAW_DOSSIER		SPWAW_DOSSIER;

/* SPWAW dossier: formation info record */
typedef struct s_SPWAW_DOSSIER_FIR {
	USHORT			leader;				/* Formation leader unit index			*/
	SPWAW_SNAP_OOB_FEL	*snap;				/* Formation original snapshot data		*/
} SPWAW_DOSSIER_FIR;

/* SPWAW dossier: unit info record */
struct s_SPWAW_DOSSIER_UIR {
	SPWAW_SNAP_OOB_UEL	*snap;				/* Unit original snapshot data			*/
	SPWAW_DOSSIER_FIR	*fptr;				/* Unit parent formation info record pointer	*/
};

/* SPWAW dossier: battle info record set
 *
 * The info records are a superset of the core info records.
 */
typedef struct s_SPWAW_DOSSIER_BIR {
	USHORT			fcnt;				/* Formation info records count			*/
	SPWAW_DOSSIER_FIR	*fir;				/* Pointer to formation info records		*/
	USHORT			ucnt;				/* Unit info records count			*/
	SPWAW_DOSSIER_UIR	*uir;				/* Pointer to unit info records			*/
} SPWAW_DOSSIER_BIR;

/* SPWAW dossier: battle info record set collection
 *
 * The info records are a superset of the core info records.
 */
typedef struct s_SPWAW_DOSSIER_BIRS {
	SPWAW_DOSSIER_BIR	pbir_core;			/* Player info record set (core force)		*/
	SPWAW_DOSSIER_BIR	pbir_support;			/* Player info record set (support force)	*/
	SPWAW_DOSSIER_BIR	pbir_battle;			/* Player info record set (battle force)	*/
	SPWAW_DOSSIER_BIR	obir_battle;			/* Opponent info record set (battle force)	*/
} SPWAW_DOSSIER_BIRS;

/* SPWAW dossier: battle turn data */
struct s_SPWAW_BTURN {
	SPWAW_BTURN		*prev;				/* Pointer to previous battle turn		*/
	SPWAW_BTURN		*next;				/* Pointer to next battle turn			*/
	SPWAW_SNAPSHOT		*snap;				/* Battle turn snapshot data			*/
	SPWAW_TURN_DATE		tdate;				/* Battle turn date				*/
	SPWAW_DOSSIER_BIRS	info;				/* Battle info record set			*/
	SPWAW_BATTLE		*battle;			/* Pointer to parent battle data struct		*/
};

/* SPWAW dossier: battle unit reassignment record */
typedef struct s_SPWAW_DOSSIER_BURA {
	USHORT			src;				/* Unit reassignment records: src UID		*/
	USHORT			dst;				/* Unit reassignment records: dst UID		*/
	bool			rpl;				/* Unit replacement flag			*/
} SPWAW_DOSSIER_BURA;

/* SPWAW dossier: battle properties */
typedef struct s_SPWAW_DOSSIER_BTLPROPS {
	USHORT			pc_fcnt;			/* Player core force formations count		*/
	USHORT			pc_ucnt;			/* Player core force units count		*/
	USHORT			ps_fcnt;			/* Player support force formations count	*/
	USHORT			ps_ucnt;			/* Player support force units count		*/
	USHORT			pb_fcnt;			/* Player battle force formations count		*/
	USHORT			pb_ucnt;			/* Player battle force units count		*/
	USHORT			ob_fcnt;			/* Player battle force formations count		*/
	USHORT			ob_ucnt;			/* Player battle force units count		*/
} SPWAW_DOSSIER_BTLPROPS;

/* SPWAW dossier: battle data */
struct s_SPWAW_BATTLE {
	char			*name;				/* Optional battle name				*/
	SPWAW_BATTLE		*prev;				/* Pointer to previous battle			*/
	SPWAW_BATTLE		*next;				/* Pointer to next battle			*/
	SPWAW_SNAPSHOT		*snap;				/* Battle turn #0 snapshot data			*/
	SPWAW_BATTLE_DATE	bdate;				/* Battle date					*/
	char			*location;			/* Battle location				*/
	SPWOOB			*oobdat;			/* Battle OOB data				*/
	BYTE			OOB_p1;				/* Player OOB ID				*/
	BYTE			OOB_p2;				/* Opponent OOB ID				*/
	char			*miss_p1;			/* Player mission				*/
	char			*miss_p2;			/* Opponent mission				*/
	bool			meeting;			/* Meeting engagement flag			*/
	SPWAW_BTURN		**tlist;			/* Battle turn list				*/
	USHORT			tcnt;				/* Battle turn list element count		*/
	USHORT			tlen;				/* Battle turn list length			*/
	SPWAW_BTURN		*tfirst;			/* Pointer to first battle turn			*/
	SPWAW_BTURN		*tlast;				/* Pointer to last battle turn			*/
	SPWAW_DOSSIER_BIRS	*info_sob;			/* Battle info record sets at start of battle	*/
	SPWAW_DOSSIER_BIRS	*info_eob;			/* Battle info record sets at end of battle	*/
	USHORT			racnt;				/* Unit reassignment records count		*/
	SPWAW_DOSSIER_BURA	*ra;				/* Unit reassignment records			*/
	SPWAW_DOSSIER		*dossier;			/* Pointer to parent dossier data struct	*/
	SPWAW_DOSSIER_BTLPROPS	props;				/* Battle properties				*/
};

/* SPWAW dossier: campaign properties */
typedef struct s_SPWAW_DOSSIER_CMPPROPS {
	BYTE			OOB;				/* Player OOB ID				*/
	SPWAW_DATE		start;				/* Campaign start date				*/
	SPWAW_DATE		end;				/* Campaign end date				*/
	USHORT			maxbtlcnt;			/* Campaign maximum battles count		*/
	USHORT			ifcnt;				/* Player initial core formations count		*/
	USHORT			iucnt;				/* Player initial core units count		*/
	USHORT			cfcnt;				/* Player current core formations count		*/
	USHORT			cucnt;				/* Player current core units count		*/
} SPWAW_DOSSIER_CMPPROPS;

/* SPWAW dossier: campaign stats */
typedef struct s_SPWAW_DOSSIER_CMPSTATS {
	USHORT	concluded;					/* Number of concluded battles in this campaign	*/
	USHORT	results[SPWAW_BRLASTCODE+1];			/* Counts for each battle result		*/
} SPWAW_DOSSIER_CMPSTATS;

/* SPWAW dossier: data */
struct s_SPWAW_DOSSIER {
	SPWAW_GAME_TYPE		gametype;			/* Dossier game type				*/
	char			*name;				/* Dossier name					*/
	char			*comment;			/* Dossier comment				*/
	SPWAW_DOSSIER_TYPE	type;				/* Dossier type					*/
	char			*oobdir;			/* Original OOB data directory			*/
	SPWAW_SPWOOB_LIST	*oobdata;			/* OOB data list				*/
	SPWAW_BATTLE		**blist;			/* Battle list					*/
	USHORT			bcnt;				/* Battle list element count			*/
	USHORT			blen;				/* Battle list length				*/
	SPWAW_BATTLE		*bfirst;			/* Pointer to first battle			*/
	SPWAW_BATTLE		*blast;				/* Pointer to last battle			*/
	void			*stab;				/* \internal string table			*/
	SPWAW_DOSSIER_CMPPROPS	props;				/* Campaign properties				*/
	SPWAW_DOSSIER_CMPSTATS	stats;				/* Campaign stats				*/
	SPWAW_UHT		uht;				/* Unit history tracking			*/
};

/* SPWAW dossier: file info */
typedef struct s_SPWAW_DOSSIER_INFO {
	SPWAW_GAME_TYPE		gametype;			/* Dossier game type				*/
	char			name[SPWAW_AZSDNAME+1];		/* Dossier name					*/
	char			comment[SPWAW_AZSDCMT+1];	/* Dossier comment				*/
	SPWAW_DOSSIER_TYPE	type;				/* Dossier type					*/
	USHORT			btlcnt;				/* Dossier battle count				*/
	SPWAW_DOSSIER_CMPPROPS	props;				/* Campaign properties				*/
} SPWAW_DOSSIER_INFO;

/*** API ***/

extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_new		(SPWAW_GAME_TYPE gametype, const char *name, const char *comment, SPWAW_DOSSIER **dossier);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_info		(const char *file, SPWAW_DOSSIER_INFO *info);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_load		(const char *file, SPWAW_DOSSIER **dossier);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_save		(SPWAW_DOSSIER **dossier, const char *file, bool compress);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_free		(SPWAW_DOSSIER **dossier);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_edit		(SPWAW_DOSSIER *dossier, const char *name, const char *comment);

extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_add_campaign_snap	(SPWAW_DOSSIER *dossier, SPWAW_SNAPSHOT *snap, SPWAW_BTURN **bturn);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_add_battle	(SPWAW_DOSSIER *dossier, SPWAW_SNAPSHOT *snap, const char *name, SPWAW_BATTLE **battle);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_add_battle_snap	(SPWAW_BATTLE *battle, SPWAW_SNAPSHOT *snap, SPWAW_BTURN **bturn);

extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_del		(SPWAW_DOSSIER *dossier, SPWAW_BTURN *bturn);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_del		(SPWAW_DOSSIER *dossier, SPWAW_BATTLE *battle);

extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_savelist		(SPWAW_DOSSIER *dossier, SPWAW_SAVELIST **list);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_snaplist		(SPWAW_DOSSIER *dossier, SPWAW_SNAPLIST **list);

#endif	/* SPWAW_LIB_DOSSIER_H */

/** @} */
