#include "stdafx.h"
#include <spwawlib_api.h>
#include "common/internal.h"
#include "fileio/fileio.h"

#define	ZMSG(str_)	((str_)->msg?(str_)->msg:"there is no zlib error message")

#define	BLOCK	16384

SPWAW_ERROR
zcompress (char *src, unsigned long slen, char **dst, unsigned long *dlen)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	z_stream	stream;
	z_stream	*str = &stream;
	int		zrc;
	unsigned long	bound;
	char		*p = NULL;

	CNULLARG (src); CNULLARG (dst); CNULLARG (dlen);
	*dst = NULL; *dlen = 0;

	if (slen == 0) RWE (SPWERR_FAILED, "unable to compress zero data");

	clear_ptr (str);
	str->zalloc = Z_NULL; str->zfree = Z_NULL;

	zrc = deflateInit (str, Z_BEST_COMPRESSION);
	if (zrc != Z_OK) {
		ERROR2 ("deflateInit() = %d (%s)", zrc, ZMSG(str));
		FAILGOTO (SPWERR_FAILED, "deflateInit() failed", handle_error);
	}

	bound = deflateBound (str, slen);
	p = safe_smalloc (char, bound); COOMGOTO (p, "deflate target buffer", handle_error);

	str->next_in	= (Bytef *)src;	str->avail_in	= slen;
	str->next_out	= (Bytef *)p;	str->avail_out	= bound;
	zrc = deflate (str, Z_FINISH);
	if (zrc != Z_STREAM_END) {
		ERROR2 ("deflate() = %d (%s)", zrc, ZMSG(str));
		FAILGOTO (SPWERR_FAILED, "deflate() failed", handle_error);
	}

	*dst = p;
	*dlen = str->total_out;

	zrc = deflateEnd (str);
	if (zrc != Z_OK) {
		/* error is logged but otherwise ignored */
		ERROR2 ("deflateEnd() = %d (%s)", zrc, ZMSG(str));
	}

	return (SPWERR_OK);

handle_error:
	if (p) safe_free (p);
	return (rc);
}

SPWAW_ERROR
zexpand (char *src, unsigned long slen, char *dst, unsigned long dlen)
{
	z_stream	stream;
	z_stream	*str = &stream;
	int		zrc;

	CNULLARG (src); CNULLARG (dst);

	if (slen == 0) RWE (SPWERR_FAILED, "unable to decompress zero data");
	if (dlen == 0) RWE (SPWERR_FAILED, "unable to decompress data to zero-length buffer");

	clear_ptr (str);
	str->zalloc = Z_NULL; str->zfree = Z_NULL;

	str->next_in	= (Bytef *)src;	str->avail_in	= slen;
	zrc = inflateInit (str);
	if (zrc != Z_OK) {
		ERROR2 ("inflateInit() = %d (%s)", zrc, ZMSG(str));
		RWE (SPWERR_FAILED, "inflateInit() failed");
	}

	str->next_out	= (Bytef *)dst;	str->avail_out	= dlen;
	zrc = inflate (str, Z_FINISH);
	if (zrc != Z_STREAM_END) {
		ERROR2 ("inflate() = %d (%s)", zrc, ZMSG(str));
		RWE (SPWERR_FAILED, "inflate() failed");
	}

	zrc = inflateEnd (str);
	if (zrc != Z_OK) {
		/* error is logged but otherwise ignored */
		ERROR2 ("inflateEnd() = %d (%s)", zrc, ZMSG(str));
	}

	return (SPWERR_OK);
}

SPWAW_ERROR
zbuildcrc (int fd, unsigned long spos, unsigned long fpos, unsigned long *crc)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	uLong		crcv;
	unsigned long	p0 = 0;
	char		buf[BLOCK];
	unsigned long	left;
	unsigned int	todo;
	int		done;

	CNULLARG (crc);
	*crc = 0;

	p0 = bseekget (fd);

	crcv = crc32 (0, Z_NULL, 0);
	if ((fd > -1) && (spos < fpos)) {
		bseekset (fd, spos);

		left = fpos - spos;
		while (left) {
			todo = BLOCK; if (todo > left) todo = left;
			done = read (fd, buf, todo);
			if (done != (int)todo) FAILGOTO (SPWERR_FRFAILED, "read() failed", handle_error);
			crcv = crc32 (crcv, (const Bytef *)buf, done);
			left -= done;
		}
		bseekset (fd, p0);
	}
	*crc = crcv;

	return (SPWERR_OK);

handle_error:
	bseekset (fd, p0);
	return (rc);
}

SPWAW_ERROR
zbuildcrc (char *buf, unsigned long len, unsigned long *crc)
{
	uLong	crcv;

	CNULLARG (crc);

	crcv = crc32 (0, Z_NULL, 0);
	if (buf && len) crcv = crc32 (crcv, (const Bytef *)buf, len);
	*crc = crcv;

	return (SPWERR_OK);
}
