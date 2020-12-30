/** \file
 * The SPWaW Library - API types.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 *
 * \addtogroup API
 * @{
 */

#ifndef	SPWAW_LIB_TYPES_H
#define	SPWAW_LIB_TYPES_H	1

#include <spwawlib_api.h>

/* SPWAW date */
typedef struct s_SPWAW_DATE {
	short	year;
	char	month;
	char	day;
	char	hour;
	char	minute;
} SPWAW_DATE;

/* SPWAW timestamp */
typedef LONGLONG SPWAW_TIMESTAMP;

/* Empty SPWAW timestamp */
#define	SPWAW_TIMESTAMP_EMPTY	-2

/* Month-only timestamp: bits */
#define	SPWAW_MONTHONLY_TIMESTAMP_BITS	0x4000000000000000

/* Month-only timestamp: bitmask */
#define	SPWAW_MONTHONLY_TIMESTAMP_MASK	0xbfffffffffffffff

/* Pure Timestamp */
#define	SPWAW_PURE_TIMESTAMP(stamp_)	((stamp_) & SPWAW_MONTHONLY_TIMESTAMP_MASK)

/* SPWAW period */
typedef struct s_SPWAW_PERIOD {
	SPWAW_TIMESTAMP	stamp;
	char		years;
	char		months;
	char		weeks;
	char		days;
	char		hours;
	char		minutes;
} SPWAW_PERIOD;

/* SPWAW battle date */
typedef struct s_SPWAW_BATTLE_DATE {
	USHORT		btlidx;
	SPWAW_DATE	date;
} SPWAW_BATTLE_DATE;

/* SPWAW turn date */
typedef struct s_SPWAW_TURN_DATE {
	UINT		turn;
	SPWAW_DATE	date;
} SPWAW_TURN_DATE;

extern SPWAWLIB_API void	SPWAW_set_date		(SPWAW_DATE &date, short year, char month=0, char day=0, char hour=0, char minute=0);

extern SPWAWLIB_API bool	SPWAW_isMonthOnlyDate	(SPWAW_DATE *date);

extern SPWAWLIB_API SPWAW_ERROR	SPWAW_date2stamp	(SPWAW_DATE *date, SPWAW_TIMESTAMP *stamp);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_stamp2date	(SPWAW_TIMESTAMP *stamp, SPWAW_DATE *date);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_date2str		(SPWAW_DATE *date, char *buf, int len, bool full=true);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_date2str		(SPWAW_DATE *date, char **str, bool full=true);

extern SPWAWLIB_API SPWAW_ERROR	SPWAW_period2stamp	(SPWAW_PERIOD *period, SPWAW_TIMESTAMP *stamp);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_stamp2period	(SPWAW_TIMESTAMP *stamp, SPWAW_PERIOD *period);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_period2str	(SPWAW_PERIOD *period, char *buf, int len);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_period2str	(SPWAW_PERIOD *period, char **str);

extern SPWAWLIB_API SPWAW_ERROR	SPWAW_date_delta	(SPWAW_DATE *base, SPWAW_DATE *item, SPWAW_PERIOD *delta);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_date_add		(SPWAW_DATE *base, SPWAW_PERIOD *add, SPWAW_DATE *sum);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_period_delta	(SPWAW_PERIOD *base, SPWAW_PERIOD *item, SPWAW_PERIOD *delta);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_period_add	(SPWAW_PERIOD *base, SPWAW_PERIOD *add, SPWAW_PERIOD *sum);

extern SPWAWLIB_API int		SPWAW_bdate_cmp		(SPWAW_BATTLE_DATE *a, SPWAW_BATTLE_DATE *b);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_bdate2str		(SPWAW_BATTLE_DATE *bdate, char *buf, int len, bool tolog=false);
#define	SPWAW_BDATE(d_,v_,l_)	char v_[128]; SPWAW_bdate2str (&(d_), v_, sizeof (v_), l_)

extern SPWAWLIB_API int		SPWAW_tdate_cmp		(SPWAW_TURN_DATE *a, SPWAW_TURN_DATE *b);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_tdate2str		(SPWAW_TURN_DATE *tdate, char *buf, int len, bool tolog=false);
#define	SPWAW_TDATE(d_,v_,l_)	char v_[128]; SPWAW_tdate2str (&(d_), v_, sizeof (v_), l_)

/* SPWAW terrain */
typedef enum e_SPWAW_TERRAIN {
	SPWAW_TDESERT = 0,
	SPWAW_TSUMMER,
	SPWAW_TWINTER,
	SPWAW_TJUNGLE,
	SPWAW_TROUGH,
	SPWAW_TUNKNOWN,
	SPWAW_TSTARTCODE = SPWAW_TDESERT,
	SPWAW_TLASTCODE = SPWAW_TUNKNOWN
} SPWAW_TERRAIN;

#define	SPWAW_TERRAIN_CNT	(SPWAW_TLASTCODE - SPWAW_TSTARTCODE + 1)

extern SPWAWLIB_API const char *SPWAW_terrain2str (SPWAW_TERRAIN id);

/* SPWAW weather */
typedef enum e_SPWAW_WEATHER {
	SPWAW_WBEST = 0,
	SPWAW_WGOOD,
	SPWAW_WMEDIUM,
	SPWAW_WBAD,
	SPWAW_WVERYBAD,
	SPWAW_WWORST,
	SPWAW_WUNKNOWN,
	SPWAW_WSTARTCODE = SPWAW_WBEST,
	SPWAW_WLASTCODE = SPWAW_WUNKNOWN
} SPWAW_WEATHER;

#define	SPWAW_WEATHER_CNT	(SPWAW_WLASTCODE - SPWAW_WSTARTCODE + 1)

extern SPWAWLIB_API const char *SPWAW_weather2str (SPWAW_WEATHER id);

/* SPWAW mission type */
typedef enum e_SPWAW_MISSION {
	SPWAW_MASSAULT = 0,
	SPWAW_MADVANCE,
	SPWAW_MDELAY,
	SPWAW_MDEFEND,
	SPWAW_MUNKNOWN,
	SPWAW_MSTARTCODE = SPWAW_MASSAULT,
	SPWAW_MLASTCODE = SPWAW_MUNKNOWN
} SPWAW_MISSION;

#define	SPWAW_MISSION_CNT	(SPWAW_MLASTCODE - SPWAW_MSTARTCODE + 1)

extern SPWAWLIB_API const char *SPWAW_mission2str (SPWAW_MISSION id);

/* SPWAW formation status */
typedef enum e_SPWAW_FSTATUS {
	SPWAW_FREG = 0,
	SPWAW_FCORE,
	SPWAW_FAUX,
	SPWAW_FUNKNOWN,
	SPWAW_FSTARTCODE = SPWAW_FREG,
	SPWAW_FLASTCODE = SPWAW_FUNKNOWN
} SPWAW_FSTATUS;

#define	SPWAW_FSTATUS_CNT	(SPWAW_FLASTCODE - SPWAW_FSTARTCODE + 1)

extern SPWAWLIB_API const char *SPWAW_fstatus2str (SPWAW_FSTATUS id);

/* SPWAW unit status */
typedef enum e_SPWAW_USTATUS {
	SPWAW_UREADY = 0,
	SPWAW_UPINNED,
	SPWAW_URETREATING,
	SPWAW_UROUTED,
	SPWAW_UABANDONED,
	SPWAW_UDESTROYED,
	SPWAW_UBURNING,
	SPWAW_UBUTTONED,
	SPWAW_UIMMOBILIZED,
	SPWAW_UPASSENGER,
	SPWAW_UUNKNOWN,
	SPWAW_USTARTCODE = SPWAW_UREADY,
	SPWAW_ULASTCODE = SPWAW_UUNKNOWN
} SPWAW_USTATUS;

#define	SPWAW_USTATUS_CNT	(SPWAW_ULASTCODE - SPWAW_USTARTCODE + 1)

extern SPWAWLIB_API const char *SPWAW_ustatus2str (SPWAW_USTATUS id);

/* SPWAW leader rank */
typedef enum e_SPWAW_RANK {
	SPWAW_RKIA = 0,
	SPWAW_RPVT,
	SPWAW_RCPL,
	SPWAW_RSGT,
	SPWAW_R2LT,
	SPWAW_R1LT,
	SPWAW_RCPT,
	SPWAW_RMAJ,
	SPWAW_RLTC,
	SPWAW_RCOL,
	SPWAW_RGEN,
	SPWAW_RUNKNOWN,
	SPWAW_RSTARTCODE = SPWAW_RKIA,
	SPWAW_RLASTCODE = SPWAW_RUNKNOWN
} SPWAW_RANK;

#define	SPWAW_RANK_CNT	(SPWAW_RLASTCODE - SPWAW_RSTARTCODE + 1)

extern SPWAWLIB_API const char *SPWAW_rank2str	(SPWAW_RANK id);

/* SPWAW unit experience level */
typedef enum e_SPWAW_EXP {
	SPWAW_ECADET = 0,
	SPWAW_EGREEN,
	SPWAW_EAVERAGE,
	SPWAW_EVETERAN,
	SPWAW_EELITE,
	SPWAW_ESTARTCODE = SPWAW_ECADET,
	SPWAW_ELASTCODE = SPWAW_EELITE
} SPWAW_EXP;

#define	SPWAW_EXP_CNT	(SPWAW_ELASTCODE - SPWAW_ESTARTCODE + 1)

extern SPWAWLIB_API const char *SPWAW_exp2str	(SPWAW_EXP id);

/* SPWAW unit abandonment status */
typedef enum e_SPWAW_ABAND {
	SPWAW_ANONE = 0,	/* No abandonment		*/
	SPWAW_ASTAY,		/* Abandoned unit stayed behind	*/
	SPWAW_ALEFT,		/* Crew left abandonded unit	*/
	SPWAW_ASTARTCODE = SPWAW_ANONE,
	SPWAW_ALASTCODE = SPWAW_ALEFT
} SPWAW_ABAND;

#define	SPWAW_ABAND_CNT	(SPWAW_ALASTCODE - SPWAW_ASTARTCODE + 1)

extern SPWAWLIB_API const char *SPWAW_aband2str (SPWAW_ABAND id);

/* SPWAW unit contact status */
typedef enum e_SPWAW_CSTATUS {
	SPWAW_CSTATUSNONE = 0,	/* No contact	*/
	SPWAW_CSTATUSRADIO,	/* Radio contact	*/
	SPWAW_CSTATUSDIRECT,	/* Direct Contact	*/
	SPWAW_CSTATUSUNKNOWN,
	SPWAW_CSTATUSSTARTCODE = SPWAW_CSTATUSNONE,
	SPWAW_CSTATUSLASTCODE = SPWAW_CSTATUSUNKNOWN
} SPWAW_CSTATUS;

#define	SPWAW_CSTATUS_CNT	(SPWAW_CSTATUSLASTCODE - SPWAW_CSTATUSSTARTCODE + 1)

extern SPWAWLIB_API const char *SPWAW_cstatus2str (SPWAW_CSTATUS id);
/* SPWAW unit entrenchment status */
typedef enum e_SPWAW_ENTR {
	SPWAW_ENTRNONE = 0,	/* No entrenchment	*/
	SPWAW_ENTRCOVER,	/* Unit is in cover	*/
	SPWAW_ENTRDUGIN,	/* Unit is dug in	*/
	SPWAW_ENTRUNKNOWN,
	SPWAW_ENTRSTARTCODE = SPWAW_ENTRNONE,
	SPWAW_ENTRLASTCODE = SPWAW_ENTRUNKNOWN
} SPWAW_ENTR;

#define	SPWAW_ENTR_CNT	(SPWAW_ENTRLASTCODE - SPWAW_ENTRSTARTCODE + 1)

extern SPWAWLIB_API const char *SPWAW_entr2str (SPWAW_ENTR id);

/* SPWAW battle turn status */
typedef enum e_SPWAW_BTSTATUS {
	SPWAW_BTDEPLOY = 0,	/* Battle deployment	*/
	SPWAW_BTBUSY,		/* Battle in progress	*/
	SPWAW_BTSCORE,		/* Battle score		*/
	SPWAW_BTSTARTCODE = SPWAW_BTDEPLOY,
	SPWAW_BTLASTCODE = SPWAW_BTSCORE
} SPWAW_BTSTATUS;

#define	SPWAW_BTSTATUS_CNT	(SPWAW_BTLASTCODE - SPWAW_BTSTARTCODE + 1)

extern SPWAWLIB_API const char *SPWAW_btstatus2str (SPWAW_BTSTATUS id);

/* SPWAW terrain feature set */
typedef union u_SPWAW_TFS {
	struct s_SPWAW_TFS {
		// Water features
		ULONGLONG	stream:1;		/* Terrain contains stream		*/
		ULONGLONG	swamp:1;		/* Terrain contains swamp		*/
		ULONGLONG	water:1;		/* Terrain contains water		*/
		ULONGLONG	water_deep:1;		/* Terrain contains deep water		*/
		ULONGLONG	water_shallow:1;	/* Terrain contains shallow water	*/

		// Natural features
		ULONGLONG	slope:1;		/* Terrain contains slope		*/
		ULONGLONG	hole:1;			/* Terrain contains hole		*/
		ULONGLONG	mud:1;			/* Terrain contains mud			*/
		ULONGLONG	rough:1;		/* Terrain contains rough		*/
		ULONGLONG	snowdrift:1;		/* Terrain contains snowdrift		*/
		ULONGLONG	softsand:1;		/* Terrain contains soft sand		*/
		ULONGLONG	tallgrass:1;		/* Terrain contains tall grass		*/
		ULONGLONG	trees:1;		/* Terrain contains trees		*/

		// Human features
		ULONGLONG	bocage:1;		/* Terrain contains bocage		*/
		ULONGLONG	ditch:1;		/* Terrain contains ditch		*/
		ULONGLONG	field:1;		/* Terrain contains field		*/
		ULONGLONG	hedgerow:1;		/* Terrain contains hedgerow		*/
		ULONGLONG	orchard:1;		/* Terrain contains orchard		*/

		// Constructions
		ULONGLONG	building_stone:1;	/* Terrain contains stone building	*/
		ULONGLONG	building_wood:1;	/* Terrain contains wooden building	*/
		ULONGLONG	bridge_wood:1;		/* Terrain contains wooden bridge	*/
		ULONGLONG	bridge_stone:1;		/* Terrain contains stone bridge	*/
		ULONGLONG	wall:1;			/* Terrain contains wall		*/
		ULONGLONG	trench1:1;		/* Terrain contains trench type #1	*/
		ULONGLONG	trench2:1;		/* Terrain contains trench type #2	*/

		// Transportation
		ULONGLONG	road1:1;		/* Terrain contains primary road	*/
		ULONGLONG	road2:1;		/* Terrain contains secondary road	*/
		ULONGLONG	railroad:1;		/* Terrain contains railroad		*/
		ULONGLONG	tramline:1;		/* Terrain contains tramline		*/
		ULONGLONG	path:1;			/* Terrain contains path		*/
	}		tfs;
	ULONGLONG	raw;
} SPWAW_TFS;

extern SPWAWLIB_API	bool	SPWAW_tfs2water	(SPWAW_TFS tfs);
extern SPWAWLIB_API	bool	SPWAW_tfs2bridge(SPWAW_TFS tfs);
extern SPWAWLIB_API	bool	SPWAW_tfs2road	(SPWAW_TFS tfs);

/* SPWAW victory hex ownership status */
typedef enum e_SPWAW_VHSTATUS {
	SPWAW_VHN = 0,		/* Victory hex unassigned	*/
	SPWAW_VHP1,		/* Victory hex owned by Player1	*/
	SPWAW_VHP2,		/* Victory hex owned by Player2	*/
	SPWAW_VHUNKNOWN,
	SPWAW_VHSTARTCODE = SPWAW_VHN,
	SPWAW_VHLASTCODE = SPWAW_VHUNKNOWN
} SPWAW_VHSTATUS;

#define	SPWAW_VHSTATUS_CNT	(SPWAW_VHLASTCODE - SPWAW_VHSTARTCODE + 1)

extern SPWAWLIB_API const char *SPWAW_vhstatus2str (SPWAW_VHSTATUS id);

/* SPWAW battle results */
typedef enum e_SPWAW_BRESULT {
	SPWAW_BRDV = 0,		/* Decisive victory	*/
	SPWAW_BRMV,		/* Marginal victory	*/
	SPWAW_BRDB,		/* Drawn battle		*/
	SPWAW_BRMD,		/* Marginal defeat	*/
	SPWAW_BRDD,		/* Decisive defeat	*/
	SPWAW_BRUNKNOWN,
	SPWAW_BRSTARTCODE = SPWAW_BRDV,
	SPWAW_BRLASTCODE = SPWAW_BRUNKNOWN
} SPWAW_BRESULT;

#define	SPWAW_BRESULT_CNT	(SPWAW_BRLASTCODE - SPWAW_BRSTARTCODE + 1)

extern SPWAWLIB_API const char *SPWAW_bresult2str (SPWAW_BRESULT result);

/* SPWAW battle type */
typedef enum e_SPWAW_BATTLE_TYPE {
	SPWAW_UNKNOWN_BATTLE = 0,	/* The type of this battle is unknown			*/
	SPWAW_CAMPAIGN_BATTLE,		/* A campaign battle (allows campaign tracking)		*/
	SPWAW_STDALONE_BATTLE,		/* A standalone battle (no campaign tracking)		*/
	SPWAW_MEGACAM_BATTLE		/* An SP:WaW Mega Campaign battle (tracking savegame)	*/
} SPWAW_BATTLE_TYPE;

extern SPWAWLIB_API const char *SPWAW_battletype2str	(SPWAW_BATTLE_TYPE type);

/* SPWAW dossier type */
typedef enum e_SPWAW_DOSSIER_TYPE {
	SPWAW_EMPTY_DOSSIER = 0,	/* An empty dossier (type not yet determined)		*/
	SPWAW_CAMPAIGN_DOSSIER,		/* A campaign dossier (tracks a campaign)		*/
	SPWAW_STDALONE_DOSSIER,		/* A standalone dossier (no campaign tracking)		*/
	SPWAW_MEGACAM_DOSSIER		/* An SP:WaW Mega Campaign dossier (tracking savegame)	*/
} SPWAW_DOSSIER_TYPE;

extern SPWAWLIB_API const char *SPWAW_dossiertype2str	(SPWAW_DOSSIER_TYPE type);

/* SPWAW unit type */
typedef enum e_SPWAW_UNIT_TYPE {
	SPWAW_UNIT_TYPE_UNKNOWN = 0,	/* Unknown unit type		*/
	SPWAW_UNIT_TYPE_UNIT,		/* Unit				*/
	SPWAW_UNIT_TYPE_CREW,		/* Crew				*/
	SPWAW_UNIT_TYPE_SPAU		/* Special Attached Unit	*/
} SPWAW_UNIT_TYPE;

extern SPWAWLIB_API const char *SPWAW_unittype2str	(SPWAW_UNIT_TYPE type);

/* SPWAW game type */
typedef enum e_SPWAW_GAME_TYPE {
	SPWAW_GAME_TYPE_UNKNOWN = 0,	/* Unknown game type		*/
	SPWAW_GAME_TYPE_SPWAW,		/* The SP:WaW game		*/
	SPWAW_GAME_TYPE_WINSPWW2,	/* The winSPWW2 game		*/
	SPWAW_GAME_TYPE_STARTCODE = SPWAW_GAME_TYPE_UNKNOWN,
	SPWAW_GAME_TYPE_LASTCODE = SPWAW_GAME_TYPE_WINSPWW2
} SPWAW_GAME_TYPE;

#define	SPWAW_GAME_TYPE_CNT	(SPWAW_GAME_TYPE_LASTCODE - SPWAW_GAME_TYPE_STARTCODE + 1)

extern SPWAWLIB_API const char *	SPWAW_gametype2str	(SPWAW_GAME_TYPE gametype);
extern SPWAWLIB_API SPWAW_GAME_TYPE	SPWAW_str2gametype	(char * gametype);

/* SPWAW formation type */
#define	SPWAW_FORMATION_TYPE_CORE	0x01	/* Formation contains core units	*/
#define	SPWAW_FORMATION_TYPE_SUPPORT	0x02	/* Formation contains support units	*/

/* SPWAW save type */
typedef enum e_SPWAW_SAVE_TYPE {
	SPWAW_SAVE_TYPE_UNKNOWN = 0,	/* Unknown save type				*/
	SPWAW_SAVE_TYPE_REGULAR,	/* Regular save type				*/
	SPWAW_SAVE_TYPE_MEGACAM,	/* SP:WaW General's Edition Mega Campaign	*/
} SPWAW_SAVE_TYPE;

extern SPWAWLIB_API const char *	SPWAW_savetype2str	(SPWAW_SAVE_TYPE savetype);
extern SPWAWLIB_API SPWAW_SAVE_TYPE	SPWAW_str2savetype	(char * savetype);

/* SPWAW savegame descriptor */
typedef struct s_SPWAW_SAVEGAME_DESCRIPTOR {
	SPWAW_GAME_TYPE		gametype;
	SPWAW_SAVE_TYPE		savetype;
	char			*oobdir;
	char			*path;
	bool			numeric_id;
	union u_id {
		unsigned int	number;
		char		*name;
	}			id;
} SPWAW_SAVEGAME_DESCRIPTOR;

extern SPWAWLIB_API SPWAW_ERROR	SPWAW_savegame_descriptor_init	(SPWAW_SAVEGAME_DESCRIPTOR &sgd, SPWAW_GAME_TYPE gametype, SPWAW_SAVE_TYPE savetype, const char *oobdir, const char *path, unsigned int id);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_savegame_descriptor_init	(SPWAW_SAVEGAME_DESCRIPTOR &sgd, SPWAW_GAME_TYPE gametype, SPWAW_SAVE_TYPE savetype, const char *oobdir, const char *path, const char * id);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_savegame_descriptor_init	(SPWAW_SAVEGAME_DESCRIPTOR &sgd, SPWAW_SAVEGAME_DESCRIPTOR &src);
extern SPWAWLIB_API void	SPWAW_savegame_descriptor_clear	(SPWAW_SAVEGAME_DESCRIPTOR &sgd);
extern SPWAWLIB_API bool	SPWAW_savegame_descriptor_equal	(SPWAW_SAVEGAME_DESCRIPTOR &a, SPWAW_SAVEGAME_DESCRIPTOR &b);

#endif	/* SPWAW_LIB_TYPES_H */

/** @} */
