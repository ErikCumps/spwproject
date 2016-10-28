/** \file
 * The SPWaW war cabinet - resource handling - nation flags.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "../common.h"

typedef struct s_OOB_FLAG_MAP {
	BYTE		OOB;
	unsigned long	rid;
} OOB_FLAG_MAP;

#define	OOBFLAG1(id_)	{ id_, RID_OOB_FLAG_0##id_ }
#define	OOBFLAG(id_)	{ id_, RID_OOB_FLAG_##id_ }

static const OOB_FLAG_MAP flagmap[] = {
	OOBFLAG1 (9),
	OOBFLAG (15),
	OOBFLAG (16),
	OOBFLAG (19),
	OOBFLAG (20),
	OOBFLAG (29),
	OOBFLAG (70),
	OOBFLAG (71),
	OOBFLAG (72),
	OOBFLAG (73),
	OOBFLAG (74),
	OOBFLAG (75),
	OOBFLAG (76),
	OOBFLAG (77),
	OOBFLAG (78),
	OOBFLAG (79),
	OOBFLAG (80),
	OOBFLAG (81),
	OOBFLAG (82),
	OOBFLAG (83),
	OOBFLAG (84),
	OOBFLAG (85),
	OOBFLAG (86),
	OOBFLAG (87),
	OOBFLAG (88),
	OOBFLAG (89),
	OOBFLAG (90),
	OOBFLAG (91),
	{ 0, 0 }
};

QPixmap *
RES_flag (BYTE oob)
{
	int	i = 0;
	QPixmap	*pm = NULL;

	while (flagmap[i].OOB != 0) {
		if (flagmap[i].OOB == oob) break;
		i++;
	}
	if (flagmap[i].OOB != 0) {
		pm = RES_pixmap (flagmap[i].rid);
	} else {
		pm = RES_pixmap (RID_OOB_FLAG_00);
	}
	return (pm);
}