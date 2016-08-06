#ifndef	INTERNAL_STRTAB_H
#define	INTERNAL_STRTAB_H	1

#include <spwawlib_api.h>
#include "strbuf.h"
#include <ad_hash.h>

#define	BADSTRIDX	((ULONG)-1)

typedef struct s_STRIDX {
	char	*ptr;
	ULONG	ref;
} STRIDX;

typedef struct s_STRTAB {
	ULONG		cnt;
	STRBUF		*buf;
	STRIDX		*idx;
	ULONG		ilen;
	ULONG		icnt;
	ADhash_t	hash;
} STRTAB;

typedef struct s_STRINFO {
	ULONG	idx;
	char	*str;
} STRINFO;

extern SPWAW_ERROR	STRTAB_new	(STRTAB **tab);
extern void		STRTAB_free	(STRTAB **tab);
extern void		STRTAB_clear	(STRTAB *tab);
extern SPWAW_ERROR	STRTAB_init	(STRTAB *tab, char *buffer, ULONG size);
extern SPWAW_ERROR	STRTAB_load	(STRTAB *tab, char *file);
extern SPWAW_ERROR	STRTAB_fdload	(STRTAB *tab, int fd);
extern SPWAW_ERROR	STRTAB_compact	(STRTAB *tab);
extern SPWAW_ERROR	STRTAB_save	(STRTAB *tab, char *file, bool compress);
extern SPWAW_ERROR	STRTAB_fdsave	(STRTAB *tab, int fd, bool compress);
extern SPWAW_ERROR	STRTAB_add	(STRTAB *tab, char *str, STRINFO *inf);
extern SPWAW_ERROR	STRTAB_add	(STRTAB *tab, char *str, ULONG cnt, STRINFO *inf);
extern char *		STRTAB_add	(STRTAB *tab, char *str);
extern char *		STRTAB_add	(STRTAB *tab, char *str, ULONG cnt);
extern void		STRTAB_del	(STRTAB *tab, STRINFO *inf);
extern void		STRTAB_del	(STRTAB *tab, STRINFO *inf, ULONG cnt);
extern void		STRTAB_del	(STRTAB *tab, char *str);
extern void		STRTAB_del	(STRTAB *tab, char *str, ULONG cnt);
extern SPWAW_ERROR	STRTAB_getstr	(STRTAB *tab, STRINFO *inf);
extern char *		STRTAB_getstr	(STRTAB *tab, ULONG idx);
extern SPWAW_ERROR	STRTAB_getidx	(STRTAB *tab, STRINFO *inf);
extern ULONG		STRTAB_getidx	(STRTAB *tab, char *str);

extern SPWAW_ERROR	STRTAB_merge	(STRTAB *tab, STRTAB *ptr);
extern SPWAW_ERROR	STRTAB_remove	(STRTAB *tab, STRTAB *ptr);

extern SPWAW_ERROR	STRTAB_export	(int src, FILE *dst);
extern SPWAW_ERROR	STRTAB_import	(FILE *src, int dst);

extern void		debug_STRTAB_report	(FILE *rf, STRTAB *tab, char *msg);

#endif	/* INTERNAL_STRTAB_H */



