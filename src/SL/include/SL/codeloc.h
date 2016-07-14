/** \file
 * Slay's Library - debug support: code location.
 *
 * Copyright (C) 2006-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef SL_CODELOC_H
#define SL_CODELOC_H	1
#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */

#define	SL_CODELOC_STRSIZE	256

/*! Code location structure */
typedef struct {
	char		file[SL_CODELOC_STRSIZE+1];	/*!< Source file name				*/
	unsigned long	line;				/*!< Line in file				*/
	char		func[SL_CODELOC_STRSIZE+1];	/*!< Name of function				*/
} SL_CODELOC;



/***** Function prototypes *****/

/* Handle code location */
extern SL_CODELOC	*SL_HERE_I_AM_core	(const char *file, unsigned long line, const char *func);
extern void		SL_FREE_CODELOC_core	(SL_CODELOC **here);
extern void		SL_CODELOC_set		(SL_CODELOC *loc, const char *file, unsigned long line, const char *func);
extern void		SL_CODELOC_copy		(SL_CODELOC *dst, SL_CODELOC *src);



/***** Macro definitions *****/

/*! Registers code location */
#define	SL_HERE_I_AM		SL_HERE_I_AM_core (__FILE__, __LINE__, __FUNCTION__)

/*! Frees registered code location
 * \param _here   Code location to free
 */
#define	SL_FREE_CODELOC(_here)	SL_FREE_CODELOC_core (&(_here))



#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif /* SL_CODELOC_H */
