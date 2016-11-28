/** \file
 * The SPWaW war cabinet - utility functions - simple buffered IO class.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	UTIL_STRBUF_H
#define	UTIL_STRBUF_H	1

/*! Simple buffered IO class */
class UtilStrbuf
{
public:
	UtilStrbuf	(char *buf, unsigned long len, bool init, bool nl);
	~UtilStrbuf	(void);

public:
	bool	full	(void);
	void	add	(char *string);
	void	del	(unsigned long cnt);
	void	printf	(char *fmt, ...);
	void	clear	(void);

private:
	struct s_data {
		char		*buf;
		unsigned long	size;
		char		*ptr;
		unsigned long	left;
		bool		eot_nl;
	} d;
};

#endif	/* UTIL_STRBUF_H */
