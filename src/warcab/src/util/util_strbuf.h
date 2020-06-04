/** \file
 * The SPWaW war cabinet - utility functions - simple buffered IO class.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
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
	UtilStrbuf	(bool nl);
	~UtilStrbuf	(void);

public:
	bool	full	(void);
	bool	empty	(void);
	void	add	(char *string);
	void	del	(unsigned long cnt);
	void	printf	(char *fmt, ...);
	void	clear	(void);
	char *	data	(void);

private:
	struct s_data {
		bool		grow;
		char		*buf;
		unsigned long	size;
		char		*ptr;
		unsigned long	left;
		bool		eot_nl;
		unsigned long	reserved;
	} d;

	void	limited_add	(char *str, unsigned long len);
	void	growing_add	(char *str, unsigned long len);
};

#endif	/* UTIL_STRBUF_H */
