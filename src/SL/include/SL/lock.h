/** \file
 * Slay's Library - lock handling.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SL_LOCK_H
#define	SL_LOCK_H	1
#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */



/* --- Data types --- */

struct SL_LOCK_struct;

/*! Opaque lock data type */
typedef struct SL_LOCK_struct SL_LOCK;



/* --- API --- */

extern SL_LOCK *SL_LOCK_new	(void);
extern void	SL_LOCK_take	(SL_LOCK *lock);
extern void	SL_LOCK_drop	(SL_LOCK *lock);
extern void	SL_LOCK_free	(SL_LOCK **lock);



#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif	/* SL_BOOL_H */
