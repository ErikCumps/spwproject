/** \file
 * The Steel Panthers World at War report tool.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL V2
 */

#include "spwreport.h"

void
map_dump (SPWAW_SNAPSHOT *ptr, FILE *rf)
{
	int			i, cnt;
	DWORD			x, y;
	SPWAW_SNAP_OOB_UEL	*p;

	if (!ptr || !rf) return;

	fprintf (rf, "# Map size: WIDTH HEIGHT\n");
	fprintf (rf, "[map]\n");
	fprintf (rf, "%u %u\n", ptr->game.map.width, ptr->game.map.height);
	fprintf (rf, "\n");

	fprintf (rf, "# Map hex info: X Y HEIGHT WATER BRIDGE R1C R2C RRC ROAD\n");
	fprintf (rf, "[map info]\n");
	fprintf (rf, "%u\n", ptr->game.map.width * ptr->game.map.height);
	for (y=0; y<ptr->raw.game.map.height; y++) {
		for (x=0; x<ptr->raw.game.map.width; x++) {
			fprintf (rf, "%lu %lu %d %u %u %u %u %u %u\n", x, y,
				(signed char)ptr->raw.game.map.data[y*ptr->raw.game.map.width+x].height,
				ptr->game.map.data[y*ptr->game.map.width+x].water,
				ptr->game.map.data[y*ptr->game.map.width+x].bridge,
				ptr->game.map.data[y*ptr->game.map.width+x].conn_road1,
				ptr->game.map.data[y*ptr->game.map.width+x].conn_road2,
				ptr->game.map.data[y*ptr->game.map.width+x].conn_rail,
				ptr->game.map.data[y*ptr->game.map.width+x].road);
		}
	}
	fprintf (rf, "\n");

	fprintf (rf, "# Blue positions: X Y\n");
	fprintf (rf, "[blue]\n");

	cnt = 0;
	for (i=0; i<ptr->OOBp1.battle.units.cnt; i++) {
		p = &(ptr->OOBp1.battle.units.list[i]);
		if (p->data.posx >= 0) cnt++;
	}
	fprintf (rf, "%d\n", cnt);
	for (i=0; i<ptr->OOBp1.battle.units.cnt; i++) {
		p = &(ptr->OOBp1.battle.units.list[i]);
		if (p->data.posx < 0) continue;
		fprintf (rf, "%u %u\n",p->data.posx, p->data.posy);
	}
	fprintf (rf, "\n");

	fprintf (rf, "# Red positions: X Y\n");
	fprintf (rf, "[red]\n");

	cnt = 0;
	for (i=0; i<ptr->OOBp2.battle.units.cnt; i++) {
		p = &(ptr->OOBp2.battle.units.list[i]);
		if (p->data.posx >= 0) cnt++;
	}
	fprintf (rf, "%d\n", cnt);
	for (i=0; i<ptr->OOBp2.battle.units.cnt; i++) {
		p = &(ptr->OOBp2.battle.units.list[i]);
		if (p->data.posx < 0) continue;
		fprintf (rf, "%u %u\n",p->data.posx, p->data.posy);
	}
	fprintf (rf, "\n");

	fprintf (rf, "# Victory hexes: X Y OWNER\n");
	fprintf (rf, "[victory]\n");

	fprintf (rf, "%u\n", SPWAW_VHEXCNT);
	for (i=0; i<SPWAW_VHEXCNT; i++) {
		SPWAW_SNAP_VHEX *v = &(ptr->game.battle.data.vhex[i]);
		fprintf (rf, "%u %u %u\n", v->x, v->y, v->status);
	}
	fprintf (rf, "\n");

	return;
}
