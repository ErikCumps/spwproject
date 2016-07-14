/** \file
 * Slay's Library - safe memory management handling.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef SAFE_MEM_H
#define SAFE_MEM_H	1

#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */



/* --- API --- */

extern void	SL_SAFE_calloc	(const char *file, unsigned long line, const char *func, void **p, size_t n, size_t s);
extern void	SL_SAFE_malloc	(const char *file, unsigned long line, const char *func, void **p, size_t s);
extern void	SL_SAFE_realloc	(const char *file, unsigned long line, const char *func, void **p, size_t s);
extern void	SL_SAFE_free	(const char *file, unsigned long line, const char *func, void **p);
extern void	SL_SAFE_strdup	(const char *file, unsigned long line, const char *func, void **p, const char *s);

#define	SL_SAFE_CALLOC(p_, n_, s_)	SL_SAFE_calloc	(__FILE__, __LINE__, __FUNCTION__, (void **)&(p_), n_, s_)
#define	SL_SAFE_MALLOC(p_, s_)		SL_SAFE_malloc	(__FILE__, __LINE__, __FUNCTION__, (void **)&(p_), s_)
#define	SL_SAFE_REALLOC(p_, s_)		SL_SAFE_realloc	(__FILE__, __LINE__, __FUNCTION__, (void **)&(p_), s_)
#define	SL_SAFE_FREE(p_)		SL_SAFE_free	(__FILE__, __LINE__, __FUNCTION__, (void **)&(p_))
#define	SL_SAFE_STRDUP(p_, s_)		SL_SAFE_strdup	(__FILE__, __LINE__, __FUNCTION__, (void **)&(p_), (const char *)s_)



#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif /* SAFE_MEM_H */
