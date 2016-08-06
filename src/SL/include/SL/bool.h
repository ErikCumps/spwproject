/** \file
 * Slay's Library - boolean data type.
 *
 * Copyright (C) 2003-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SL_BOOL_H
#define	SL_BOOL_H	1
#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */



/* --- Data types --- */

/*! Basic boolean data */
typedef enum e_SL_BOOL {
	SL_false = 0,	/*!< FALSE value	*/
	SL_true  = 1	/*!< TRUE value		*/
} SL_BOOL;



/* --- API --- */

extern const char*	SL_BOOL_tostr	(SL_BOOL abool);
extern void		SL_BOOL_toggle	(SL_BOOL *abool);


#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif	/* SL_BOOL_H */
