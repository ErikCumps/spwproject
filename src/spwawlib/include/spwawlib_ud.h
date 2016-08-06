/** \file
 * The SPWaW Library - unknown data API.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SPWAW_LIB_UD_H
#define	SPWAW_LIB_UD_H	1

/* SPWAW generic unknown data: element structure */
typedef struct s_SPWAW_UDEL {
	struct s_SPWAW_UDEL	*prev;
	struct s_SPWAW_UDEL	*next;
	DWORD			addr;
	DWORD			size;
	BYTE			data[1];
} SPWAW_UDEL;

/* SPWAW generic unknown data: array structure */
typedef struct s_SPWAW_UD {
	DWORD		cnt;	/* Number of UDELs		*/
	DWORD		size;	/* Size of originating array	*/
	SPWAW_UDEL	*head;
	SPWAW_UDEL	*tail;
	bool		freeme;	/* malloced UD */
} SPWAW_UD;

#endif	/* SPWAW_LIB_API_H */
