/** \file
 * The SPWaW war cabinet - utility functions - simple sequence number class.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	UTIL_SEQNUM_H
#define	UTIL_SEQNUM_H	1

/*! Simple sequence number class */
class UtilSeqnum
{
public:
	UtilSeqnum			(void);
	~UtilSeqnum			(void);

public:
	void		update		(void);
	bool		is_updated	(unsigned int &ref);
	unsigned int	value		(void);

private:
	volatile unsigned int	seq;
};

#endif	/* UTIL_SEQNUM_H */
