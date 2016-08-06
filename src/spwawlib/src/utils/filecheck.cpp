#include "stdafx.h"
#include <spwawlib_api.h>
#include "common/internal.h"
#include "fileio/fileio.h"
#include "utils/compression.h"

#define	FCHK_MAGIC	"SPWAWLIB_FILECHECK"
#define	FCHK_MGCLEN	20
#define	FCHK_VERSION	1

#pragma pack(push, r1, 1)

typedef struct s_FCHK_INFO {
	ULONG	fsize;			/* Total file size, including INFO and HEADER headers	*/
} FCHK_INFO;

typedef struct s_FCHK_HEADER {
	ULONG	fcrc32;			/* CRC of file data, excluding this header		*/
	ULONG	version;		/* Filecheck file format version			*/
	char	magic[FCHK_MGCLEN];	/* Filecheck magic string				*/
} FCHK_HEADER;

#pragma pack(pop, r1)

SPWAW_ERROR
fcheck_make (int fd)
{
	FCHK_INFO	info;
	FCHK_HEADER	hdr;
	long		fpos;
	SPWAW_ERROR	rc;

	memset (&info, 0, sizeof (info));
	
	info.fsize = _filelength(fd) + sizeof (info) + sizeof (hdr);

	memset (&hdr, 0, sizeof (hdr));

	memcpy (hdr.magic, FCHK_MAGIC, strlen (FCHK_MAGIC));
	hdr.version = FCHK_VERSION;

       	if (!bwrite (fd, (char *)&info, sizeof (info)))
		RWE (SPWERR_FWFAILED, "bwrite(FCHK_INFO header) failed");

	fpos = bseekget (fd);
	rc = zbuildcrc (fd, 0, fpos, &(hdr.fcrc32)); ROE ("buildcrc()");

	if (!bwrite (fd, (char *)&hdr, sizeof (hdr)))
		RWE (SPWERR_FWFAILED, "bwrite(FCHK_HEADER header) failed");
	
	return (SPWERR_OK);
}

SPWAW_ERROR
fcheck_verify (int fd)
{
	FCHK_INFO	info;
	FCHK_HEADER	hdr;
	long		flen, fpos;
	ULONG		crc32;
	SPWAW_ERROR	rc;

	flen = _filelength(fd);
	if (flen <= (sizeof (info) + sizeof (hdr)))
		RWE (SPWERR_CORRUPT, "detected truncated file missing data");

	fpos = flen - sizeof (hdr);

	memset (&hdr, 0, sizeof (hdr));

	bseekset (fd, fpos);
	if (!bread (fd, (char *)&hdr, sizeof (hdr), false))
		RWE (SPWERR_FRFAILED, "bread(FCHK_HEADER header) failed");
	if ((memcmp (hdr.magic, FCHK_MAGIC, FCHK_MGCLEN) != 0) || (hdr.version != FCHK_VERSION))
		RWE (SPWERR_CORRUPT, "FCHK_HEADER header check failed");

	bseekset (fd, fpos - sizeof (info));
	if (!bread (fd, (char *)&info, sizeof (info), false))
		RWE (SPWERR_FRFAILED, "bread(FCHK_INFO header) failed");

	if (info.fsize != (ULONG)flen) /* flen is not negative, see first check on flen */
		RWE (SPWERR_CORRUPT, "file size check failed");

	rc = zbuildcrc (fd, 0, fpos, &crc32); ROE ("zbuildcrc");
	if (hdr.fcrc32 != crc32)
		RWE (SPWERR_CORRUPT, "file CRC32 check failed");

	bseekset (fd, 0);

	return (SPWERR_OK);
}