/** \file
 * The SPWaW Library - unit history tracking list job API.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 *
 * \addtogroup API
 * @{
 */

#ifndef	SPWAW_LIB_UHT_JOB_H
#define	SPWAW_LIB_UHT_JOB_H	1

#include <spwawlib_api.h>

/* Forward declarations for convenience */
typedef struct s_SPWAW_DOSSIER_UIR	SPWAW_DOSSIER_UIR;
typedef struct s_SPWAW_BATTLE_DATE	SPWAW_BATTLE_DATE;
typedef struct s_SPWAW_UHTE		SPWAW_UHTE;
typedef struct s_SPWAW_UHT		SPWAW_UHT;
typedef struct s_SPWAW_UHT_BINFO	SPWAW_UHT_BINFO;

/* SPWAW UHT list job type */
typedef enum e_SPWAW_UHT_LIST_JOB_TYPE {
	SPWAW_UHT_LIST_CHAIN = 0,		/* Detect and count UHTE on all UHTE chains					*/
	SPWAW_UHT_LIST_BINFO,			/* Detect UHTE on a battle info data list					*/
} SPWAW_UHT_LIST_JOB_TYPE;

/* SPWAW UHT list job result element.
 *
 * Note: the uhte result member content depends on the list job type:
 * + for SPWAW_UHT_LIST_CHAIN: pointer to the final UHTE of the chain with one or more detected UHTEs
 * + for SPWAW_UHT_LIST_BINFO: pointer to the detected UHTE
 */
typedef struct s_SPWAW_UHT_LISTEL {
	SPWAW_UHTE		*uhte;		/* Detected UHTE								*/
	unsigned long		count;		/* Number of detections for this UHTE						*/
	void			*data;		/* Optional storage for extra data						*/
} SPWAW_UHT_LISTEL;

/* SPWAW UHT list job result */
typedef struct s_SPWAW_UHT_LIST {
	SPWAW_UHT_LIST_JOB_TYPE	type;		/* The SPWAW UHT list job type that generated this result			*/
	SPWAW_UHT_LISTEL	*list;		/* List with detected UHTE							*/
	SPWAW_UHT_LISTEL	**smap;		/* Sort map									*/
	unsigned int		cnt;		/* Number of used elements in the list						*/
	unsigned int		len;		/* Total length of the list (in elements)					*/
} SPWAW_UHT_LIST;

/* SPWAW UHT list sort type */
typedef enum e_SPWAW_UHT_LIST_SORT {
	SPWAW_UHT_SORT_UID = 0,			/* Sort SPWAW_UHT_LISTEL on UHTE UID						*/
	SPWAW_UHT_SORT_COUNT,			/* Sort SPWAW_UHT_LISTEL on UHTE detection count				*/
	SPWAW_UHT_SORT_USER,			/* Sort SPWAW_UHT_LISTEL using user-provided comparison function		*/
} SPWAW_UHT_LIST_SORT;

/* SPWAW UHT list sort comparison function. Arguments are pointers to *SPWAW_UHT_LISTEL. */
typedef int (*SPWAW_UHT_list_cmp) (const void *a, const void *b);

/* SPWAW UHT list job detection private data helper callback */
typedef char *(* SPWAW_UHT_list_extra_cb)(SPWAW_DOSSIER_UIR *uir);

/* SPWAW UHT list job detection data callback context structure */
typedef struct s_SPWAW_UHT_LIST_CBCTX {
	SPWAW_UHTE		*uhte;		/* Detection: UHTE								*/
	SPWAW_BATTLE_DATE	*bdate;		/* Detection: battle date							*/
	SPWAW_DOSSIER_UIR	*uir;		/* UIR for detected UHTE at battle date						*/
	SPWAW_BATTLE_DATE	*pbdate;	/* Battle date of previous battle						*/
	SPWAW_DOSSIER_UIR	*puir;		/* UIR for detected UHTE at previous battle					*/
	bool			decommisioned;	/* Is this is a decommissioned UHTE?						*/
	bool			first;		/* Is this the first detection in the chain?					*/
	bool			last;		/* Is this the last detection in the chain?					*/
	void			**data;		/* Pointer to storage for pointer to private data				*/
	void			*extra;		/* Optional caller-provided extra callback context data (can be NULL)		*/
} SPWAW_UHT_LIST_CBCTX;

/* SPWAW UHT list job detection data callback */
typedef void (*SPWAW_UHT_list_data_cb) (SPWAW_UHT_LIST_CBCTX &context);

/* SPWAW UHT list job descriptor */
typedef struct s_SPWAW_UHT_LIST_JOB {
	SPWAW_UHT_LIST_JOB_TYPE	type;		/* The SPWAW UHT list job type							*/
	union u_src {
		SPWAW_UHT	*uht;		/* Data source: UHT								*/
		struct s_bid {			/* Data source: battle info							*/
			SPWAW_UHT_BINFO		*bid;	/* Battle info data							*/
			SPWAW_BATTLE_DATE	*pbd;	/* Date of previous battle						*/
		}	bid;
	}	src;				/* UHT data source to perform the list job on					*/
	USHORT			status;		/* Unit history status to detect						*/
	bool			reversed;	/* Reversed detection direction? (newest to oldest)				*/
	bool			allow_decomm;	/* Allow decommissioned units to be detected					*/
	bool			inc_terminal;	/* Include terminal UHTE (chain head or tail)					*/
	SPWAW_UHT_list_data_cb	data_cb;	/* Optional detection data callback (can be NULL)				*/
	void			*extra;		/* Optional caller-provided extra callback context data (can be NULL)		*/
	SPWAW_UHT_LIST		*dst;		/* Storage for job result							*/
} SPWAW_UHT_LIST_JOB;

/*** API ***/

/* SPWAW UHT list handling */
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_UHT_list_init	(SPWAW_UHT_LIST_JOB_TYPE type, unsigned int len, SPWAW_UHT_LIST **list);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_UHT_list_free	(SPWAW_UHT_LIST **list);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_UHT_list_add	(SPWAW_UHT_LIST *list, SPWAW_UHTE *uhte, SPWAW_UHT_LISTEL **listel);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_UHT_list_sort	(SPWAW_UHT_LIST *list, SPWAW_UHT_LIST_SORT type, bool ascending, SPWAW_UHT_list_cmp cmp);

/* Perform an SPWAW UHT list job */
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_UHT_list_job	(SPWAW_UHT_LIST_JOB &job);

/* Sort the SPWAW UHT list in a list job */
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_UHT_list_job_sort	(SPWAW_UHT_LIST_JOB &job, SPWAW_UHT_LIST_SORT type, bool ascending, SPWAW_UHT_list_cmp cmp);

/* Predefined SPWAW UHT list comparison functions */
extern SPWAWLIB_API int	SPWAW_UHT_list_cmp_UID_ascending	(const void *a, const void *b);
extern SPWAWLIB_API int	SPWAW_UHT_list_cmp_UID_descending	(const void *a, const void *b);
extern SPWAWLIB_API int	SPWAW_UHT_list_cmp_CNT_ascending	(const void *a, const void *b);
extern SPWAWLIB_API int	SPWAW_UHT_list_cmp_CNT_descending	(const void *a, const void *b);

#endif	/* SPWAW_LIB_UHT_JOB_H */

/** @} */
