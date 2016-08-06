/** \file
 * Slay's Library - Initialization codes
 * 
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */ 

#include <SL/init_ids.h>



/*! SL_init error code */
static const char *SL_INIT_names[SL_INIT_LIMIT+1] = {
	"no error",                                             		/*!< SL_INIT_OK        */
	"failed to initialize library options",                 		/*!< SL_INIT_SLOPTS    */
	"failed to initialize message handling",                		/*!< SL_INIT_SLMSG     */
	"failed to initialize basic output message intercept",  		/*!< SL_INIT_SLBO      */
	"failed to initialize state debug reporting",           		/*!< SL_INIT_STDBG     */
	"failed to initialize thread handling",                 		/*!< SL_INIT_THREAD    */
	"failed to initialize application information handling",		/*!< SL_INIT_APP       */
	"failed to initialize application exit handling",       		/*!< SL_INIT_EXIT      */
	"failed to initialize error handling",                  		/*!< SL_INIT_ERROR     */
};

/*! SL_init error code lookup function
 *
 * \param e	SL_init error code
 * \return	Pointer to const string with SL_init error code name
 */
const char *
SL_INIT_lookup (SL_INIT e)
{
	return (SL_INIT_names[e]);
}



/*! Subsystem initialization error code */
static const char *SL_SINIT_names[SL_SINIT_LIMIT+1] = {
	"no error",                                  		/*!< SL_SINIT_OK        */
	"failed",                                    		/*!< SL_SINIT_FAIL      */
	"subsystem already initialized",             		/*!< SL_SINIT_REINIT    */
};

/*! Subsystem initialization error code lookup function
 *
 * \param e	Subsystem initialization error code
 * \return	Pointer to const string with Subsystem initialization error code name
 */
const char *
SL_SINIT_lookup (SL_SINIT e)
{
	return (SL_SINIT_names[e]);
}



