/** \file
 * The SPWaW Library - gamefile block compression handling.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/packing.h"
#include "gamefile/spwaw/packing_spwaw.h"
#include "gamefile/winspww2/packing_winspww2.h"
#include "fileio/fileio.h"
#include "common/internal.h"

/* This is the SP:WaW/winSPWW2 savegame file format:
 *
 * A savegame is a contiguous set of several data blocks.
 *
 * The first data block is the game info section.
 * This is a fixed-size data block containing the savegame marker.
 * (which is currently "SPWAW_SAVE_V101"/"SPCTS_SAVE_V100" for SP:WaW/winSPWW2)
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

/* This is the SP:WaW/winSPWW2 section data compression format:
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

#define	GAMEINFOSECTION	0

#define	ISCRLC(b_)	((unsigned char)b_ & 0x80)
#define	ISURLC(b_)	(!ISCRLC(b_))

#define	RL(b_)		((unsigned char)b_ & 0x7f)

#define	SETCRLC(b_,rl_)	b_ = (unsigned char)(0x80 | ((char)(rl_) & 0x7f))
#define	SETURLC(b_,ul_)	b_ = (unsigned char)((char)(ul_) & 0x7f)

static DWORD
unpack_crlc (BBR *bbr, unsigned char rlb, SBW *sbw, DWORD offset)
{
	int		rl;
	unsigned char	c, buf[127];
#if	!PACKLOGGING
	/* prevent compiler warnings */
	UNREFERENCED_PARAMETER (offset);
#endif	/* !PACKLOGGING */

	rl = RL(rlb);
	if ((rl <= 0) || (rl >= 128)) {
		ERROR1 ("invalid RL value: %d", rl);
		return (0);
	}

	if (!bbread (bbr, (char *)&c)) {
		ERROR0 ("failed to read CRLC data");
		return (0);
	}

	PACKLOG5 ("UNPACK CRLC [0x%8.8x] RL=%3.3d (0x%02.2x), RLB=0x%02.2x, C=0x%02.2x\n",
		offset, rl, rl & 0xff, rlb, c);

	memset (buf, c, rl);
	if (sbwrite (sbw, (char *)buf, rl) != rl) {
		ERROR0 ("failed to write unpacked CRLC data");
		return (0);
	}

	return (rl);
}

static DWORD
unpack_urlc (BBR *bbr, unsigned char ulb, SBW *sbw, DWORD offset)
{
	int		ul, todo;
	unsigned char	c, buf[127];
	int		done;
#if	!PACKLOGGING
	/* prevent compiler warnings */
	UNREFERENCED_PARAMETER (offset);
#endif	/* !PACKLOGGING */

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

	PACKLOG6 ("UNPACK URLC [0x%8.8x] UL=%3.3d (0x%02.2x), ULB=0x%02.2x, C=0x%02.2x...0x%02.2x\n",
		offset, ul, ul & 0xff, ulb, buf[0], buf[done-1]);

	if (sbwrite (sbw, (char *)buf, done) != done) {
		ERROR0 ("failed to write unpacked URLC data");
	}

	return (done);
}


static unsigned long
unpack (int fd, DWORD len, void **dst, unsigned long *size)
{
	BBR		*bbr = NULL;
	SBW		*sbw = NULL;
	int		left;
	DWORD		done;
	unsigned char	c;
	int		rl;

	if (!dst || !size) return (0);

	if ((bbr = bbread_init (fd, len)) == 0) return (0);
	if ((sbw = sbwrite_init (*dst, *size)) == 0) return (0);

	PACKLOG4 ("UNPACK %lu (0x%8.8x) -> %lu (0x%8.8x)\n", len, len, *size, *size);

	left = len; done = 0;
	while (left) {
		if (!bbread (bbr, (char *)&c)) {
			ERROR0 ("failed to read packed data");
			break;
		}
		left--;

		if (ISCRLC(c)) {
			rl = unpack_crlc (bbr, c, sbw, done);
			left--;
		} else {
			rl = unpack_urlc (bbr, c, sbw, done);
			left -= rl;
		}
		done += rl;

		if (!rl) {
			ERROR0 ("failed to unpack data");
			break;
		}
	}

	if (!*dst) {
		sbwrite_stop (sbw, (char **)dst, (long *)size);
	} else {
		sbwrite_stop (sbw);
	}
	bbread_stop (bbr);

	return (done);
}

static DWORD
pack_crlc (SBW *sbw, int *rl, unsigned char c, DWORD offset)
{
	unsigned char	rlb;
	int		done = 0;
#if	!PACKLOGGING
	/* prevent compiler warnings */
	UNREFERENCED_PARAMETER (offset);
#endif	/* !PACKLOGGING */

	if ((*rl <= 0) || (*rl >= 128)) {
		ERROR1 ("invalid RL value: %d", *rl);
		return (0);
	}
	SETCRLC (rlb, *rl);

	PACKLOG5 ("PACK CRLC [0x%8.8x] RL=%3.3d (0x%02.2x), RLB=0x%02.2x, C=0x%02.2x\n",
		offset, *rl, *rl & 0xff, rlb, c);

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
pack_urlc (SBW *sbw, int *ul, unsigned char *buf, DWORD offset)
{
	unsigned char	ulb;
	int		done;
#if	!PACKLOGGING
	/* prevent compiler warnings */
	UNREFERENCED_PARAMETER (offset);
#endif	/* !PACKLOGGING */

	if ((*ul <= 0) || (*ul >= 128)) {
		ERROR1 ("invalid UL value: %d", *ul);
		return (0);
	}
	SETURLC(ulb, *ul);

	PACKLOG6 ("PACK URLC [0x%8.8x] UL=%3.3d (0x%02.2x), ULB=0x%02.2x, C=0x%02.2x...0x%02.2x\n",
		offset, *ul, *ul & 0xff, ulb, buf[0], buf[*ul-1]);

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
	PACKLOG4 ("PACK %lu (0x%8.8x) -> %lu (0x%8.8x)\n", len, len, size, size);

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
				if (ul) done += pack_urlc (sbw, &ul, buf, done);
				c0 = c;
				rl = 3;
			} else {
	                        if (ul >= 127) {
					done += pack_urlc (sbw, &ul, buf, done);
				}
				buf[ul++] = c;
			}
		} else {
			if (c == c0) {
				if (rl >= 127) {
					done += pack_crlc (sbw, &rl, c0, done);
					buf[ul++] = c;
				} else {
					rl++;
				}
			} else {
				done += pack_crlc (sbw, &rl, c0, done);
				buf[ul++] = c;
			}
		}
	}
	if (rl) {
		done += pack_crlc (sbw, &rl, c0, done);
	}
	if (ul) {
		done += pack_urlc (sbw, &ul, buf, done);
	}

	sbwrite_stop (sbw, dst, size);
	sbread_stop (sbr);

	return (done);
}

static bool
load_block (SPWAW_GAME_TYPE gametype, int fd, BLOCKHEAD &block)
{
	switch (gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			return (load_spwaw_block(fd, block));
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			return (load_winspww2_block(fd, block));
			break;
		default:
			ERROR1 ("unknown gametype %d", gametype);
			break;
	}

	return (false);
}

static bool
save_block (SPWAW_GAME_TYPE gametype, int fd, BLOCKHEAD &block)
{
	switch (gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			return (save_spwaw_block(fd, block));
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			return (save_winspww2_block(fd, block));
			break;
		default:
			ERROR1 ("unknown gametype %d", gametype);
			break;
	}

	return (false);
}

static bool
unpack_section (int fd, BLOCKHEAD &block, void **dst, unsigned long *len)
{
	if (!dst || !len) return (false);

	if (block.flag & FLAG_COMPRESSED) {
		volatile unsigned long *vlen = len;
		if (unpack (fd, block.size, dst, len) != *vlen) {
			ERROR2 ("failed to unpack section %d (%lu bytes)", block.section, *vlen);
			return (false);
		}
		log ("unpacked section %d: %lu bytes -> %lu bytes\n", block.section, block.size, *len);
	} else {
		if (*dst && *len) {
			if (*len < block.size) {
				ERROR2 ("not enough room in caller buffer for uncompressed section %d data (%lu bytes)", block.section, block.size);
				return (false);
			}
		} else {
			*dst = safe_smalloc (char, block.size); COOMRET (*dst, "uncompressed section data buffer", false);
			*len = block.size;
		}
		if (!bread (fd, (char *)*dst, block.size, true)) {
			ERROR2 ("failed to read section %d data (%lu bytes)", block.section, block.size);
			return (false);
		}
	}
	return (true);
}

bool
load_packed (GAMEFILE *file, GAMEDATA *dst)
{
	BLOCKHEAD	block;
	SECMAPEL	*sp;
	long		pos;

	if (!file || !dst) return (false);
	if (file->gametype != dst->gametype) return (false);

	bseekset (file->data.fd, 0);

	sp = gamedata_section (&(dst->map), GAMEINFOSECTION);
	if (bread (file->data.fd, (char *)sp->ptr, sp->size, false)) {
		log ("# load_packed: START<");
		for (DWORD i=0; i<sp->size; i++) log ("%c", ((char *)sp->ptr)[i]);
		log (">\n");
	} else {
		ERROR0 ("failed to read game info section");
		return (false);
	}

	pos = bseekget(file->data.fd);

	while (load_block (file->gametype, file->data.fd, block)) {
		log ("### load_packed: 0x%8.8x BLOCK #%lu (0x%8.8x) <size=%lu (0x%8.8x), flag=0x%8.8x>\n", pos, block.section, block.section, block.size, block.size, block.flag);

		sp = gamedata_section (&(dst->map), block.section);
		if (!sp) {
			ERROR1 ("skipping unknown gamedata section %d", block.section);
			bseekset (file->data.fd, bseekget(file->data.fd)+block.size);
		} else {
			if (!unpack_section (file->data.fd, block, &(sp->ptr), &(sp->size))) return (false);
		}
		pos = bseekget(file->data.fd);
	}

	return (true);
}

bool
save_packed (GAMEDATA *src, GAMEFILE *file)
{
	int		i;
	BLOCKHEAD	block;
	char		*dst;
	long		size;

	if (!src || !file) return (false);
	if (src->gametype != file->gametype) return (false);

	bseekset (file->data.fd, 0);

	SECMAPEL *sp = gamedata_section (&(src->map), GAMEINFOSECTION);
	if (bwrite (file->data.fd, (char *)sp->ptr, sp->size)) {
		log ("# save_packed: START\n");
	} else {
		ERROR0 ("failed to write game info section");
		return (false);
	}

	for (i=1; i<src->map.cnt; i++) {
		if (src->map.list[i].optional && (src->map.list[i].size == 0)) continue;

		block.section = src->map.list[i].idx;
		if (src->map.list[i].compress) {
			if (pack (src->map.list[i].ptr, src->map.list[i].size, &dst, &size) != size) {
				ERROR2 ("failed to pack section %d (%lu bytes)", block.section, src->map.list[i].size);
				return (false);
			}
			log ("packed section %d: %lu bytes -> %lu bytes\n", block.section, src->map.list[i].size, size);

			block.size = size;
			block.flag = FLAG_COMPRESSED;

			if (!save_block (file->gametype, file->data.fd, block)) {
				ERROR1 ("failed to write block header for section %d", block.section);
				return (false);
			}
			if (!bwrite (file->data.fd, (char *)(dst), size)) {
				ERROR2 ("failed to write section %d (%lu bytes)", block.section, block.size);
				return (false);
			}
			safe_free (dst);
		} else {
			block.size = src->map.list[i].size;
			block.flag = 0;

			if (!save_block (file->gametype, file->data.fd, block)) {
				ERROR1 ("failed to write block header for section %d", block.section);
				return (false);
			}
			if (!bwrite (file->data.fd, (char *)(src->map.list[i].ptr), src->map.list[i].size)) {
				ERROR2 ("failed to write section %d (%lu bytes)", block.section, block.size);
				return (false);
			}
		}
		log ("### save_packed: BLOCK #%lu <size=%lu, flag=0x%8.8x>\n", block.section, block.size, block.flag);
	}

	return (true);
}

bool
load_packed_section (GAMEFILE *file, DWORD sec, void **dst, unsigned long *len)
{
	SECMAP		*map;
	SECMAPEL	*spinfo, *sp;
	BLOCKHEAD	block;

	if (!file || !dst || !len) return (false);
	*dst = NULL; *len = 0;

	map = gamedata_SECMAP (file->gametype);

	spinfo = gamedata_section (map, GAMEINFOSECTION);
	if (!spinfo) {
		ERROR0 ("no game info section present in section map");
		return (false);
	}

	sp = gamedata_section (map, sec);
	if (!sp) {
		ERROR1 ("unknown gamedata section %d", sec);
		return (false);
	}

	*len = sp->size;
	*dst = safe_smalloc (char, *len); COOMRET (dst, "gamedata section data buffer", false);

	bseekset (file->data.fd, 0);

	if (sec == GAMEINFOSECTION) {
		if (!bread (file->data.fd, (char *)(*dst), *len, false)) {
			ERROR0 ("failed to read game info section");
			return (false);
		}
	} else {
		bseekmove (file->data.fd, spinfo->size);

		while (load_block (file->gametype, file->data.fd, block)) {
			if (sec == block.section) {
				if (!unpack_section (file->data.fd, block, dst, len)) return (false);
				break;
			} else {
				bseekmove (file->data.fd, block.size);
			}
		}
	}

	return (true);
}