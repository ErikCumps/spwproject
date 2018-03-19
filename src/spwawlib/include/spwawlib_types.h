/** \file
 * The SPWaW Library - API types.
 *
 * Copyright (C) 2007-2017 Erik Cumps <erik.cumps@gmail.com>
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

extern SPWAWLIB_API void	SPWAW_set_date		(SPWAW_DATE &date, short year, char month=0, char day=0, char hour=0, char minute=0);

extern SPWAWLIB_API SPWAW_ERROR	SPWAW_date2stamp	(SPWAW_DATE *date, SPWAW_TIMESTAMP *stamp);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_stamp2date	(SPWAW_TIMESTAMP *stamp, SPWAW_DATE *date);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_date2str		(SPWAW_DATE *date, char *buf, int len);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_date2str		(SPWAW_DATE *date, char **str);

extern SPWAWLIB_API SPWAW_ERROR	SPWAW_period2stamp	(SPWAW_PERIOD *period, SPWAW_TIMESTAMP *stamp);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_stamp2period	(SPWAW_TIMESTAMP *stamp, SPWAW_PERIOD *period);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_period2str	(SPWAW_PERIOD *period, char *buf, int len);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_period2str	(SPWAW_PERIOD *period, char **str);

extern SPWAWLIB_API SPWAW_ERROR	SPWAW_date_delta	(SPWAW_DATE *base, SPWAW_DATE *item, SPWAW_PERIOD *delta);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_date_add		(SPWAW_DATE *base, SPWAW_PERIOD *add, SPWAW_DATE *sum);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_period_delta	(SPWAW_PERIOD *base, SPWAW_PERIOD *item, SPWAW_PERIOD *delta);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_period_add	(SPWAW_PERIOD *base, SPWAW_PERIOD *add, SPWAW_PERIOD *sum);

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

/* SPWAW snapshot type */
typedef enum e_SPWAW_SNAPSHOT_TYPE {
	SPWAW_CAMPAIGN_SNAPSHOT = 0,	/* A campaign snapshot (allows campaign tracking)	*/
	SPWAW_STDALONE_SNAPSHOT		/* A standalone snapshot (no campaign tracking)		*/
} SPWAW_SNAPSHOT_TYPE;

extern SPWAWLIB_API const char *SPWAW_snapshottype2str	(SPWAW_SNAPSHOT_TYPE type);

/* SPWAW dossier type */
typedef enum e_SPWAW_DOSSIER_TYPE {
	SPWAW_EMPTY_DOSSIER = 0,	/* An empty dossier (type not yet determined)		*/
	SPWAW_CAMPAIGN_DOSSIER,		/* A campaign dossier (tracks a campaign)		*/
	SPWAW_STDALONE_DOSSIER		/* A standalone dossier (no campaign tracking)		*/
} SPWAW_DOSSIER_TYPE;

extern SPWAWLIB_API const char *SPWAW_dossiertype2str	(SPWAW_DOSSIER_TYPE type);

#endif	/* SPWAW_LIB_TYPES_H */

/** @} */
