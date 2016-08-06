/** \file
 * Slay's Library - error handling: private data types.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef SL_ERROR_PRIVATE_H
#define SL_ERROR_PRIVATE_H	1

#include <SL/error.h>



/*! Error code information structure	*/
typedef struct s_SL_ERROR_CODE {
	int			ec;				/*!< Error code					*/
	char			msg[SL_ERROR_STRSIZE+1];	/*!< Error message				*/
	struct s_SL_ERROR_CODE	*next;				/*!< Pointer to next error code			*/
} SL_ERROR_CODE;

/*! Error code collection */
typedef struct s_SL_ERROR_SET {
	int			 max;				/*!< Biggest error code in this collection	*/
	SL_ERROR_CODE		*head;				/*!< Pointer to first error code info		*/
	SL_ERROR_CODE		*tail;				/*!< Pointer to last error code info		*/
} SL_ERROR_SET;



#endif /* SL_ERROR_PRIVATE_H */
