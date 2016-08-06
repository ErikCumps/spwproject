#include "stdafx.h"
#include "snapshot/snapfile.h"
#include "fileio/fileio.h"
#include "utils/textconv.h"
#include "common/internal.h"

#define	ROF(b)	do { if (!b) { rc = SPWERR_FRFAILED; ERRORGOTO ("TC_v2t()", handle_error); } } while (0)

#define	dumpH(n,t)	do { b = TC_v2t (dst, "snapshot.header." #n, t, &(mhdr.##n)); ROF(b); } while (0)
#define	CdumpH(n,t)	do { b = TC_v2t (dst, "//snapshot.header." #n, t, &(mhdr.##n)); ROF(b); } while (0)
#define	dumpS(n,t)	do { b = TC_v2t (dst, "snapshot.source." #n, t, &(shdr.##n)); ROF(b); } while (0)
#define	dumpI(n,t)	do { b = TC_v2t (dst, "snapshot.info." #n, t, &(ihdr.##n)); ROF(b); } while (0)
#define	dumpCMT(n,t)	do { b = TC_v2t (dst, "snapshot.snap.cmt." #n, t, &(snap.cmt.##n)); ROF(b); } while (0)
#define	dumpB(n,t)	do { b = TC_v2t (dst, "snapshot.snap.battle." #n, t, &(snap.b.##n)); ROF(b); } while (0)
#define	dumpC(n,t)	do { b = TC_v2t (dst, "snapshot.snap.campaign." #n, t, &(snap.c.##n)); ROF(b); } while (0)
#define	dumpM(n,t)	do { b = TC_v2t (dst, "snapshot.snap.map." #n, t, &(snap.m.##n)); ROF(b); } while (0)
#define	dumpO(n,t)	do { b = TC_v2t (dst, "snapshot.oob.header." #n, t, &(hdr.##n)); ROF(b); } while (0)
#define	CdumpO(n,t)	do { b = TC_v2t (dst, "//snapshot.oob.header." #n, t, &(hdr.##n)); ROF(b); } while (0)
#define	dumpOF(n,t,i)do { b = TC_v2t (dst, "snapshot.oob.fel." #n, t, &(f.##n), 0, i); ROF(b); } while (0)
#define	dumpOU(n,t,i)do { b = TC_v2t (dst, "snapshot.oob.uel." #n, t, &(u.##n), 0, i); ROF(b); } while (0)
#define	dumpOL(n,t,i)do { b = TC_v2t (dst, "snapshot.oob.lel." #n, t, &(l.##n), 0, i); ROF(b); } while (0)
#define	dumpOP(n,t,i)do { b = TC_v2t (dst, "snapshot.oob.pel." #n, t, &(p.##n), 0, i); ROF(b); } while (0)

static SPWAW_ERROR
snapexport_hdrs (int src, FILE *dst, SNAP_HEADER &mhdr, SNAP_SOURCE &shdr, SNAP_INFO &ihdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		p0;
	bool		b;

	memset (&mhdr, 0, sizeof (mhdr));
	memset (&shdr, 0, sizeof (shdr));
	memset (&ihdr, 0, sizeof (ihdr));

	p0 = bseekget (src);

	if (!bread (src, (char *)&mhdr, sizeof (mhdr), false)) rc = SPWERR_FRFAILED;
	ERRORGOTO ("bread(header)", handle_error);

	b = TC_v2t (dst, "snapshot.header.magic", TC_DATA_ASCIIZ, &(mhdr.magic), sizeof (mhdr.magic)); ROF(b);
	dumpH (version,	TC_DATA_ULONG);
	CdumpH (src,	TC_DATA_ULONG);
	CdumpH (info,	TC_DATA_ULONG);
	CdumpH (snap,	TC_DATA_ULONG);
	CdumpH (oobp1,	TC_DATA_ULONG);
	CdumpH (oobp2,	TC_DATA_ULONG);
	CdumpH (stab,	TC_DATA_ULONG);
		
	bseekset (src, mhdr.src + p0);
	if (!bread (src, (char *)&shdr, sizeof (shdr), false)) rc = SPWERR_FRFAILED;
	ERRORGOTO ("bread(source)", handle_error);

	dumpS (path,	TC_DATA_ULONG);
	dumpS (file,	TC_DATA_ULONG);
	dumpS (date,	TC_DATA_TIMESTAMP);

	bseekset (src, mhdr.info + p0);
	if (!bread (src, (char *)&ihdr, sizeof (ihdr), false)) rc = SPWERR_FRFAILED;
	ERRORGOTO ("bread(info)", handle_error);

	dumpI (title,		TC_DATA_ULONG);
	dumpI (start,		TC_DATA_TIMESTAMP);
	dumpI (date,		TC_DATA_TIMESTAMP);
	dumpI (turn,		TC_DATA_ULONG);
	dumpI (location,	TC_DATA_ULONG);

	return (SPWERR_OK);

handle_error:
	return (rc);
}

static SPWAW_ERROR
snapexport_snap (int src, FILE *dst, long pos)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SNAP		snap;
	bool		b;

	bseekset (src, pos);
	if (!bread (src, (char *)&snap, sizeof (snap), false)) rc = SPWERR_FRFAILED;
	ERRORGOTO ("bread(snap)", handle_error);

	dumpCMT (title,		TC_DATA_ULONG);
	dumpCMT (mapsrc,	TC_DATA_ULONG);
	
	dumpB (year,		TC_DATA_BYTE);
	dumpB (month,		TC_DATA_BYTE);
	dumpB (day,		TC_DATA_BYTE);
	dumpB (hour,		TC_DATA_BYTE);
	dumpB (location,	TC_DATA_ULONG);
	dumpB (terrain,		TC_DATA_BYTE);
	dumpB (weather,		TC_DATA_BYTE);
	dumpB (visibility,	TC_DATA_BYTE);
	dumpB (turn,		TC_DATA_BYTE);
	dumpB (turn_max,	TC_DATA_BYTE);
	dumpB (OOB_p1,		TC_DATA_BYTE);
	dumpB (OOB_p2,		TC_DATA_BYTE);
	dumpB (OOB_p3,		TC_DATA_BYTE);
	dumpB (OOB_p4,		TC_DATA_BYTE);
	dumpB (miss_p1,		TC_DATA_BYTE);
	dumpB (miss_p2,		TC_DATA_BYTE);
	dumpB (credit,		TC_DATA_DWORD);

	dumpC (campaign,	TC_DATA_BYTE);
	dumpC (start_year,	TC_DATA_USHORT);
	dumpC (start_month,	TC_DATA_USHORT);
	dumpC (end_year,	TC_DATA_USHORT);
	dumpC (end_month,	TC_DATA_USHORT);
	dumpC (battles,		TC_DATA_USHORT);
	dumpC (majvics,		TC_DATA_USHORT);
	dumpC (minvics,		TC_DATA_USHORT);
	dumpC (battles_max,	TC_DATA_USHORT);

	dumpC (P1BLmen,		TC_DATA_USHORT);
	dumpC (P1BLart,		TC_DATA_USHORT);
	dumpC (P1BLsoft,	TC_DATA_USHORT);
	dumpC (P1BLapc,		TC_DATA_USHORT);
	dumpC (P1BLafv,		TC_DATA_USHORT);
	dumpC (P1BLgliders,	TC_DATA_USHORT);
	dumpC (P1BLair,		TC_DATA_USHORT);
	dumpC (P2BLmen,		TC_DATA_USHORT);
	dumpC (P2BLart,		TC_DATA_USHORT);
	dumpC (P2BLsoft,	TC_DATA_USHORT);
	dumpC (P2BLapc,		TC_DATA_USHORT);
	dumpC (P2BLafv,		TC_DATA_USHORT);
	dumpC (P2BLgliders,	TC_DATA_USHORT);
	dumpC (P2BLair,		TC_DATA_USHORT);
	dumpC (P1TLmen,		TC_DATA_USHORT);
	dumpC (P1TLart,		TC_DATA_USHORT);
	dumpC (P1TLsoft,	TC_DATA_USHORT);
	dumpC (P1TLapc,		TC_DATA_USHORT);
	dumpC (P1TLafv,		TC_DATA_USHORT);
	dumpC (P1TLgliders,	TC_DATA_USHORT);
	dumpC (P1TLair,		TC_DATA_USHORT);
	dumpC (P2TLmen,		TC_DATA_USHORT);
	dumpC (P2TLart,		TC_DATA_USHORT);
	dumpC (P2TLsoft,	TC_DATA_USHORT);
	dumpC (P2TLapc,		TC_DATA_USHORT);
	dumpC (P2TLafv,		TC_DATA_USHORT);
	dumpC (P2TLgliders,	TC_DATA_USHORT);
	dumpC (P2TLair,		TC_DATA_USHORT);
	dumpC (P1score,		TC_DATA_DWORD);
	dumpC (P2score,		TC_DATA_DWORD);

	//dumpM (width,		TC_DATA_DWORD);
	//dumpM (height,		TC_DATA_DWORD);

	return (SPWERR_OK);

handle_error:
	return (rc);

}

static SPWAW_ERROR
snapexport_oob (int src, FILE *dst, long pos)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SNAP_OOBHDR	hdr;
	USHORT		i;
	SNAP_OOB_FEL	f;
	SNAP_OOB_UEL	u;
	SNAP_OOB_LEL	l;
	SNAP_OOB_PEL	p;
	bool		b;

	bseekset (src, pos);
	if (!bread (src, (char *)&hdr, sizeof (hdr), false)) rc = SPWERR_FRFAILED;
	ERRORGOTO ("bread(oob header)", handle_error);

	dumpO (fcnt,	TC_DATA_USHORT);
	CdumpO (fpos,	TC_DATA_ULONG);
	dumpO (fstart,	TC_DATA_USHORT);
	dumpO (ucnt,	TC_DATA_USHORT);
	CdumpO (upos,	TC_DATA_ULONG);
	dumpO (lcnt,	TC_DATA_USHORT);
	CdumpO (lpos,	TC_DATA_ULONG);
	dumpO (pcnt,	TC_DATA_USHORT);
	CdumpO (ppos,	TC_DATA_ULONG);

	for (i=0; i<hdr.fcnt; i++) {
		if (!bread (src, (char *)&f, sizeof (f), false)) rc = SPWERR_FRFAILED;
		ERRORGOTO ("bread(oob formation element)", handle_error);

		dumpOF (RID,	TC_DATA_IDX_USHORT,	i);
		dumpOF (FID,	TC_DATA_IDX_BYTE,	i);
		dumpOF (name,	TC_DATA_IDX_ULONG,	i);
		dumpOF (leader,	TC_DATA_IDX_USHORT,	i);
		dumpOF (hcmd,	TC_DATA_IDX_USHORT,	i);
		dumpOF (OOBrid,	TC_DATA_IDX_BYTE,	i);
		dumpOF (status,	TC_DATA_IDX_BYTE,	i);
	}

	for (i=0; i<hdr.ucnt; i++) {
		if (!bread (src, (char *)&u, sizeof (u), false)) rc = SPWERR_FRFAILED;
		ERRORGOTO ("bread(oob unit element)", handle_error);

		dumpOU (RID,		TC_DATA_IDX_USHORT,	i);
		dumpOU (FMID,		TC_DATA_IDX_BYTE,	i);
		dumpOU (FSID,		TC_DATA_IDX_BYTE,	i);
		dumpOU (name,		TC_DATA_IDX_ULONG,	i);
		dumpOU (classID,	TC_DATA_IDX_BYTE,	i);
		dumpOU (OOB,		TC_DATA_IDX_BYTE,	i);
		dumpOU (OOBrid,		TC_DATA_IDX_BYTE,	i);
		dumpOU (size,		TC_DATA_IDX_BYTE,	i);
		dumpOU (cost,		TC_DATA_IDX_BYTE,	i);
		dumpOU (survive,	TC_DATA_IDX_BYTE,	i);
		dumpOU (leader,		TC_DATA_IDX_USHORT,	i);
		dumpOU (exp,		TC_DATA_IDX_BYTE,	i);
		dumpOU (mor,		TC_DATA_IDX_BYTE,	i);
		dumpOU (sup,		TC_DATA_IDX_BYTE,	i);
		dumpOU (status,		TC_DATA_IDX_BYTE,	i);
		dumpOU (entr,		TC_DATA_IDX_BYTE,	i);
		dumpOU (smkdev,		TC_DATA_IDX_BYTE,	i);
		dumpOU (smkammo,	TC_DATA_IDX_BYTE,	i);
		dumpOU (crew,		TC_DATA_IDX_USHORT,	i);
		dumpOU (range,		TC_DATA_IDX_BYTE,	i);
		dumpOU (stance_x,	TC_DATA_IDX_BYTE,	i);
		dumpOU (stance_y,	TC_DATA_IDX_BYTE,	i);
		dumpOU (loader,		TC_DATA_IDX_USHORT,	i);
		dumpOU (load_cap,	TC_DATA_IDX_BYTE,	i);
		dumpOU (load_cost,	TC_DATA_IDX_BYTE,	i);
		dumpOU (radio,		TC_DATA_IDX_BYTE,	i);
		dumpOU (rof,		TC_DATA_IDX_BYTE,	i);
		dumpOU (tgt,		TC_DATA_IDX_BYTE,	i);
		dumpOU (rf,		TC_DATA_IDX_BYTE,	i);
		dumpOU (fc,		TC_DATA_IDX_BYTE,	i);
		dumpOU (iv,		TC_DATA_IDX_BYTE,	i);
		dumpOU (swim,		TC_DATA_IDX_BYTE,	i);
		dumpOU (men,		TC_DATA_IDX_BYTE,	i);
		dumpOU (men_ori,	TC_DATA_IDX_BYTE,	i);
		dumpOU (speed,		TC_DATA_IDX_BYTE,	i);
		dumpOU (moves,		TC_DATA_IDX_BYTE,	i);
		dumpOU (damage,		TC_DATA_IDX_USHORT,	i);
		dumpOU (movdir,		TC_DATA_IDX_USHORT,	i);
		dumpOU (shtdir,		TC_DATA_IDX_USHORT,	i);
		dumpOU (target,		TC_DATA_IDX_USHORT,	i);
		dumpOU (UTGidx,		TC_DATA_IDX_BYTE,	i);
	}

	for (i=0; i<hdr.lcnt; i++) {
		if (!bread (src, (char *)&l, sizeof (l), false)) rc = SPWERR_FRFAILED;
		ERRORGOTO ("bread(oob leader element)", handle_error);

		dumpOL (RID,	TC_DATA_IDX_USHORT,	i);
		dumpOL (URID,	TC_DATA_IDX_USHORT,	i);
		dumpOL (name,	TC_DATA_IDX_ULONG,	i);
		dumpOL (rank,	TC_DATA_IDX_BYTE,	i);
		dumpOL (ral,	TC_DATA_IDX_BYTE,	i);
		dumpOL (inf,	TC_DATA_IDX_BYTE,	i);
		dumpOL (art,	TC_DATA_IDX_BYTE,	i);
		dumpOL (arm,	TC_DATA_IDX_BYTE,	i);
		dumpOL (kills,	TC_DATA_IDX_BYTE,	i);
		dumpOL (status,	TC_DATA_IDX_BYTE,	i);
	}

	for (i=0; i<hdr.pcnt; i++) {
		if (!bread (src, (char *)&p, sizeof (p), false)) rc = SPWERR_FRFAILED;
		ERRORGOTO ("bread(oob position element)", handle_error);

		dumpOP (RID,	TC_DATA_IDX_USHORT,	i);
		dumpOP (URID,	TC_DATA_IDX_USHORT,	i);
		dumpOP (x,	TC_DATA_IDX_USHORT,	i);
		dumpOP (y,	TC_DATA_IDX_USHORT,	i);
		dumpOP (seen,	TC_DATA_IDX_BYTE,	i);
	}

	return (SPWERR_OK);

handle_error:
	return (rc);

}

SPWAW_ERROR
snapexport (int src, FILE *dst)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		pos;
	SNAP_HEADER	mhdr;
	SNAP_SOURCE	shdr;
	SNAP_INFO	ihdr;

	CNULLARG (dst);

	pos = bseekget (src);

	rc = snapexport_hdrs (src, dst, mhdr, shdr, ihdr);
	ERRORGOTO ("snapexport_hdrs()", handle_error);

	if (mhdr.stab) {
		bseekset (src, pos + mhdr.stab);
		rc = STRTAB_export (src, dst);
		ERRORGOTO ("STRTAB_export()", handle_error);
	}

	rc = snapexport_snap (src, dst, pos + mhdr.snap.data);
	ERRORGOTO ("snapexport_snap()", handle_error);

	rc = snapexport_oob (src, dst, pos + mhdr.oobp1);
	ERRORGOTO ("snapexport_oob()", handle_error);

	rc = snapexport_oob (src, dst, pos + mhdr.oobp2);
	ERRORGOTO ("snapexport_oob()", handle_error);

	return (SPWERR_OK);

handle_error:
	return (rc);
}
