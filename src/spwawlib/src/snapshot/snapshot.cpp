#include "stdafx.h"
#include <spwawlib_api.h>
#include <spwawlib_snapshot.h>
#include "common/internal.h"

// Checks unit record ID
bool
check_unitid (DWORD urid, SPWAW_SNAP_OOB_URAW *ptr)
{
	DWORD	i;

	if (!ptr) return (false);

	for (i=0; i<ptr->cnt; i++)
		if (urid == ptr->raw[i].RID) return (true);

	return (false);
}

// Checks formation record ID
bool
check_formationid (DWORD frid, SPWAW_SNAP_OOB_FRAW *ptr, BYTE *fid)
{
	DWORD	i;

	if (!ptr) return (false);

	for (i=0; i<ptr->cnt; i++)
		if (frid == ptr->raw[i].RID) {
			if (fid) *fid = ptr->raw[i].FID;
			return (true);
		}

	return (false);
}

// Obtain unit data from unit record ID
SPWAW_SNAP_OOB_UEL *
unitbyid (DWORD urid, SPWAW_SNAP_OOB_FORCE *ptr)
{
	SPWAW_SNAP_OOB_UEL	*p = NULL;
	DWORD			i;

	for (i=0; i<ptr->units.cnt; i++) {
		if (ptr->units.list[i].data.RID == urid) {
			p = &(ptr->units.list[i]);
			break;
		}
	}
	return (p);
}

// Obtain crew data from unit record ID
SPWAW_SNAP_OOB_UEL *
crewbyid (DWORD urid, SPWAW_SNAP_OOB_FORCE *ptr)
{
	SPWAW_SNAP_OOB_UEL	*p = NULL;
	DWORD			i;

	for (i=0; i<ptr->crews.cnt; i++) {
		if (ptr->crews.list[i].data.RID == urid) {
			p = &(ptr->crews.list[i]);
			break;
		}
	}
	return (p);
}

// Obtain either unit or crew data from unit record ID
SPWAW_SNAP_OOB_UEL *
unitorcrewbyid (DWORD urid, SPWAW_SNAP_OOB_FORCE *ptr)
{
	SPWAW_SNAP_OOB_UEL	*p = NULL;

	p = unitbyid (urid, ptr);
	if (!p) p = crewbyid (urid, ptr);

	return (p);
}

SPWAW_SNAP_OOB_FEL *
formationbyid (DWORD frid, SPWAW_SNAP_OOB_FORCE *ptr)
{
	SPWAW_SNAP_OOB_FEL	*p = NULL;
	DWORD			i;

	for (i=0; i<ptr->formations.cnt; i++) {
		if (ptr->formations.list[i].data.RID == frid) {
			p = &(ptr->formations.list[i]);
			break;
		}
	}
	return (p);
}
