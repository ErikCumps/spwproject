/** \file
 * Slay's Library - Initialization codes
 * 
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */ 

#ifndef	protection____INCLUDE_SL_INIT_IDS_H
#define	protection____INCLUDE_SL_INIT_IDS_H	1
#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */



/*! SL_init error code */
typedef enum e_SL_INIT {
	SL_INIT_START = 0,                           		/*!< \internal start code                                     */
	SL_INIT_OK = 0,                              		/*!< no error                                                 */
	SL_INIT_SLOPTS,                              		/*!< failed to initialize library options                     */
	SL_INIT_SLMSG,                               		/*!< failed to initialize message handling                    */
	SL_INIT_SLBO,                                		/*!< failed to initialize basic output message intercept      */
	SL_INIT_STDBG,                               		/*!< failed to initialize state debug reporting               */
	SL_INIT_THREAD,                              		/*!< failed to initialize thread handling                     */
	SL_INIT_APP,                                 		/*!< failed to initialize application information handling    */
	SL_INIT_EXIT,                                		/*!< failed to initialize application exit handling           */
	SL_INIT_ERROR,                               		/*!< failed to initialize error handling                      */
	SL_INIT_LIMIT = SL_INIT_ERROR                		/*!< \internal final code                                     */
} SL_INIT;
#define	SL_INIT_CNT	(SL_INIT_LIMIT - SL_INIT_START + 1)

/*! SL_init error code lookup function
 *
 * \param e	SL_init error code
 * \return	Pointer to const string with SL_init error code name
 */
extern const char *SL_INIT_lookup (SL_INIT e);



/*! Subsystem initialization error code */
typedef enum e_SL_SINIT {
	SL_SINIT_START = 0,                          		/*!< \internal start code             */
	SL_SINIT_OK = 0,                             		/*!< no error                         */
	SL_SINIT_FAIL,                               		/*!< failed                           */
	SL_SINIT_REINIT,                             		/*!< subsystem already initialized    */
	SL_SINIT_LIMIT = SL_SINIT_REINIT             		/*!< \internal final code             */
} SL_SINIT;
#define	SL_SINIT_CNT	(SL_SINIT_LIMIT - SL_SINIT_START + 1)

/*! Subsystem initialization error code lookup function
 *
 * \param e	Subsystem initialization error code
 * \return	Pointer to const string with Subsystem initialization error code name
 */
extern const char *SL_SINIT_lookup (SL_SINIT e);



#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif	/* protection____INCLUDE_SL_INIT_IDS_H */
