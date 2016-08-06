/** \file
 * The SPWaW Library - file I/O handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "gamefile/gamefile.h"
#include "fileio/fileio.h"
#include "utils/log.h"
#include "common/internal.h"

typedef struct s_BLOCKHEAD {
	unsigned int	section;
	unsigned int	size;
	unsigned int	flag;
} BLOCKHEAD;

#define	FLAG_COMPRESSED	0x00000001

#define	ISRLC(b_)	((unsigned char)b_ & 0x80)
#define	RL(b_)		((unsigned char)b_ & 0x7f)

static DWORD
unpack (int fd, DWORD len, void *dst, DWORD size)
{
	BBR	*bbr = NULL;
	BBW	*bbw = NULL;
	int	left;
	DWORD	done;
	char	c0, c1, gbuf[256], fbuf[256];
	int	rl;

	if ((bbr = bbread_init (fd, len)) == 0) return (0);
	if ((bbw = bbwrite_init (dst, size)) == 0) return (0);

	memset (gbuf, 0, sizeof (gbuf));
	memset (fbuf, 0, sizeof (fbuf));

	left = len; done = 0;
	while (left) {
		if (!bbread (bbr, &c0)) break;
		left--;

		if (ISRLC(c0)) {
			rl = RL (c0);

			if (!bbread (bbr, &c1)) {
				bbwrite (bbw, &c0);
				done++;
				continue;
			}
			left--;
			memset (gbuf, c1, rl);
			bbwrite (bbw, gbuf, rl);
			done += rl;
		} else {
			if (!bbread (bbr, fbuf, c0, false)) {
				bbwrite (bbw, &c0);
				done++;
			} else {
				left -= c0;
				bbwrite (bbw, fbuf, c0);
				done += c0;
			}
		}
	}

	bbwrite_stop (bbw);
	bbread_stop (bbr);

	return (done);
}

// TODO: fix API
bool
gamedata_load_all (GAMEFILE *file, GAMEDATA *data)
{
	int		i;
	BLOCKHEAD	block;
	void		*dst;
	DWORD		size;

	if (!file || !data) return (false);

	if (bread (file->dat_fd, (char *)&(data->sec00), sizeof (data->sec00), false)) {
		log ("# START<");
		for (i=0; i<sizeof (data->sec00); i++) log_nots ("%c", data->sec00.u.raw[i]);
		log_nots (">\n");

		while (bread (file->dat_fd, (char *)&block, sizeof (block), true)) {
			log ("### BLOCK #%lu <size=%lu, flag=0x%8.8x>\n", block.section, block.size, block.flag);

			dst  = data->MAP[block.section].ptr;
			size = data->MAP[block.section].size;
			if (block.flag & FLAG_COMPRESSED) {
				if (unpack (file->dat_fd, block.size, dst, size) != size) {
					ERROR2 ("failed to unpack section %d (%lu bytes)", block.section, block.size);
					return (false);
				}
			} else {
				if (!bread (file->dat_fd, (char *)dst, size, true)) {
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
	SECMAP		*map;
	BLOCKHEAD	block;
	DWORD		size;

	if (!file || !dst || !len) return (false);
	memset (dst, 0, len);
	map = gamedata_secmap ();

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
					size = map[block.section].size;
					if (len < size) {
						ERROR2 ("not enough room in caller buffer for section %d (%lu bytes)", sec, size);
						return (false);
					}
					if (block.flag & FLAG_COMPRESSED) {
						if (unpack (file->dat_fd, block.size, dst, size) != size) {
							ERROR2 ("failed to unpack section %d (%lu bytes)", sec, size);
							return (false);
						}
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
