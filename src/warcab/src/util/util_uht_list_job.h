/** \file
 * The SPWaW war cabinet - utility functions - unit history tracking list jobs.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	UTIL_UHT_LIST_JOB_H
#define	UTIL_UHT_LIST_JOB_H	1

#include <spwawlib_uht_job.h>

typedef enum e_UHT_LIST_JOB_TYPE {
	UHT_LIST_DOSSIER = 0,
	UHT_LIST_BATTLE,
} UHT_LIST_JOB_TYPE;

typedef void (*UHT_LIST_VISITOR_CB)(SPWAW_UHT_LISTEL *listel, SPWAW_DOSSIER_UIR *uir, UtilStrbuf *sb);

typedef struct s_UHT_LIST_JOB {
	const char			*what;		/* Job title or description					*/
	UHT_LIST_JOB_TYPE		type;		/* Job type							*/
	union u_in {
		struct s_d {
			SPWAW_DOSSIER	*dossier;	/* Job input: dossier						*/
		} d;
		struct s_b {
			SPWAW_BATTLE	*battle;	/* Job input: battle						*/
		} b;
	}	in;
	struct s_how {
		USHORT			status;		/* Job parameters: UHT status to detect				*/
		bool			reversed;	/* Job parameters: process UHT chain from newer to older	*/
		bool			allow_decomm;	/* Job parameters: allow detetcing decommissioned units?	*/
	}	how;
	struct s_dext {
		SPWAW_UHT_list_data_cb	data;		/* Job data extraction: data callback				*/
		void			*extra;		/* Job data extraction: data callback extra context		*/
		bool			counted;	/* Job data extraction: count detections per chain?		*/
	}	dext;
	struct s_sort {
		bool			sorted;		/* Job sort: sorting activated?					*/
		SPWAW_UHT_LIST_SORT	type;		/* Job sort: type						*/
		bool			ascending;	/* Job sort: sort ascending? (smaller/younger to bigger/older)	*/
		SPWAW_UHT_list_cmp	cmp;		/* Job sort: optional comparison function			*/
	}	sort;
	struct s_out {
		bool			skip_if_empty;	/* Job output: no output for empty list?			*/
		UHT_LIST_VISITOR_CB	visitor;	/* Job output: optional output visitor callback			*/
		const char		*hdrpre;	/* Job output: optional header prefix string			*/
		const char		*hdrpst;	/* Job output: optional header suffix string			*/
		const char		*lstpre;	/* Job output: optional list prefix string			*/
		const char		*lstpst;	/* Job output: optional list suffix string			*/
		UtilStrbuf		*strbuf;	/* Job output: optional pointer to target UtilStrbuf		*/
		QLabel			*hdr;		/* Job output: optional pointer to target header QLabel		*/
		QLabel			*lst;		/* Job output: optional pointer to target list QLabel		*/
	}	out;
} UHT_LIST_JOB;

extern void	UHT_list_job	(UHT_LIST_JOB &job);

#endif	/* UTIL_UHT_LIST_JOB_H */
