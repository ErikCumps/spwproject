/** \file
 * Slays' Library - lock handling.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <SL/system.h>
#include <SL/safe_mem.h>
#include <SL/lock.h>
#include "magic.h"



/* --- private data types and structures --- */

/*! Lock structure */
struct SL_LOCK_struct {
	MAGIC			magic;	/*!< Structure magic number	*/
	CRITICAL_SECTION	*CS;	/*!< Critical section		*/
};



/* --- API --- */

/*! Creates new lock
 *
 * \return	new lock
 */
SL_LOCK *
SL_LOCK_new (void)
{
	struct SL_LOCK_struct	*p1;
	CRITICAL_SECTION	*p2;

	SL_SAFE_CALLOC (p1, 1, sizeof (struct SL_LOCK_struct));
	SL_SAFE_CALLOC (p2, 1, sizeof (CRITICAL_SECTION));

	p1->magic = MAGIC_LOCK;
	p1->CS    = p2;
	InitializeCriticalSection (p1->CS);
	return ((SL_LOCK *)p1);
}

/*! Acquires lock
 *
 * \param lock	lock to acquire
 */
void
SL_LOCK_take (SL_LOCK *lock)
{
	ASSERT (lock != NULL);
	ASSERT (lock->magic == MAGIC_LOCK);

	EnterCriticalSection (lock->CS);
}

/*! Releases acquired lock
 *
 * \param lock	lock to release
 */
void
SL_LOCK_drop (SL_LOCK *lock)
{
	ASSERT (lock != NULL);
	ASSERT (lock->magic == MAGIC_LOCK);

	LeaveCriticalSection (lock->CS);
}

/*! Frees lock
 *
 * \param lock	pointer to lock to free
 */

void
SL_LOCK_free (SL_LOCK **lock)
{
	ASSERT (lock != NULL);
	ASSERT (*lock != NULL);
	ASSERT ((*lock)->magic == MAGIC_LOCK);

	DeleteCriticalSection ((*lock)->CS);
	SL_SAFE_FREE ((*lock)->CS);
	SL_SAFE_FREE (*lock);
}
