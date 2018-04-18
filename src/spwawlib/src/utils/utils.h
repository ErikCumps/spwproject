/** \file
 * The SPWaW Library - utility code.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	UTILS_H
#define	UTILS_H	1

#include "strtab/strtab.h"
#include "utils/azs.h"

#define	safe_malloc(type_)		(type_ *)safe_malloc_core (__FILE__, __LINE__, __FUNCTION__, sizeof (type_), #type_)
#define	safe_nmalloc(type_, cnt_)	(type_ *)safe_malloc_core (__FILE__, __LINE__, __FUNCTION__, cnt_ * sizeof (type_), #type_)
#define	safe_smalloc(type_, size_)	(type_ *)safe_malloc_core (__FILE__, __LINE__, __FUNCTION__, size_, #type_)
#define	safe_free(ptr_)			safe_free_core (__FILE__, __LINE__, __FUNCTION__, (void **)&(ptr_))
#define	safe_realloc(ptr_, size_)	safe_realloc_core (__FILE__, __LINE__, __FUNCTION__, (void *)ptr_, size_);
#define	clear_ptr(ptr_)			fill_ptr_core (ptr_, sizeof (*ptr_), 0)
#define	fill_ptr(ptr_, val_)		fill_ptr_core (ptr_, sizeof (*ptr_), val_)
#define	azstrcpy(src_, dst_)		sazscpy (src_,dst_)
#define	azstrstab(src_, stab_)		azstrstab_core ((char *)src_, sizeof (src_), stab_)


extern void *	safe_malloc_core	(const char *file, unsigned long line, const char *func, unsigned int size, const char *name);
extern void	safe_free_core		(const char *file, unsigned long line, const char *func, void **ptr);
extern void *	safe_realloc_core	(const char *file, unsigned long line, const char *func, void *ptr, unsigned int size);
extern void	fill_ptr_core		(void *ptr, unsigned int size, int val);
extern char *	azstrstab_core		(char *src, unsigned int size, STRTAB *stab);


extern void *	mmapfile		(int fd, HANDLE *maph);
extern void	unmmapfile		(void **map, HANDLE *maph);

/*! Simple buffered IO class */
class UtilStrbuf
{
public:
	UtilStrbuf	(char *buf, unsigned long len, bool init, bool nl);
	~UtilStrbuf	(void);

	bool	full	(void);
	void	add	(char *string);
	void	printf	(char *fmt, ...);

private:
	void	update	(void);

private:
	char		*buf;
	unsigned long	size;
	char		*ptr;
	unsigned long	left;
	bool		eot_nl;
};

#endif	/* UTILS_H */
