#ifndef	INTERNAL_STRBUF_H
#define	INTERNAL_STRBUF_H	1

#include <spwawlib_api.h>

typedef struct s_STRBUF_HOLE {
	char			*ptr;
	ULONG			len;
	struct s_STRBUF_HOLE	*next;
} STRBUF_HOLE;

typedef struct s_STRBUF_BLOCK {
	struct s_STRBUF_BLOCK	*next;
	ULONG			size;
	ULONG			used;
	ULONG			left;
	char			*data;
	char			*last;
	STRBUF_HOLE		*hole;
} STRBUF_BLOCK;

typedef struct s_STRBUF {
	STRBUF_BLOCK	*list;
	STRBUF_BLOCK	*last;
} STRBUF;

extern SPWAW_ERROR	STRBUF_new	(STRBUF **buf);
extern void		STRBUF_free	(STRBUF **buf);
extern SPWAW_ERROR	STRBUF_init	(STRBUF *buf);
extern void		STRBUF_clear	(STRBUF *buf);
extern SPWAW_ERROR	STRBUF_load	(STRBUF *buf, char *file);
extern SPWAW_ERROR	STRBUF_fdload	(STRBUF *buf, int fd);
extern SPWAW_ERROR	STRBUF_save	(STRBUF *buf, char *file, bool compress);
extern SPWAW_ERROR	STRBUF_fdsave	(STRBUF *buf, int fd, bool compress);
extern char *		STRBUF_add	(STRBUF *buf, char *str);
extern void		STRBUF_del	(STRBUF *buf, char *str);
extern char *		STRBUF_next	(STRBUF *buf, char *str);

extern SPWAW_ERROR	STRBUF_export	(int src, FILE *dst);
extern SPWAW_ERROR	STRBUF_import	(FILE *src, int dst);

extern void		STRBUF_report	(FILE *rf, STRBUF *buf);

#endif	/* INTERNAL_STRBUF_H */
