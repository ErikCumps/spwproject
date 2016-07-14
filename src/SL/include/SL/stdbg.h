/** \file
 * Slay's Library - state debug reporting.
 *
 * Copyright (C) 2003-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SL_STDBG_H
#define	SL_STDBG_H	1
#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */



/* --- structures and types --- */

/*! Info levels used for state debug reporting */
typedef enum {
	SL_STDBG_LEVEL_BAS,	/*!< Basic state information		*/
	SL_STDBG_LEVEL_EXT,	/*!< Extended state information		*/
	SL_STDBG_LEVEL_DEEP	/*!< Deep probed state information	*/
} SL_STDBG_INFO_LEVEL;

/*! State debug report function type */
typedef void (*SL_STDBG_REPORT)(SL_STDBG_INFO_LEVEL level);



/* --- API --- */

extern void	SL_STDBG_add		(SL_STDBG_REPORT func, const char *name);
extern void	SL_STDBG_delete		(SL_STDBG_REPORT func);
extern void	SL_STDBG_report		(SL_STDBG_INFO_LEVEL level);
extern void	SL_STDBG_say		(const char *src, char *fmt, ...);

/*! Reports state information
 *
 * \note This macro expects const char *MODULE to be present
 */
#define	SAYSTATE0(_fmt)					SL_STDBG_say (MODULE, _fmt)
#define	SAYSTATE1(_fmt, _a1)				SL_STDBG_say (MODULE, _fmt, _a1)
#define	SAYSTATE2(_fmt, _a1, _a2)			SL_STDBG_say (MODULE, _fmt, _a1, _a2)
#define	SAYSTATE3(_fmt, _a1, _a2, _a3)			SL_STDBG_say (MODULE, _fmt, _a1, _a2, _a3)
#define	SAYSTATE4(_fmt, _a1, _a2, _a3, _a4)		SL_STDBG_say (MODULE, _fmt, _a1, _a2, _a3, _a4)
#define	SAYSTATE5(_fmt, _a1, _a2, _a3, _a4, _a5)	SL_STDBG_say (MODULE, _fmt, _a1, _a2, _a3, _a4, _a5)
#define	SAYSTATE6(_fmt, _a1, _a2, _a3, _a4, _a5, _a6)	SL_STDBG_say (MODULE, _fmt, _a1, _a2, _a3, _a4, _a5, _a6)



#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif	/* SL_STDBG_H */

