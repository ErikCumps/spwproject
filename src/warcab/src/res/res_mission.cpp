/** \file
 * The SPWaW war cabinet - resource handling - mission types.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "../common.h"

typedef struct s_MISSION_MAP {
	SPWAW_MISSION	mission;
	unsigned long	rid;
} MISSION_MAP;

#define	MISSION(id_)	{ SPWAW_M##id_, RID_MSSN_##id_ }

static const MISSION_MAP missionmap[] = {
	MISSION(ASSAULT),
	MISSION(ADVANCE),
	MISSION(DELAY),
	MISSION(DEFEND),
	{ SPWAW_MUNKNOWN, 0 }
};

QPixmap *
RES_mission (SPWAW_MISSION mission, bool meeting)
{
	int	i = 0;
	QPixmap	*pm = NULL;

	if (meeting) return (RES_pixmap (RID_MSSN_MEETING));

	while (missionmap[i].mission != SPWAW_MUNKNOWN) {
		if (missionmap[i].mission == mission) break;
		i++;
	}
	if (missionmap[i].mission != SPWAW_MUNKNOWN) {
		pm = RES_pixmap (missionmap[i].rid);
	} else {
		pm = RES_pixmap (RID_MSSN_UNKNOWN);
	}
	return (pm);

}