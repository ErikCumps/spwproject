/** \file
 * Slay's Library - library initialization options.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SL_OPT_H
#define	SL_OPT_H	1

#include <SL/callbacks.h>
#include <SL/error.h>

#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */

typedef struct {
	struct {
		SL_EVT_ABORT		abort;
		SL_EVT_ERROR		error;
	} ecb;
	struct {
		SL_HOOK_OOM		oom;
		SL_HOOK_PANIC		panic;
		SL_HOOK_ERROR		error;
		SL_HOOK_EXIT		exit;
	} hook;
} SLOPT;

#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif	/* SL_OPT_H */