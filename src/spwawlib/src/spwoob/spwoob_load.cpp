#include "stdafx.h"
#include <spwawlib_api.h>
#include "spwoob/spwoob.h"
#include "spwoob/raw.h"
#include "spwoob/spwoob_file.h"
#include "fileio/fileio.h"
#include "common/internal.h"
#include "utils/compression.h"

#define	BADOOBID	((BYTE)-1)

static BYTE
name2id (const char *name)
{
	char	local[16];
	int	id;
	int	rc;

	if (strlen (name) > 6) return (BADOOBID);
	if (strnicmp (name, "oob", 3) != 0) return (BADOOBID);

	memset (local, 0, sizeof (local));
	snprintf (local, sizeof (local) - 1, "%s", name);

	local[0] = (char)tolower (local[0]);
	local[1] = (char)tolower (local[1]);
	local[2] = (char)tolower (local[2]);

	rc = sscanf (local, "oob%d", &id);
	if (rc <= 0) id = BADOOBID;
	if ((id < 0) || (id > 255)) id = BADOOBID;

	return ((BYTE)id);
}

static SPWOOB_UCLASS
utype2class (SPWOOB_UTYPE ut)
{
	SPWOOB_UCLASS	uc = SPWOOB_UCLASS_OTH;

	switch (ut) {
		case SPWOOB_UTYPE_INF:
		case SPWOOB_UTYPE_LT_INF:
		case SPWOOB_UTYPE_MD_INF:
		case SPWOOB_UTYPE_HV_INF:
		case SPWOOB_UTYPE_MOT_INF:
		case SPWOOB_UTYPE_ARM_INF:
		case SPWOOB_UTYPE_ENG:
		case SPWOOB_UTYPE_MOT_ENG:
		case SPWOOB_UTYPE_ARM_ENG:
		case SPWOOB_UTYPE_MG:
		case SPWOOB_UTYPE_HV_MG:
		case SPWOOB_UTYPE_SKI:
		case SPWOOB_UTYPE_AIR_INF:
		case SPWOOB_UTYPE_BCYCLE:
		case SPWOOB_UTYPE_ELITE:
		case SPWOOB_UTYPE_SNIPER:
		case SPWOOB_UTYPE_SPECOP:
		case SPWOOB_UTYPE_GUERILLA:
		case SPWOOB_UTYPE_CONSCR:
		case SPWOOB_UTYPE_SLINF:
			uc = SPWOOB_UCLASS_INF;
			break;
		case SPWOOB_UTYPE_CAV:
			uc = SPWOOB_UCLASS_CAV;
			break;
		case SPWOOB_UTYPE_AHT:
		case SPWOOB_UTYPE_LT_TNK:
		case SPWOOB_UTYPE_MD_TNK:
		case SPWOOB_UTYPE_HV_TNK:
		case SPWOOB_UTYPE_VHV_TNK:
		case SPWOOB_UTYPE_MB_TNK:
		case SPWOOB_UTYPE_CR_TNK:
		case SPWOOB_UTYPE_CS_TNK:
		case SPWOOB_UTYPE_INF_TNK:
		case SPWOOB_UTYPE_MINECLR_TNK:
		case SPWOOB_UTYPE_ENG_TNK:
		case SPWOOB_UTYPE_FLM_TNK:
		case SPWOOB_UTYPE_SPEC_TNK:
		case SPWOOB_UTYPE_LL_TNK:
		case SPWOOB_UTYPE_CAPT_TNK:
		case SPWOOB_UTYPE_ASSAULT:
			uc = SPWOOB_UCLASS_ARM;
			break;
		case SPWOOB_UTYPE_HT:
		case SPWOOB_UTYPE_WH_APC:
		case SPWOOB_UTYPE_TR_APC:
		case SPWOOB_UTYPE_HV_TR_APC:
			uc = SPWOOB_UCLASS_APC;
			break;
		case SPWOOB_UTYPE_LT_MORT:
		case SPWOOB_UTYPE_MORT:
		case SPWOOB_UTYPE_HV_MORT:
		case SPWOOB_UTYPE_SP_MORT:
		case SPWOOB_UTYPE_SP_GUN:
		case SPWOOB_UTYPE_INF_GUN:
		case SPWOOB_UTYPE_HWTZR:
		case SPWOOB_UTYPE_SP_ART:
		case SPWOOB_UTYPE_OB_ART:
		case SPWOOB_UTYPE_RKT:
		case SPWOOB_UTYPE_SP_RKT:
		case SPWOOB_UTYPE_OB_RKT:
		case SPWOOB_UTYPE_CAPT_SPG:
			uc = SPWOOB_UCLASS_ART;
			break;
		case SPWOOB_UTYPE_INF_AT:
		case SPWOOB_UTYPE_AT:
		case SPWOOB_UTYPE_WH_AT:
		case SPWOOB_UTYPE_SP_AT:
		case SPWOOB_UTYPE_TD:
		case SPWOOB_UTYPE_HV_TD:
		case SPWOOB_UTYPE_TTD:
			uc = SPWOOB_UCLASS_AT;
			break;
		case SPWOOB_UTYPE_FLAK:
		case SPWOOB_UTYPE_HV_FLAK:
		case SPWOOB_UTYPE_WH_FLAK:
		case SPWOOB_UTYPE_SP_FLAK:
			uc = SPWOOB_UCLASS_AA;
			break;
		case SPWOOB_UTYPE_AMPHIB:
		case SPWOOB_UTYPE_HV_AMPHIB:
		case SPWOOB_UTYPE_AMPHIB_TNK:
		case SPWOOB_UTYPE_MRNS:
			uc = SPWOOB_UCLASS_MAR;
			break;
		case SPWOOB_UTYPE_SCOUT:
		case SPWOOB_UTYPE_VEH_SCOUT:
		case SPWOOB_UTYPE_APC_SCOUT:
		case SPWOOB_UTYPE_FO:
		case SPWOOB_UTYPE_VEH_FO:
		case SPWOOB_UTYPE_TR_FO:
		case SPWOOB_UTYPE_MCYCLE:
		case SPWOOB_UTYPE_RHT:
		case SPWOOB_UTYPE_LT_ARMCAR:
		case SPWOOB_UTYPE_ARMCAR:
		case SPWOOB_UTYPE_HV_ARMCAR:
		case SPWOOB_UTYPE_RE_TNK:
			uc = SPWOOB_UCLASS_REC;
			break;
		case SPWOOB_UTYPE_HQ:
		case SPWOOB_UTYPE_PLT_HQ:
		case SPWOOB_UTYPE_CC_TNK:
			uc = SPWOOB_UCLASS_CC;
			break;
		case SPWOOB_UTYPE_PRMV:
		case SPWOOB_UTYPE_MD_TRK:
		case SPWOOB_UTYPE_HV_TRK:
		case SPWOOB_UTYPE_BCARR:
		case SPWOOB_UTYPE_WAGON:
		case SPWOOB_UTYPE_TEAM:
		case SPWOOB_UTYPE_GTEAM:
		case SPWOOB_UTYPE_PACK:
			uc = SPWOOB_UCLASS_TRN;
			break;
		case SPWOOB_UTYPE_AIR:
		case SPWOOB_UTYPE_AIRB:
		case SPWOOB_UTYPE_CRG_AIR:
		case SPWOOB_UTYPE_GLIDER:
		case SPWOOB_UTYPE_HELI:
		case SPWOOB_UTYPE_AHELI:
		case SPWOOB_UTYPE_OHELI:
			uc = SPWOOB_UCLASS_AIR;
			break;
		case SPWOOB_UTYPE_BARGE:
		case SPWOOB_UTYPE_ABOAT:
		case SPWOOB_UTYPE_LCS:
		case SPWOOB_UTYPE_LCG:
		case SPWOOB_UTYPE_SHIP:
		case SPWOOB_UTYPE_NAVB:
			uc = SPWOOB_UCLASS_NVY;
			break;
		case SPWOOB_UTYPE_FORT:
		case SPWOOB_UTYPE_EMP_FORT:
		case SPWOOB_UTYPE_NAV_FORT:
			uc = SPWOOB_UCLASS_FRT;
			break;
		case SPWOOB_UTYPE_UTIL:
		case SPWOOB_UTYPE_CREW:
		case SPWOOB_UTYPE_AMMOC:
		case SPWOOB_UTYPE_AMMOD:
		case SPWOOB_UTYPE_CAVE:
		default:
			uc = SPWOOB_UCLASS_OTH;
			break;
	}
	return (uc);
}

static SPWAW_ERROR
load_oob_data (SPWOOB_DATA *dst)
{
	SPWAW_ERROR	rc;
	RAWOOB		*raw;
	int		i;

	CNULLARG (dst); CNULLARG (dst->rdata);

	rc = zbuildcrc ((char *)dst->rdata, dst->rsize, &(dst->crc32));
	ROE ("zbuildcrc()");

	raw = (RAWOOB *)dst->rdata;

	for (i=0; i<SPWOOB_WCNT; i++) {
		if (raw->w.name[i].data[0] != '\0') {
			azstrcpy (raw->w.name[i].data, dst->wdata[i].name);
			dst->wdata[i].wclass		= SPWOOB_WCLASS_xlt (raw->w.wclass[i]);
			dst->wdata[i].size		= raw->w.size[i];
			dst->wdata[i].warhead		= raw->w.warhead[i];
			dst->wdata[i].kill		= raw->w.kill[i];
			dst->wdata[i].pen_AP		= raw->w.pen[i].AP;
			dst->wdata[i].pen_HE		= raw->w.pen[i].HE;
			dst->wdata[i].pen_HEAT		= raw->w.pen_HEAT[i];
			dst->wdata[i].pen_APCR		= raw->w.pen_APCR[i];
			dst->wdata[i].accuracy		= raw->w.accuracy[i];
			dst->wdata[i].range_max		= raw->w.rng_max[i];
			dst->wdata[i].range_APCR	= raw->w.rng_APCR[i];
			dst->wdata[i].valid		= true;
		}
	}

	for (i=0; i<SPWOOB_UCNT; i++) {
		if (raw->u.name[i].data[0] != '\0') {
			azstrcpy (raw->u.name[i].data, dst->udata[i].name);
			dst->udata[i].nation		= raw->u.nation[i];
			dst->udata[i].type		= SPWOOB_UTYPE_xlt (raw->u.uclass[i]);
			dst->udata[i].uclass		= utype2class (dst->udata[i].type);
			dst->udata[i].start_yr		= raw->u.start_yr[i] + 1900;
			dst->udata[i].start_mo		= raw->u.start_mo[i];
			dst->udata[i].end_yr		= raw->u.end_yr[i] + 1900;
			dst->udata[i].end_mo		= raw->u.end_mo[i];
			dst->udata[i].size		= raw->u.size[i];
			dst->udata[i].crew		= raw->u.crew[i];
			dst->udata[i].survive		= raw->u.survive[i];
			dst->udata[i].cost		= raw->u.cost[i];
			dst->udata[i].speed		= raw->u.speed[i];
			//dst->udata[i].mclass		= SPWOOB_MOVCL_xlt (raw->u.movcl[i]);
			dst->udata[i].mclass		= SPWOOB_MOVCL_xlt (0);		//FIXME
			dst->udata[i].radio		= raw->u.radio[i];
			dst->udata[i].irvis		= raw->u.irvis[i];
			dst->udata[i].fc		= raw->u.fc[i];
			dst->udata[i].rf		= raw->u.rf[i];
			dst->udata[i].stab		= raw->u.stab[i];
			dst->udata[i].rof		= raw->u.rof[i];
			dst->udata[i].load_cap		= raw->u.load_cap[i];
			dst->udata[i].load_cost		= raw->u.load_cost[i];
			dst->udata[i].swim		= raw->u.swim[i];
			dst->udata[i].smkdev		= raw->u.smkdev[i];
			dst->udata[i].wpn1		= raw->u.wpn[i].w1;
			dst->udata[i].wpn1_HEammo	= raw->u.wpn_ammo[i].HE1;
			dst->udata[i].wpn1_APammo	= raw->u.wpn_ammo[i].AP1;
			dst->udata[i].wpn1_HEATammo	= raw->u.wpn1_HEATammo[i];
			dst->udata[i].wpn1_APCRammo	= raw->u.wpn1_APCRammo[i];
			dst->udata[i].wpn2		= raw->u.wpn[i].w2;
			dst->udata[i].wpn2_HEammo	= raw->u.wpn_ammo[i].HE2;
			dst->udata[i].wpn2_APammo	= raw->u.wpn_ammo[i].AP2;
			dst->udata[i].wpn3		= raw->u.wpn[i].w3;
			dst->udata[i].wpn3_HEammo	= raw->u.wpn_ammo[i].HE3;
			dst->udata[i].wpn3_APammo	= raw->u.wpn_ammo[i].AP3;
			dst->udata[i].wpn4		= raw->u.wpn[i].w4;
			dst->udata[i].wpn4_HEammo	= raw->u.wpn_ammo[i].HE4;
			dst->udata[i].wpn4_APammo	= raw->u.wpn_ammo[i].AP4;
			dst->udata[i].arm_FH		= raw->u.arm[i].FH;
			dst->udata[i].arm_SH		= raw->u.arm[i].SH;
			dst->udata[i].arm_RH		= raw->u.arm[i].RH;
			dst->udata[i].arm_FT		= raw->u.arm[i].FT;
			dst->udata[i].arm_ST		= raw->u.arm[i].ST;
			dst->udata[i].arm_RT		= raw->u.arm[i].RT;
			dst->udata[i].arm_TP		= raw->u.arm[i].TP;
			dst->udata[i].arm_SK		= raw->u.arm_SK[i];
			dst->udata[i].slp_FH		= raw->u.slp[i].FH;
			dst->udata[i].slp_SH		= raw->u.slp[i].SH;
			dst->udata[i].slp_RH		= raw->u.slp[i].RH;
			dst->udata[i].slp_FT		= raw->u.slp[i].FT;
			dst->udata[i].slp_ST		= raw->u.slp[i].ST;
			dst->udata[i].slp_RT		= raw->u.slp[i].RT;
			dst->udata[i].valid		= true;
		}
	}

	for (i=0; i<SPWOOB_FCNT; i++) {
		if (raw->f.name[i].data[0] != '\0') {
			azstrcpy (raw->f.name[i].data, dst->fdata[i].name);
			dst->fdata[i].nation		= raw->f.nation[i];
			dst->fdata[i].stat		= SPWOOB_FSTAT_xlt (raw->f.special[i]);
			dst->fdata[i].type		= SPWOOB_FTYPE_xlt (raw->f.type[i]);
			dst->fdata[i].pscr		= SPWOOB_FPSCR_xlt (raw->f.purchscrn[i]);
			dst->fdata[i].start_yr		= raw->f.start_yr[i] + 1900;
			dst->fdata[i].start_mo		= raw->f.start_mo[i];
			dst->fdata[i].end_yr		= raw->f.end_yr[i] + 1900;
			dst->fdata[i].unit_ids[0]	= raw->f.urid[i].dat[0];
			dst->fdata[i].unit_ids[1]	= raw->f.urid[i].dat[1];
			dst->fdata[i].unit_ids[2]	= raw->f.urid[i].dat[2];
			dst->fdata[i].unit_ids[3]	= raw->f.urid[i].dat[3];
			dst->fdata[i].unit_ids[4]	= raw->f.urid[i].dat[4];
			dst->fdata[i].unit_ids[5]	= raw->f.urid[i].dat[5];
			dst->fdata[i].unit_ids[6]	= raw->f.urid[i].dat[6];
			dst->fdata[i].unit_ids[7]	= raw->f.urid[i].dat[7];
			dst->fdata[i].unit_ids[8]	= raw->f.urid[i].dat[8];
			dst->fdata[i].unit_ids[9]	= raw->f.urid[i].dat[9];
			dst->fdata[i].unit_cnt[0]	= raw->f.ucnt[i].dat[0];
			dst->fdata[i].unit_cnt[1]	= raw->f.ucnt[i].dat[1];
			dst->fdata[i].unit_cnt[2]	= raw->f.ucnt[i].dat[2];
			dst->fdata[i].unit_cnt[3]	= raw->f.ucnt[i].dat[3];
			dst->fdata[i].unit_cnt[4]	= raw->f.ucnt[i].dat[4];
			dst->fdata[i].unit_cnt[5]	= raw->f.ucnt[i].dat[5];
			dst->fdata[i].unit_cnt[6]	= raw->f.ucnt[i].dat[6];
			dst->fdata[i].unit_cnt[7]	= raw->f.ucnt[i].dat[7];
			dst->fdata[i].unit_cnt[8]	= raw->f.ucnt[i].dat[8];
			dst->fdata[i].unit_cnt[9]	= raw->f.ucnt[i].dat[9];
			dst->fdata[i].valid		= true;
		}
	}

	return (SPWERR_OK);
}

static SPWAW_ERROR
load_oob_file (SPWOOB *oob, BYTE id, const char *file)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWOOB_DATA	*dst;
	char		path[MAX_PATH+1];
	int		fd = -1;
	HANDLE		maph;
	RAWOOB		*raw = NULL;

	CNULLARG (oob); CNULLARG (file);
	if (oob->data[id]) return (SPWERR_OK);

	dst = safe_malloc (SPWOOB_DATA); COOMGOTO (dst, "SPWOOB_DATA", handle_error);
	dst->id = id;
	
	dst->rdata = safe_malloc (RAWOOB); COOMGOTO (dst, "RAWOOB", handle_error);
	dst->rsize = sizeof (RAWOOB);

	memset (path, 0, sizeof (path));
	snprintf (path, sizeof (path) - 1, "%s\\%s", oob->srcdir, file);

	fd = open (path, O_RDONLY|O_BINARY);
	if (fd < 0) FAILGOTO (SPWERR_FOFAILED, "open(oob file) failed", handle_error);

	raw = (RAWOOB *)mmapfile (fd, &maph);
	if (!raw) FAILGOTO (SPWERR_FRFAILED, "mmapfile(oob file) failed", handle_error);

	memcpy (dst->rdata, raw, dst->rsize);
	unmmapfile ((void **)&raw, &maph); raw = NULL;
	close (fd); fd = -1;

	rc = load_oob_data (dst); ERRORGOTO ("load_oob_data()", handle_error);

	oob->data[id] = dst;
	oob->count++;

	return (SPWERR_OK);

handle_error:
	if (dst) {
		if (dst->rdata) safe_free (dst->rdata);
		safe_free (dst);
	}
	if (raw) unmmapfile ((void **)&raw, &maph);
	if (fd != -1) close (fd);
	return (rc);
}

static SPWAW_ERROR
load_oob_files (SPWOOB *oob)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	char			glob[MAX_PATH+1];
	intptr_t		f_hndl = -1;
	int			f_stat;
	struct _finddata_t	f_data;
	BYTE			id;

	CNULLARG (oob);

	if (oob->count) return (SPWERR_OK);

	memset (glob, 0, sizeof (glob));
	snprintf (glob, sizeof (glob) - 1, "%s\\oob*", oob->srcdir);

	if ((f_hndl = _findfirst (glob, &f_data)) == -1)
		FAILGOTO (SPWERR_NOOOBFILES, "_findfirst() failed", handle_error);

	f_stat = 0;
	while (f_stat != -1) {
		id = name2id (f_data.name);
		if ((id != BADOOBID) && (oob->data[id] == NULL)) {
			rc = load_oob_file (oob, id, f_data.name);
			ERRORGOTO ("load_oob_data()", handle_error);
		}

		f_stat = _findnext (f_hndl, &f_data);
	}
	_findclose (f_hndl);

	if (oob->count == 0) RWE (SPWERR_NOOOBFILES, "no valid OOB files found");

	return (SPWERR_OK);

handle_error:
	if (f_hndl != -1) _findclose (f_hndl);
	return (rc);
}

SPWAW_ERROR
SPWOOB_load (SPWOOB *oob, const char *dir)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	CNULLARG (dir);

	snprintf (oob->srcdir, sizeof (oob->srcdir) - 1, "%s", dir);

	rc = load_oob_files (oob);

	return (rc);
}

SPWAW_ERROR
SPWOOB_load (SPWOOB *oob, int fd)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWOOB_HEADER	hdr;
	SPWOOB_OOBHDR	*ohdr = NULL;
	long		p0;
	BYTE		idx;
	SPWOOB_DATA	*p;
	CBIO		cbio;

	CNULLARG (oob);

	memset (&hdr, 0, sizeof (hdr));

	if (!bread (fd, (char *)&hdr, sizeof (hdr), false))
		RWE (SPWERR_FRFAILED, "bread(SPWOOB header) failed");
	if ((memcmp (hdr.magic, SPWOOB_MAGIC, SPWOOB_MGCLEN) != 0) || (hdr.version != SPWOOB_VERSION))
		RWE (SPWERR_BADSAVEDATA, "SPWOOB header check failed");

	oob->count = hdr.cnt;

	ohdr = safe_nmalloc (SPWOOB_OOBHDR, oob->count); COOMGOTO (ohdr, "SPWOOB_OOBDATA list", handle_error);

	p0 = bseekget (fd);

	if (!bread (fd, (char *)ohdr, oob->count * sizeof (SPWOOB_OOBHDR), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(SPWOOB data headers) failed", handle_error);

	for (idx=0; idx<oob->count; idx++) {
                p = safe_malloc (SPWOOB_DATA); COOMGOTO (p, "SPWOOB_DATA", handle_error);

		p->id	 = ohdr[idx].id;
		p->rsize = ohdr[idx].size;
		p->rdata = safe_smalloc (char, p->rsize); COOMGOTO (p->rdata, "raw SPWOOB data", handle_error);

		bseekset (fd, p0 + ohdr[idx].data);

		cbio.data = (char *)(p->rdata); cbio.size = p->rsize; cbio.comp = &(ohdr[idx].comp);
		if (!cbread (fd, cbio, "raw SPWOOB data"))
			FAILGOTO (SPWERR_FRFAILED, "cbread(raw SPWOOB data) failed", handle_error);

		rc = load_oob_data (p); ERRORGOTO ("load_oob_data()", handle_error);

		if (p->crc32 != ohdr[idx].crc32)
			FAILGOTO (SPWERR_FRFAILED, "SPWOOB data CRC check failed", handle_error);

		oob->data[p->id] = p;
	}

	safe_free (ohdr);

	return (SPWERR_OK);

handle_error:
	if (ohdr) safe_free (ohdr);
	spwoob_free (oob);
	return (rc);
}
