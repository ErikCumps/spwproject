/** \file
 * The SPWaW Library - dossier API.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SPWAW_LIB_DOSSIER_H
#define	SPWAW_LIB_DOSSIER_H	1

#include <spwawlib_api.h>
#include <spwawlib_defines.h>
#include <spwawlib_snapshot.h>
#include <spwawlib_savelist.h>
#include <spwawlib_snaplist.h>

/*******************************/
/***   MAIN DATA STRUCTURE   ***/
/*******************************/

typedef struct s_SPWAW_DOSSIER_UIR	SPWAW_DOSSIER_UIR;
typedef struct s_SPWAW_BTURN		SPWAW_BTURN;
typedef struct s_SPWAW_BATTLE		SPWAW_BATTLE;
typedef struct s_SPWAW_DOSSIER		SPWAW_DOSSIER;

/* SPWAW dossier: formation info record */
typedef struct s_SPWAW_DOSSIER_FIR {
	USHORT			leader;			/* Formation leader unit index			*/
	SPWAW_SNAP_OOB_FEL	*snap;			/* Formation original snapshot data		*/
} SPWAW_DOSSIER_FIR;

/* SPWAW dossier: unit info record */
struct s_SPWAW_DOSSIER_UIR {
	SPWAW_SNAP_OOB_UEL	*snap;			/* Unit original snapshot data			*/
	SPWAW_DOSSIER_FIR	*fptr;			/* Unit parent formation info record pointer	*/
	SPWAW_DOSSIER_UIR	*prev;			/* Pointer to previous battle UIR		*/
	SPWAW_DOSSIER_UIR	*next;			/* Pointer to next battle UIR			*/
};

/* SPWAW dossier: battle info record set
 *
 * The info records are a superset of the core info records.
 */
typedef struct s_SPWAW_DOSSIER_BIR {
	USHORT			fcnt;			/* Formation info records count		*/
	SPWAW_DOSSIER_FIR	*fir;			/* Pointer to formation info records	*/
	USHORT			ucnt;			/* Unit info records count		*/
	SPWAW_DOSSIER_UIR	*uir;			/* Pointer to unit info records		*/
} SPWAW_DOSSIER_BIR;

/* SPWAW dossier: battle info record set collection
 *
 * The info records are a superset of the core info records.
 */
typedef struct s_SPWAW_DOSSIER_BIRS {
	SPWAW_DOSSIER_BIR	pbir;			/* Pointer to player battle info record set	*/
	SPWAW_DOSSIER_BIR	obir;			/* Pointer to opponent battle info record set	*/
} SPWAW_DOSSIER_BIRS;

/* SPWAW dossier: battle turn data */
struct s_SPWAW_BTURN {
	SPWAW_BTURN		*prev;			/* Pointer to previous battle turn		*/
	SPWAW_BTURN		*next;			/* Pointer to next battle turn		*/
	SPWAW_SNAPSHOT		*snap;			/* Battle turn snapshot data			*/
	SPWAW_DATE		date;			/* Battle turn date				*/
	int			turn;			/* Battle turn					*/
	SPWAW_DOSSIER_BIRS	info;			/* Battle turn info record set			*/
	SPWAW_BATTLE		*battle;		/* Pointer to parent battle data struct		*/
};

/* SPWAW dossier: battle unit reassignment record */
typedef struct s_SPWAW_DOSSIER_BURA {
	USHORT			src;			/* Unit reassignment records: src UID		*/
	USHORT			dst;			/* Unit reassignment records: dst UID		*/
	bool			rpl;			/* Unit replacement flag			*/
} SPWAW_DOSSIER_BURA;

/* SPWAW dossier: battle data */
struct s_SPWAW_BATTLE {
	SPWAW_BATTLE		*prev;			/* Pointer to previous battle			*/
	SPWAW_BATTLE		*next;			/* Pointer to next battle			*/
	SPWAW_SNAPSHOT		*snap;			/* Battle turn #0 snapshot data			*/
	SPWAW_DATE		date;			/* Battle date					*/
	char			*location;		/* Battle location				*/
	int			OOB;			/* Opponent OOB ID				*/
	char			*miss_p1;		/* Player mission				*/
	char			*miss_p2;		/* Opponent mission				*/
	bool			meeting;		/* Meeting engagement flag			*/
	SPWAW_BTURN		**tlist;		/* Battle turn list				*/
	USHORT			tcnt;			/* Battle turn list element count		*/
	USHORT			tlen;			/* Battle turn list length			*/
	SPWAW_BTURN		*tfirst;		/* Pointer to first battle turn			*/
	SPWAW_BTURN		*tlast;			/* Pointer to last battle turn			*/
	SPWAW_DOSSIER_BIRS	*info_sob;		/* Battle info record sets at start of battle	*/
	SPWAW_DOSSIER_BIRS	*info_eob;		/* Battle info record sets at end of battle	*/
	SPWAW_DOSSIER_BURA	*ra;			/* Unit reassignment records			*/
	SPWAW_DOSSIER		*dossier;		/* Pointer to parent dossier data struct	*/
};

/* SPWAW dossier: data */
struct s_SPWAW_DOSSIER {
	char		*name;				/* Dossier name					*/
	char		*comment;			/* Dossier comment				*/
	char		*oobdir;			/* Original OOB data directory			*/
	SPWOOB		*oobdat;			/* OOB data					*/
	int		OOB;				/* OOB ID					*/
	USHORT		fcnt;				/* Core formations count			*/
	USHORT		ucnt;				/* Core units count				*/
	SPWAW_BATTLE	**blist;			/* Battle list					*/
	USHORT		bcnt;				/* Battle list element count			*/
	USHORT		blen;				/* Battle list length				*/
	SPWAW_BATTLE	*bfirst;			/* Pointer to last battle			*/
	SPWAW_BATTLE	*blast;				/* Pointer to last battle			*/
	void		*stab;				/* \internal string table			*/
};

/* SPWAW dossier: file info */
typedef struct s_SPWAW_DOSSIER_INFO {
	char		name[SPWAW_AZSDNAME+1];		/* Dossier name					*/
	char		comment[SPWAW_AZSDCMT+1];	/* Dossier comment				*/
	int		OOB;				/* Dossier OOB ID				*/
	USHORT		bcnt;				/* Dossier battle count				*/
} SPWAW_DOSSIER_INFO;

/*** API ***/

extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_new	(const char *name, const char *comment, SPWAW_DOSSIER **dossier);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_info	(const char *file, SPWAW_DOSSIER_INFO *info);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_load	(const char *file, SPWAW_DOSSIER **dossier);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_save	(SPWAW_DOSSIER **dossier, const char *file, bool compress);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_export	(const char *file, const char *export);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_import	(const char *import, const char *file);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_free	(SPWAW_DOSSIER **dossier);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_edit	(SPWAW_DOSSIER *dossier, const char *name, const char *comment);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_add	(SPWAW_DOSSIER *dossier, SPWAW_SNAPSHOT *snap, SPWAW_BTURN **bturn);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_del	(SPWAW_DOSSIER *dossier, SPWAW_BTURN *bturn);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_del	(SPWAW_DOSSIER *dossier, SPWAW_BATTLE *battle);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_savelist	(SPWAW_DOSSIER *dossier, SPWAW_SAVELIST **list);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_dossier_snaplist	(SPWAW_DOSSIER *dossier, SPWAW_SNAPLIST **list);

#endif	/* SPWAW_LIB_DOSSIER_H */
