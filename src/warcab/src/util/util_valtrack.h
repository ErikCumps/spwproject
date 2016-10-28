/** \file
 * The SPWaW war cabinet - utility functions - simple value tracking class.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	UTIL_VALTRACK_H
#define	UTIL_VALTRACK_H	1

/*! Simple value tracking class */
template <typename T>
class UtilValtrack
{
public:
	UtilValtrack	(void)
	{
		memset (&d, 0, sizeof (d));
		d.active = false;
	};

public:
	bool	changed	(T v)
	{
		bool	b;

		if (!d.active) {
			b = true; d.active = true;
		} else {
			b = (d.v != v);
		}

		d.v = v;
		return (b);
	}

private:
	struct s_data {
		bool	active;
		T	v;
	} d;
};

#endif	/* UTIL_VALTRACK_H */
