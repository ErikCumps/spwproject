/** \file
 * The SPWaW Library - file I/O handling.
 *
 * Copyright (C) 2007-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/gamefile.h"
#include "fileio/fileio.h"
#include "utils/log.h"
#include "common/internal.h"

/* This is the SPWaW savegame file format:
 *
 * A savegame is a contiguous set of several data blocks.
 *
 * The first data block is the game info section.
 * This is a fixed-size data block containing the savegame marker. (currently "SPWAW_SAVE_V101")
 *
 * The other data blocks contain the savegame section data.
 *
 * Each data block consists of:
 *	 a data block header
 *	 the section data.
 *
 * The fixed-size data block header describes the section data that follows:
 *	the identification number of the section
 *	the size of the section data
 *	a flag indicating if the section data is compressed or not
 *
 * Uncompressed section data can be loaded as-is.
 *
 * Compressed section data must be decompressed after loading.
 * The actual size of the section data will be larger than the size of the section data in the file.
 */

/* This is the SPWaW section data compression format:
 *
 * Compressed section data is a contiguous set of run-length coded data blocks.
 *
 * There are two types of run-length coded data blocks:
 *	uncompressed run-length coded data (URLC)
 *	compressed run-length coded data (CRLC)
 *
 * Each run-length coded data block consists of:
 *	a run-length coding byte (RLB)
 *	one or more data bytes
 *
 * A compressed run-length coded data block is used to for sequences of identical data bytes:
 *	the RLB indicates the number of identical data bytes (N)
 *	the RLB is followed by a single data byte (B)
 *	the decompressed data is the sequence of N times the data byte B
 *
 * An uncompressed run-length coded data block is used for sequences of data bytes with mixed values:
 *	the RLB indicates the number of data bytes that follow (N)
 *	there is at least 1 and maximum 127 data bytes
 *	the N data bytes can be copied as-is
 */

typedef struct s_BLOCKHEAD {
	unsigned int	section;
	unsigned int	size;
	unsigned int	flag;
} BLOCKHEAD;

#define	FLAG_COMPRESSED	0x00000001

#define	ISCRLC(b_)	((unsigned char)b_ & 0x80)
#define	ISURLC(b_)	(!ISCRLC(b_))

#define	RL(b_)		((unsigned char)b_ & 0x7f)

#define	SETCRLC(b_,rl_)	b_ = (unsigned char)(0x80 | ((char)(rl_) & 0x7f))
#define	SETURLC(b_,ul_)	b_ = (unsigned char)((char)(ul_) & 0x7f)

static DWORD
unpack_crlc (BBR *bbr, unsigned char rlb, SBW *sbw)
{
	int		rl;
	unsigned char	c, buf[127];

	rl = RL(rlb);
	if ((rl <= 0) || (rl >= 128)) {
		ERROR1 ("invalid RL value: %d", rl);
		return (0);
	}

	if (!bbread (bbr, (char *)&c)) {
		ERROR0 ("failed to read CRLC data");
		return (0);
	}

	//log ("RL=%3.3d (0x%02.2x), RLB=0x%02.2x, C=0x%02.2x\n", rl, rl & 0xff, rlb, c);

	memset (buf, c, rl);
	if (sbwrite (sbw, (char *)buf, rl) != rl) {
		ERROR0 ("failed to write unpacked CRLC data");
		return (0);
	}

	return (rl);
}

static DWORD
unpack_urlc (BBR *bbr, unsigned char ulb, SBW *sbw)
{
	int		ul, todo;
	unsigned char	c, buf[127];
	int		done;

	ul = RL(ulb);
	if ((ul <= 0) || (ul >= 128)) {
		ERROR1 ("invalid UL value: %d", ul);
		return (0);
	}

	todo = ul; done = 0;
	while (todo) {
		if (!bbread (bbr, (char *)&c)) {
			ERROR0 ("failed to read URLC data");
			break;
		}
		buf[done] = c;
		todo--; done++;
	}

	//log ("UL=%3.3d (0x%02.2x), ULB=0x%02.2x, C=0x%02.2x...0x%02.2x\n", ul, ul & 0xff, ulb, buf[0], buf[done-1]);

	if (sbwrite (sbw, (char *)buf, done) != done) {
		ERROR0 ("failed to write unpacked URLC data");
	}

	return (done);
}


static DWORD
unpack (int fd, DWORD len, void *dst, DWORD size)
{
	BBR		*bbr = NULL;
	SBW		*sbw = NULL;
	int		left;
	DWORD		done;
	unsigned char	c;
	int		rl;

	if ((bbr = bbread_init (fd, len)) == 0) return (0);
	if ((sbw = sbwrite_init (dst, size)) == 0) return (0);

	left = len; done = 0;
	while (left) {
		if (!bbread (bbr, (char *)&c)) {
			ERROR0 ("failed to read packed data");
			break;
		}
		left--;

		if (ISCRLC(c)) {
			rl = unpack_crlc (bbr, c, sbw);
			left--;
		} else {
			rl = unpack_urlc (bbr, c, sbw);
			left -= rl;
		}
		done += rl;

		if (!rl) {
			ERROR0 ("failed to unpack data");
			break;
		}
	}

	sbwrite_stop (sbw);
	bbread_stop (bbr);

	return (done);
}

static DWORD
pack_crlc (SBW *sbw, int *rl, unsigned char c)
{
	unsigned char	rlb;
	int		done = 0;

	if ((*rl <= 0) || (*rl >= 128)) {
		ERROR1 ("invalid RL value: %d", *rl);
		return (0);
	}
	SETCRLC (rlb, *rl);

	//log ("RL=%3.3d (0x%02.2x), RLB=0x%02.2x, C=0x%02.2x\n", *rl, *rl & 0xff, rlb, c);

	if (sbwrite (sbw, (char *)&rlb, 1) != 1) {
		ERROR0 ("failed to write packed CRLC data");
		return (0);
	}
	if (sbwrite (sbw, (char *)&c, 1) != 1) {
		ERROR0 ("failed to write packed CRLC data");
		return (0);
	}

	done = 1 + 1; *rl = 0;

	return (done);
}

static DWORD
pack_urlc (SBW *sbw, int *ul, unsigned char *buf)
{
	unsigned char	ulb;
	int		done;

	if ((*ul <= 0) || (*ul >= 128)) {
		ERROR1 ("invalid UL value: %d", *ul);
		return (0);
	}
	SETURLC(ulb, *ul);

	//log ("UL=%3.3d (0x%02.2x), ULB=0x%02.2x, C=0x%02.2x...0x%02.2x\n", *ul, *ul & 0xff, ulb, buf[0], buf[*ul-1]);

	if (sbwrite (sbw, (char *)&ulb, 1) != 1) {
		ERROR0 ("failed to write packed URLC data");
		return (0);
	}
	if (sbwrite (sbw, (char *)buf, *ul) != *ul) {
		ERROR0 ("failed to write packed URLC data");
		return (0);
	}

	done = 1 + *ul; *ul = 0;

	return (done);
}
static long
pack (void *src, DWORD len, char **dst, long *size)
{
	SBR		*sbr = NULL;
	SBW		*sbw = NULL;
	int		left;
	long		done;
	int		start = 2;
	unsigned char	c, buf[127], c0;
	int		rl, ul;

	if (!src) return (0);
	if (!dst || !size) return (0);
	*dst = NULL; *size = 0;

	if ((sbr = sbread_init (src, len)) == NULL) return (0);
	if ((sbw = sbwrite_init (NULL, 0)) == NULL) return (0);

	memset (buf, 0, sizeof (buf));

	left = len; done = 0;
	ul = rl = 0; c0 = 0;
	while (left) {
		if (!sbread (sbr, (char *)&c, 1)) {
			ERROR0 ("failed to read unpacked data");
			break;
		}
		left--;

		if (start) {
			buf[ul++] = c;
			start--;
			continue;
		}

		if (ul) {
			if ((ul >= 2) && (c == buf[ul-1]) && (c == buf[ul-2])) {
				ul -= 2;
				if (ul) done += pack_urlc (sbw, &ul, buf);
				c0 = c;
				rl = 3;
			} else {
	                        if (ul >= 127) {
					done += pack_urlc (sbw, &ul, buf);
				}
				buf[ul++] = c;
			}
		} else {
			if (c == c0) {
				if (rl >= 127) {
					done += pack_crlc (sbw, &rl, c0);
					buf[ul++] = c;
				} else {
					rl++;
				}
			} else {
				done += pack_crlc (sbw, &rl, c0);
				buf[ul++] = c;
			}
		}
	}
	if (rl) {
		done += pack_crlc (sbw, &rl, c0);
	}
	if (ul) {
		done += pack_urlc (sbw, &ul, buf);
	}

	sbwrite_stop (sbw, dst, size);
	sbread_stop (sbr);

	return (done);
}

// TODO: fix API
bool
gamedata_load_all (GAMEFILE *file, GAMEDATA *dst)
{
	BLOCKHEAD	block;
	SECMAP		*sp;

	if (!file || !dst) return (false);

	if (bread (file->dat_fd, (char *)&(dst->sec00), sizeof (dst->sec00), false)) {
		log ("# gamedata_load_all: START<");
		for (int i=0; i<sizeof (dst->sec00); i++) log ("%c", dst->sec00.u.raw[i]);
		log (">\n");

		while (bread (file->dat_fd, (char *)&block, sizeof (block), true)) {
			log ("### gamedata_load_all: BLOCK #%lu <size=%lu, flag=0x%8.8x>\n", block.section, block.size, block.flag);

			sp = gamedata_section ((SECMAP *)&(dst->MAP), block.section);
			if (!sp) {
				ERROR1 ("unknown gamedata section %d", block.section);
			}
			void *dptr = sp->ptr;
			DWORD size = sp->size;
			if (block.flag & FLAG_COMPRESSED) {
				if (unpack (file->dat_fd, block.size, dptr, size) != size) {
					ERROR2 ("failed to unpack section %d (%lu bytes)", block.section, block.size);
					return (false);
				}
				log ("unpacked section %d: %lu bytes -> %lu bytes\n", block.section, block.size, size);
			} else {
				if (!bread (file->dat_fd, (char *)dptr, size, true)) {
					ERROR2 ("failed to read section %d (%lu bytes)", block.section, block.size);
					return (false);
				}
			}
		}
	}
	return (true);
}

bool
gamedata_load_section (GAMEFILE *file, DWORD sec, void *dst, unsigned long len)
{
	STRUCT00	head;
	SECMAP		*map, *sp;
	BLOCKHEAD	block;

	if (!file || !dst || !len) return (false);
	memset (dst, 0, len);
	map = gamedata_secmap ();

	sp = gamedata_section (map, sec);
	if (!sp) {
		ERROR1 ("unknown gamedata section %d", sec);
		return (false);
	}

	if ((sec == 0) && (len < sizeof (head))) {
		ERROR2 ("not enough room in caller buffer for section %d (%lu bytes)", sec, sizeof (head));
		return (false);
	}

	if (bread (file->dat_fd, (char *)&(head), sizeof (head), false)) {
		if (sec == 0) {
			memcpy (dst, &head, sizeof (head));
		} else {
			while (bread (file->dat_fd, (char *)&block, sizeof (block), true)) {
				if (sec == block.section) {
					DWORD size = sp->size;
					if (len < size) {
						ERROR2 ("not enough room in caller buffer for section %d (%lu bytes)", sec, size);
						return (false);
					}
					if (block.flag & FLAG_COMPRESSED) {
						if (unpack (file->dat_fd, block.size, dst, size) != size) {
							ERROR2 ("failed to unpack section %d (%lu bytes)", sec, size);
							return (false);
						}
						log ("unpacked section %d: %lu bytes -> %lu bytes\n", block.section, block.size, size);
					} else {
						if (!bread (file->dat_fd, (char *)dst, size, true)) return (false);
					}
					break;
				} else {
					bseekmove (file->dat_fd, block.size);
				}
			}
		}
	}
	return (true);
}

bool
gamedata_save_all (GAMEDATA *src, GAMEFILE *file)
{
	int		i;
	BLOCKHEAD	block;
	char		*dst;
	long		size;

	if (!src || !file) return (false);

	if (bwrite (file->dat_fd, (char *)&(src->sec00), sizeof (src->sec00))) {
		log ("# gamedata_save_all: START\n");

		for (i=1; i<SPWAW_SECTION_COUNT; i++) {
			block.section = src->MAP[i].idx;
			if (src->MAP[i].compress) {
				if (pack (src->MAP[i].ptr, src->MAP[i].size, &dst, &size) != size) {
					ERROR2 ("failed to pack section %d (%lu bytes)", block.section, src->MAP[i].size);
					return (false);
				}
				log ("packed section %d: %lu bytes -> %lu bytes\n", block.section, src->MAP[i].size, size);

				block.size = size;
				block.flag = FLAG_COMPRESSED;

				if (!bwrite (file->dat_fd, (char *)&block, sizeof (block))) {
					ERROR1 ("failed to write block header for section %d", block.section);
					return (false);
				}
				if (!bwrite (file->dat_fd, (char *)(dst), size)) {
					ERROR2 ("failed to write section %d (%lu bytes)", block.section, block.size);
					return (false);
				}
				safe_free (dst);
			} else {
				block.size = src->MAP[i].size;
				block.flag = 0;

				if (!bwrite (file->dat_fd, (char *)&block, sizeof (block))) {
					ERROR1 ("failed to write block header for section %d", block.section);
					return (false);
				}
				if (!bwrite (file->dat_fd, (char *)(src->MAP[i].ptr), src->MAP[i].size)) {
					ERROR2 ("failed to write section %d (%lu bytes)", block.section, block.size);
					return (false);
				}
			}
			log ("### gamedata_save_all: BLOCK #%lu <size=%lu, flag=0x%8.8x>\n", block.section, block.size, block.flag);
		}
	}
	return (true);
}
