/** \file
 * The SPWaW Library - file I/O handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	FILEIO_H
#define	FILEIO_H	1

#define	BLKSZ	4096

/* File position handling */
extern void	bseekset	(int fd, long pos);
extern void	bseekmove	(int fd, long len);
extern long	bseekget	(int fd);

/* Basic file IO */
extern bool	bread		(int fd, char *b, int len, bool noerr);
extern bool	bwrite		(int fd, char *b, int len);

/* Buffered file IO: reading */
typedef struct s_BBR {
	struct s_file {
		int	fd;
		long	left;
	} file;
	int	bsize;
	char	buffer[BLKSZ];
	int	idx;
	int	left;
} BBR;

extern BBR	*bbread_init	(int fd, long len);
extern bool	bbread		(BBR *bb, char *b);
extern bool	bbread		(BBR *bb, char *b, int len, bool noerr);
extern void	bbread_stop	(BBR *bb);

/* Buffered file IO: writing */
typedef struct s_BBW {
	char	*dst;
	long	left;
	long	idx;
} BBW;

extern BBW	*bbwrite_init	(void *dst, long len);
extern void	bbwrite		(BBW *bb, char *b);
extern void	bbwrite		(BBW *bb, char *b, int len);
extern void	bbwrite_stop	(BBW *bb);

/* Streambuffered file IO: reading */
typedef struct s_SBR {
	char	*buf;
	long	len;
	long	idx;
	long	lft;
} SBR;

extern SBR	*sbread_init	(void *buf, long len);
extern int	sbread		(SBR *sb, char *b, int len);
extern void	sbread_stop	(SBR *sb);

/* Streambuffered file IO: writing */
typedef struct s_SBW {
	bool	grow;
	char	*buf;
	long	len;
	long	idx;
	long	lft;
} SBW;

extern SBW	*sbwrite_init	(void *buf, long len);
extern int	sbwrite		(SBW *sb, char *b, int len);
extern void	sbwrite_stop	(SBW *sb);
extern void	sbwrite_stop	(SBW *sb, char **buf, long *len);

/* Basic compressed file IO support */
typedef struct s_CBIO {
	char		*data;
	unsigned long	size;
	unsigned long	*comp;
} CBIO;

#define	cbread(fd_,cb_,n_)	\
cbread_core (fd_, cb_, "compressed " n_, "bread(compressed " n_ ") failed", "zexpand(compressed " n_ ")", "bread(" n_ ") failed")
extern bool	cbread_core	(int fd, CBIO cbio, const char *m1, const char *m2, const char *m3, const char *m4);

#define	cbwrite(fd_,cb_,n_,cf_)	\
cbwrite_core (fd_, cb_, cf_, "zcompress(" n_ ")", "bwrite(compressed " n_ ") failed", "bwrite(" n_ ") failed")
extern bool	cbwrite_core	(int fd, CBIO cbio, bool compress, const char *m1, const char *m2, const char *m3);




#endif	/* FILEIO_H */
