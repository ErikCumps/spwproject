#ifndef	COMPRESSION_H
#define	COMPRESSION_H	1

/* Simple single-step compression and CRC32 calculation using zlib */

extern SPWAW_ERROR	zcompress	(char *src, unsigned long slen, char **dst, unsigned long *dlen);
extern SPWAW_ERROR	zexpand		(char *src, unsigned long slen, char *dst, unsigned long dlen);
extern SPWAW_ERROR	zbuildcrc	(int fd, unsigned long spos, unsigned long fpos, unsigned long *crc);
extern SPWAW_ERROR	zbuildcrc	(char *buf, unsigned long len, unsigned long *crc);

#endif	/* COMPRESSION_H */
