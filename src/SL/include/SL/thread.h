/** \file
 * Slay's Library - thread handling.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef SL_THREAD_H
#define SL_THREAD_H	1

#include <SL/init_ids.h>
#include <SL/bool.h>

#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */



/* --- structures and types --- */

typedef void (*SL_THREAD_TLS_INIT) (DWORD tls);
typedef void (*SL_THREAD_TLS_EXIT) (DWORD tls);

/* --- API --- */

extern SL_SINIT	SL_THREAD_init		(void);
extern void	SL_THREAD_shutdown	(void);
extern SL_BOOL	SL_THREAD_tlsalloc	(DWORD *tls, SL_THREAD_TLS_INIT init, SL_THREAD_TLS_EXIT exit);
extern void	SL_THREAD_tlsfree	(DWORD tls);
extern HANDLE	SL_THREAD_create	(LPTHREAD_START_ROUTINE func, void *arg, DWORD stack, DWORD flags, DWORD *tid);



#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif /* SL_THREAD_H */
