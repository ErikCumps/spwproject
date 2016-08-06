/** \file
 * A simple tool to report the byte distribution in a file.
 *
 * Copyright (C) 2010-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <io.h>

typedef int ssize_t;

static void
usage (char *name)
{
	printf ("Usage: %s in\n", name);
	printf ("Where: in    input filename\n");
	printf ("\n");
	exit (1);
}

typedef enum e_CNT {
	cnt_alf = 0,
	cnt_num,
	cnt_pnc,
	cnt_spc,
	cnt_nul,
	cnt_rst,
	cnt_LAST
} CNT;

typedef struct s_FREQ {
	int		idx;
	unsigned long	cnt;
} FREQ;

typedef struct s_BI {
	unsigned long	index;		/* block index */
	unsigned long	offset;		/* block physical offset */
	int		size;		/* block physical size */
	unsigned char	*buffer;	/* block buffer pointer */
} BI;

typedef void (* BIfunc)(BI *info, unsigned long *gccnt, FREQ *gbcnt);

#define	BUFSIZE	4096
#define	BLKSIZE	 512
#define	minsize(a,b)	((a < b) ? a : b)

static int
sort_freq (const void *a, const void *b)
{
	FREQ	*ap = (FREQ *)a;
	FREQ	*bp = (FREQ *)b;

	return ((ap->cnt==bp->cnt)?0:((ap->cnt>bp->cnt)?-1:+1));
}

static int
sort_char (const void *a, const void *b)
{
	FREQ	*ap = (FREQ *)a;
	FREQ	*bp = (FREQ *)b;

	return ((ap->idx==bp->idx)?0:((ap->idx<bp->idx)?-1:+1));
}

static int
process (int in, BIfunc func)
{
	BI		info;
	unsigned char	buffer[BUFSIZE];
	unsigned char	*bidxlist[(BUFSIZE/BLKSIZE)+1];
	int		bidx;
	ssize_t		bread;
	unsigned long	size;
	unsigned long	ccnt[cnt_LAST];
	double		cprc[cnt_LAST];
	FREQ		bcnt[256];
	double		bprc[256];
	int		i, c;

	memset (&info, 0, sizeof (info));
	memset (bidxlist, 0, sizeof (bidxlist));
	bidx = 0;
	while ((bidx * BLKSIZE) < BUFSIZE) {
		bidxlist[bidx] = &(buffer[bidx * BLKSIZE]);
		bidx++;
	}

	memset (ccnt, 0, sizeof (ccnt));
	memset (cprc, 0, sizeof (cprc));
	memset (bcnt, 0, sizeof (bcnt));
	memset (bprc, 0, sizeof (bprc));

	for (i=0; i<256; i++) bcnt[i].idx = i;

	size = 0;
	while (1) {
		memset (buffer, 0, sizeof (buffer));
		bread = _read (in, buffer, sizeof (buffer));
		if (bread <= 0) break;

		size += bread; bidx = 0;
		while (bread > 0)  {
			info.size = minsize (BLKSIZE, bread);
			info.buffer = bidxlist[bidx];
			func (&info, ccnt, bcnt);
			info.index++;
			info.offset += BLKSIZE;
			bidx++;
			bread -= BLKSIZE;
		}
	}
	if (bread == -1) {
		fprintf (stderr, "*** Error: error reading from file! (errno: %d)\n", errno);
		return (1);
	}

	printf ("# Character class distribution:\n");
	printf ("# %9s %9s %9s %9s %9s %9s\n", "alfa", "num", "punc", "space", "nul", "rest");

	cprc[cnt_alf] = ccnt[cnt_alf] * 100.0 / size;
	cprc[cnt_num] = ccnt[cnt_num] * 100.0 / size;
	cprc[cnt_pnc] = ccnt[cnt_pnc] * 100.0 / size;
	cprc[cnt_spc] = ccnt[cnt_spc] * 100.0 / size;
	cprc[cnt_nul] = ccnt[cnt_nul] * 100.0 / size;
	cprc[cnt_rst] = ccnt[cnt_rst] * 100.0 / size;
	
	printf ("  %9lu %9lu %9lu %9lu %9lu %9lu\n",
			ccnt[cnt_alf], ccnt[cnt_num], ccnt[cnt_pnc], ccnt[cnt_spc], ccnt[cnt_nul], ccnt[cnt_rst]);
	printf ("  %8.2f%% %8.2f%% %8.2f%% %8.2f%% %8.2f%% %8.2f%%\n",
			cprc[cnt_alf], cprc[cnt_num], cprc[cnt_pnc], cprc[cnt_spc], cprc[cnt_nul], cprc[cnt_rst]);
	printf ("\n");

	qsort (bcnt, 256, sizeof (FREQ), sort_freq);

	printf ("# Byte distribution, sorted on frequency:\n");
	printf ("# %4s %9s %6s\n", "char", "count", "dist");
	for (i=0; i<256; i++) {
		bprc[i] = bcnt[i].cnt * 100.0 / size;
		c = bcnt[i].idx & 0xff;
		if (isprint (c)) {
			printf ("  [ %c] %9lu %6.2f%%\n", c, bcnt[i].cnt, bprc[i]);
		} else {
			printf ("  [%2.2x] %9lu %6.2f%%\n", c, bcnt[i].cnt, bprc[i]);
		}
	}
	printf ("\n");

	qsort (bcnt, 256, sizeof (FREQ), sort_char);

	printf ("# Byte distribution, sorted on byte value:\n");
	printf ("# %4s %9s %6s\n", "char", "count", "dist");
	for (i=0; i<256; i++) {
		bprc[i] = bcnt[i].cnt * 100.0 / size;
		c = bcnt[i].idx & 0xff;
		if (isprint (c)) {
			printf ("  [ %c] %9lu %6.2f%%\n", c, bcnt[i].cnt, bprc[i]);
		} else {
			printf ("  [%2.2x] %9lu %6.2f%%\n", c, bcnt[i].cnt, bprc[i]);
		}
	}
	printf ("\n");

	return (0);
}

static void
analyzer (BI *info, unsigned long *gccnt, FREQ *gbcnt)
{
	int		idx, c;
	unsigned int	ccnt[cnt_LAST];
	unsigned int	bcnt[256];

	memset (ccnt, 0, sizeof (ccnt));
	memset (bcnt, 0, sizeof (bcnt));

	for (idx = 0; idx < info->size; idx++) {
		c = info->buffer[idx];
		bcnt[c]++;

		if (	(('A' <= c) && (c <= 'Z'))	||
			(('a' <= c) && (c <= 'z'))	)
		{
			ccnt[cnt_alf]++;
			continue;
		}

		if (	('0' <= c) && (c <= '9')	)
		{
			ccnt[cnt_num]++;
			continue;
		}

		if (	(('!' <= c) && (c <= '/'))	||
			((':' <= c) && (c <= '@'))	||
			(('[' <= c) && (c <= '`'))	||
			(('{' <= c) && (c <= '~'))	)
		{
			ccnt[cnt_pnc]++;
			continue;
		}

		if ((c == ' ') || (c == 0x09) || (c == 0x0A))
		{
			ccnt[cnt_spc]++;
			continue;
		}

		if (c == '\0')
		{
			ccnt[cnt_nul]++;
			continue;
		}

		ccnt[cnt_rst]++;
	}
	
	if (gccnt) {
		gccnt[cnt_alf] += ccnt[cnt_alf];
		gccnt[cnt_num] += ccnt[cnt_num];
		gccnt[cnt_pnc] += ccnt[cnt_pnc];
		gccnt[cnt_spc] += ccnt[cnt_spc];
		gccnt[cnt_nul] += ccnt[cnt_nul];
		gccnt[cnt_rst] += ccnt[cnt_rst];
	}

	if (gbcnt) {
		for (idx=0; idx<256; idx++) gbcnt[idx].cnt += bcnt[idx];
	}
}

int
main (int argc, char **argv)
{
	int	rc, fd;

	if (argc < 2) usage (argv[0]);
	
	fd = _open (argv[1], O_RDONLY|O_BINARY);
	if (fd == -1) {
		fprintf (stderr, "*** Error: failed to open input file \"%s\" for reading! (errno %d)\n",
				argv[1], errno);
		return (1);
	}
	
	rc = process (fd, analyzer);

	_close (fd);
	
	return (rc);
}
