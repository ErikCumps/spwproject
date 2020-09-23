/** \file
 * debug_memtrace: main interface.
 *
 * Copyright (C) 2008-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	DEBUG_MEMTRACE_H
#define	DEBUG_MEMTRACE_H	1

// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the DEBUG_MEMTRACE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// DEBUG_MEMTRACE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DEBUG_MEMTRACE_EXPORTS
#define DEBUG_MEMTRACE_API __declspec(dllexport)
#else
#define DEBUG_MEMTRACE_API __declspec(dllimport)
#endif

#ifdef	DEBUG_MEMTRACE

#define	malloc(s_)				debug_memtrace_malloc		(__FILE__, __LINE__, __FUNCTION__, s_)
#define	calloc(n_,s_)				debug_memtrace_calloc		(__FILE__, __LINE__, __FUNCTION__, n_, s_)
#define	free(p_)				debug_memtrace_free		(__FILE__, __LINE__, __FUNCTION__, p_)
#define	realloc(p_,s_)				debug_memtrace_realloc		(__FILE__, __LINE__, __FUNCTION__, p_, s_)
#define	strdup(p_)				debug_memtrace_strdup		(__FILE__, __LINE__, __FUNCTION__, p_)
#define	CoTaskMemAlloc(s_)			debug_memtrace_CoTaskMemAlloc	(__FILE__, __LINE__, __FUNCTION__, s_)
#define	CoTaskMemFree(p_)			debug_memtrace_CoTaskMemFree	(__FILE__, __LINE__, __FUNCTION__, p_)
#define	CoTaskMemRealloc(p_,s_)			debug_memtrace_CoTaskMemRealloc	(__FILE__, __LINE__, __FUNCTION__, p_, s_)

#ifdef __cplusplus
#ifdef	DEBUG_MEMTRACE_CPP
#define	new	debug_memtrace_new (__FILE__, __LINE__, __FUNCTION__) ->* new
#define	delete	debug_memtrace_delete (__FILE__, __LINE__, __FUNCTION__), delete
#endif
#endif

#define	MEMTRACE_INIT(log_)			debug_memtrace_init		(log_)
#define	MEMTRACE_MARK(mark_)			debug_memtrace_mark		(mark_)

#define	MEMTRACE_MALLOC(fi_,li_,fu_,s_)		debug_memtrace_malloc		(fi_, li_, fu_, s_)
#define	MEMTRACE_CALLOC(fi_,li_,fu_,n_,s_)	debug_memtrace_calloc		(fi_, li_, fu_, n_, s_)
#define	MEMTRACE_FREE(fi_,li_,fu_,p_)		debug_memtrace_free		(fi_, li_, fu_, p_)
#define	MEMTRACE_REALLOC(fi_,li_,fu_,p_,s_)	debug_memtrace_realloc		(fi_, li_, fu_, p_, s_)
#define	MEMTRACE_STRDUP(fi_,li_,fu_,p_)		debug_memtrace_strdup		(fi_, li_, fu_, p_)
#define	MEMTRACE_COMMALLOC(fi_,li_,fu_,s_)	debug_memtrace_CoTaskMemAlloc	(fi_, li_, fu_, s_)
#define	MEMTRACE_COMFREE(fi_,li_,fu_,p_)	debug_memtrace_CoTaskMemFree	(fi_, li_, fu_, p_)
#define	MEMTRACE_COMREALLOC(fi_,li_,fu_,p_,s_)	debug_memtrace_CoTaskRealloc	(fi_, li_, fu_, p_, s_)

#else	/*! DEBUG_MEMTRACE */

#define	MEMTRACE_INIT(log_)
#define	MEMTRACE_MARK(mark_)

#define	MEMTRACE_MALLOC(fi_,li_,fu_,s_)		malloc (s_)
#define	MEMTRACE_CALLOC(fi_,li_,fu_,n_,s_)	calloc (n_,s_)
#define	MEMTRACE_FREE(fi_,li_,fu_,p_)		free (p_)
#define	MEMTRACE_REALLOC(fi_,li_,fu_,p_,s_)	realloc (p_,s_)
#define	MEMTRACE_STRDUP(fi_,li_,fu_,p_)		strdup (p_)
#define	MEMTRACE_COMMALLOC(fi_,li_,fu_,s_)	CoTaskMemAlloc (s_)
#define	MEMTRACE_COMFREE(fi_,li_,fu_,p_)	CoTaskMemFree (p_)
#define	MEMTRACE_COMREALLOC(fi_,li_,fu_,p_,s_)	CoTaskMemRealloc (p_,s_)

#endif	/*! DEBUG_MEMTRACE */

#ifdef __cplusplus

extern "C" {
#endif

DEBUG_MEMTRACE_API void		debug_memtrace_init		(const char *logf);
DEBUG_MEMTRACE_API void		debug_memtrace_mark		(const char *mark);

DEBUG_MEMTRACE_API void *	debug_memtrace_malloc		(const char *file, unsigned long line, const char *func, size_t s);
DEBUG_MEMTRACE_API void *	debug_memtrace_calloc		(const char *file, unsigned long line, const char *func, size_t n, size_t s);
DEBUG_MEMTRACE_API void		debug_memtrace_free		(const char *file, unsigned long line, const char *func, void *p);
DEBUG_MEMTRACE_API void *	debug_memtrace_realloc		(const char *file, unsigned long line, const char *func, void *p, size_t s);
DEBUG_MEMTRACE_API char *	debug_memtrace_strdup		(const char *file, unsigned long line, const char *func, const char *p);
DEBUG_MEMTRACE_API void *	debug_memtrace_CoTaskMemAlloc	(const char *file, unsigned long line, const char *func, unsigned long s);
DEBUG_MEMTRACE_API void		debug_memtrace_CoTaskMemFree	(const char *file, unsigned long line, const char *func, void *p);
DEBUG_MEMTRACE_API void *	debug_memtrace_CoTaskMemRealloc	(const char *file, unsigned long line, const char *func, void *p, unsigned long s);
DEBUG_MEMTRACE_API void		debug_memtrace_deletefree	(void *p);
DEBUG_MEMTRACE_API void		debug_memtrace_deletefreearray	(void *p);

#ifdef __cplusplus
}

class DEBUG_MEMTRACE_API debug_memtrace_new
{
	const char	*file;
	unsigned long	line;
	const char	*func;

	void process	(void *pointer, unsigned long size);

public:
	debug_memtrace_new	(const char *file, unsigned long line, const char *func);

	template <class T> T* operator->*(T* pointer) { process(pointer, sizeof(T)); return pointer; }

private:
	debug_memtrace_new (const debug_memtrace_new&);
	debug_memtrace_new& operator= (const debug_memtrace_new&);
};

class DEBUG_MEMTRACE_API debug_memtrace_delete
{
	const char	*file;
	unsigned long	line;
	const char	*func;

	void process	(void *pointer);

public:
	debug_memtrace_delete	(const char *file, unsigned long line, const char *func);

private:
	debug_memtrace_delete (const debug_memtrace_delete&);
	debug_memtrace_delete& operator= (const debug_memtrace_delete&);
};

#endif

#endif	/* DEBUG_MEMTRACE_H */