/** \file
 * Slay's Library - event callback and hook prototypes.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SL_CALLBACKS_H
#define	SL_CALLBACKS_H	1

#include <SL/bool.h>
#include <SL/error.h>

#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */



// TODO: should implement separate event handling thread to synchronize
// event handling for multiple threads.

/*** Event handlers ***/

/*! Callback result */
typedef enum e_SL_EVT_RC {
	SL_EVT_HANDLE = 0,	/*!< SL must handle event	*/
	SL_EVT_CONTINUE,	/*!< SL may continue execution	*/
	SL_EVT_RETRY,		/*!< SL may retry execution	*/
} SL_EVT_RC;

/*! Abort handler: allows client code to handle SL abort */
typedef SL_EVT_RC (*SL_EVT_ABORT)(const char *file, unsigned long line, const char *func, const char *msg);

/*! Error handler: allows client code to handle SL error */
typedef SL_ERROR_REQUEST (*SL_EVT_ERROR)(SL_ERROR_DATA *stack);



/*** Event hooks ***/

/*! Out-Of-Memory hook: invoked on SL out of memory condition */
typedef void (*SL_HOOK_OOM)(const char *file, unsigned long line, const char *func, unsigned long size);

/*! Panic hook: invoked on SL panic */
typedef void (*SL_HOOK_PANIC)(void);

/*! Error hook: invoked on SL error push */
typedef void (*SL_HOOK_ERROR)(void);

/*! Exit hook: invoked on SL application exit */
typedef void (*SL_HOOK_EXIT)(unsigned int ec, char *msg, SL_BOOL dump);



#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif	/* SL_CALLBACKS_H */

