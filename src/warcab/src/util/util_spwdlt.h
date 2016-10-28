/** \file
 * The SPWaW war cabinet - utility functions - information delta handling.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	UTIL_SPWDLT_H
#define	UTIL_SPWDLT_H	1

typedef enum e_SPWDLT_TYPE {
	SPWDLT_NONE = 0,
	SPWDLT_BOOL,
	SPWDLT_INT,
	SPWDLT_DBL,
	SPWDLT_STR,
	SPWDLT_CHR
} SPWDLT_TYPE;

typedef struct s_SPWDLT {
	bool		used;
	SPWDLT_TYPE	type;
	void		*curr;
	void		*base;
	bool		delta;
	union u_dlt {
		int	b;
		int	i;
		double	d;
		bool	s;
	} dlt;
} SPWDLT;

extern void	SPWDLT_prep	(SPWDLT *dlt, SPWDLT_TYPE type, void *curr, void *base, unsigned int offs);
extern void	SPWDLT_set	(SPWDLT *dlt);
extern bool	SPWDLT_bool	(SPWDLT *dlt);
extern int	SPWDLT_int	(SPWDLT *dlt);
extern double	SPWDLT_dbl	(SPWDLT *dlt);
extern char *	SPWDLT_str	(SPWDLT *dlt);
extern char	SPWDLT_chr	(SPWDLT *dlt);
extern bool	SPWDLT_check	(SPWDLT *dlt);
extern int	SPWDLT_getbool	(SPWDLT *dlt);
extern int	SPWDLT_getint	(SPWDLT *dlt);
extern double	SPWDLT_getdbl	(SPWDLT *dlt);
extern bool	SPWDLT_getstr	(SPWDLT *dlt);
extern bool	SPWDLT_getchr	(SPWDLT *dlt);
extern int	SPWDLT_summary	(SPWDLT *dlt);

#endif	/* UTIL_SPWDLT_H */
