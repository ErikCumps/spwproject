#include "stdafx.h"
#include "snapshot/snapshot.h"

void
snapint_addmmas (SPWAW_IMMAS *dst, DWORD val, USHORT id)
{
	if (!dst) return;

	if (!dst->cnt) {
		dst->cnt = 1;
		dst->max.v = dst->min.v = dst->tot = val;
		dst->max.i = dst->min.i = id;
		dst->avg = val;
		dst->spr = 0;
	} else {
		if (val > dst->max.v) { dst->max.v = val; dst->max.i = id; }
		if (val < dst->min.v) { dst->min.v = val; dst->min.i = id; }
		dst->tot += val; dst->cnt++;
		dst->avg = (double)dst->tot / (double)dst->cnt;
		dst->spr = dst->max.v - dst->min.v;
	}
}

void
snapint_addmmas (SPWAW_FMMAS *dst, double val, USHORT id)
{
	if (!dst) return;

	if (!dst->cnt) {
		dst->cnt = 1;
		dst->max.v = dst->min.v = dst->tot = val;
		dst->max.i = dst->min.i = id;
		dst->avg = val;
		dst->spr = 0;
	} else {
		if (val > dst->max.v) { dst->max.v = val; dst->max.i = id; }
		if (val < dst->min.v) { dst->min.v = val; dst->min.i = id; }
		dst->tot += val; dst->cnt++;
		dst->avg = dst->tot / (double)dst->cnt;
		dst->spr = dst->max.v - dst->min.v;
	}
}

void
snapint_addmmas_set (SPWAW_IMMAS_SET *dst, DWORD val, SPWAW_SNAP_OOB_UEL *ptr)
{
	snapint_addmmas (&(dst->all), val, ptr->data.idx);
	snapint_addmmas (&(dst->rnk[ptr->data.rank]), val, ptr->data.idx);
	snapint_addmmas (&(dst->exp[ptr->data.eclass]), val, ptr->data.idx);
	snapint_addmmas (&(dst->cls[ptr->data.uclass]), val, ptr->data.idx);
}

void
snapint_addmmas_set (SPWAW_FMMAS_SET *dst, double val, SPWAW_SNAP_OOB_UEL *ptr)
{
	snapint_addmmas (&(dst->all), val, ptr->data.idx);
	snapint_addmmas (&(dst->rnk[ptr->data.rank]), val, ptr->data.idx);
	snapint_addmmas (&(dst->exp[ptr->data.eclass]), val, ptr->data.idx);
	snapint_addmmas (&(dst->cls[ptr->data.uclass]), val, ptr->data.idx);
}
