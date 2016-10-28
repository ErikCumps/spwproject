/** \file
 * The SPWaW war cabinet - utility functions - simple MDLD_TREE_ITEM reference tracking class.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	UTIL_MDLTREE_REFTRACK_H
#define	UTIL_MDLTREE_REFTRACK_H	1

#include "warcab.h"

/*! Simple reference tracking class */
class UtilMdlTreeReftrack
{
public:
	UtilMdlTreeReftrack	(void);
	~UtilMdlTreeReftrack	(void);

public:
	bool	changed	(MDLD_TREE_ITEM *item);
	void	update (MDLD_TREE_ITEM *item);

private:
	struct s_data {
		bool		active;
		MDLD_TREE_ITEM	*ptr;
		unsigned int	seq;
	} d;
};

#endif	/* UTIL_MDLTREE_REFTRACK_H */
