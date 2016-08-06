/** \file
 * Slay's Library - Message handling codes
 * 
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */ 

#ifndef	protection____INCLUDE_SL_MESSAGE_IDS_H
#define	protection____INCLUDE_SL_MESSAGE_IDS_H	1
#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */



/*! Message types */
typedef enum e_SLMSG_TYPE {
	SLMSG_TYPE_START = 0,                        		/*!< \internal start code         */
	SLMSG_TYPE_STATE = 0,                        		/*!< state info                   */
	SLMSG_TYPE_EXIT,                             		/*!< application exit info        */
	SLMSG_TYPE_ERRSTR,                           		/*!< streamed error info          */
	SLMSG_TYPE_UI_INFO,                          		/*!< UI: informational message    */
	SLMSG_TYPE_ANY,                              		/*!< *ANY*                        */
	SLMSG_TYPE_LIMIT = SLMSG_TYPE_ANY            		/*!< \internal final code         */
} SLMSG_TYPE;
#define	SLMSG_TYPE_CNT	(SLMSG_TYPE_LIMIT - SLMSG_TYPE_START + 1)

/*! Message types lookup function
 *
 * \param e	Message types
 * \return	Pointer to const string with Message types name
 */
extern const char *SLMSG_TYPE_lookup (SLMSG_TYPE e);
/*! Message types code lookup function
 *
 * \param e	Message types
 * \return	Pointer to const string with Message types code
 */
extern const char *SLMSG_TYPE_lookup_code (SLMSG_TYPE e);



/*! Message senders */
typedef enum e_SLMSG_FROM {
	SLMSG_FROM_START = 0,                        		/*!< \internal start code                   */
	SLMSG_FROM_SLMSG = 0,                        		/*!< message subsystem                      */
	SLMSG_FROM_STDBG,                            		/*!< state debug subsystem                  */
	SLMSG_FROM_SLBO,                             		/*!< basic output subsystem                 */
	SLMSG_FROM_EXIT,                             		/*!< application exit handling subsystem    */
	SLMSG_FROM_ERROR,                            		/*!< error handling subsystem               */
	SLMSG_FROM_ANY,                              		/*!< *ANY*                                  */
	SLMSG_FROM_LIMIT = SLMSG_FROM_ANY            		/*!< \internal final code                   */
} SLMSG_FROM;
#define	SLMSG_FROM_CNT	(SLMSG_FROM_LIMIT - SLMSG_FROM_START + 1)

/*! Message senders lookup function
 *
 * \param e	Message senders
 * \return	Pointer to const string with Message senders name
 */
extern const char *SLMSG_FROM_lookup (SLMSG_FROM e);
/*! Message senders code lookup function
 *
 * \param e	Message senders
 * \return	Pointer to const string with Message senders code
 */
extern const char *SLMSG_FROM_lookup_code (SLMSG_FROM e);



/*! Message priorities */
typedef enum e_SLMSG_PRIO {
	SLMSG_PRIO_START = 0,                        		/*!< \internal start code     */
	SLMSG_PRIO_DEFAULT = 0,                      		/*!< *default priority*       */
	SLMSG_PRIO_PANIC,                            		/*!< system panic             */
	SLMSG_PRIO_CRITICAL,                         		/*!< critical message         */
	SLMSG_PRIO_ERROR,                            		/*!< error message            */
	SLMSG_PRIO_WARNING,                          		/*!< warning message          */
	SLMSG_PRIO_NOTICE,                           		/*!< notification             */
	SLMSG_PRIO_INFO,                             		/*!< informational message    */
	SLMSG_PRIO_DEBUG,                            		/*!< debug message            */
	SLMSG_PRIO_TRACE,                            		/*!< trace message            */
	SLMSG_PRIO_DEEPTRACE,                        		/*!< deeptrace message        */
	SLMSG_PRIO_LIMIT = SLMSG_PRIO_DEEPTRACE      		/*!< \internal final code     */
} SLMSG_PRIO;
#define	SLMSG_PRIO_CNT	(SLMSG_PRIO_LIMIT - SLMSG_PRIO_START + 1)

/*! Message priorities lookup function
 *
 * \param e	Message priorities
 * \return	Pointer to const string with Message priorities name
 */
extern const char *SLMSG_PRIO_lookup (SLMSG_PRIO e);
/*! Message priorities code lookup function
 *
 * \param e	Message priorities
 * \return	Pointer to const string with Message priorities code
 */
extern const char *SLMSG_PRIO_lookup_code (SLMSG_PRIO e);



/*! Messages: deeptrace data codes */
typedef enum e_SLMSG_DATA_CODE_DT {
	SLMSG_DATA_CODE_DT_START = 0,                		/*!< \internal start code */
	SLMSG_DATA_CODE_DT_NORMAL = 0,               		/*!< simple event         */
	SLMSG_DATA_CODE_DT_ENTRY,                    		/*!< scope entry event    */
	SLMSG_DATA_CODE_DT_EXIT,                     		/*!< scope exit event     */
	SLMSG_DATA_CODE_DT_LIMIT = SLMSG_DATA_CODE_DT_EXIT		/*!< \internal final code */
} SLMSG_DATA_CODE_DT;
#define	SLMSG_DATA_CODE_DT_CNT	(SLMSG_DATA_CODE_DT_LIMIT - SLMSG_DATA_CODE_DT_START + 1)

/*! Messages: deeptrace data codes lookup function
 *
 * \param e	Messages: deeptrace data codes
 * \return	Pointer to const string with Messages: deeptrace data codes name
 */
extern const char *SLMSG_DATA_CODE_DT_lookup (SLMSG_DATA_CODE_DT e);
/*! Messages: deeptrace data codes code lookup function
 *
 * \param e	Messages: deeptrace data codes
 * \return	Pointer to const string with Messages: deeptrace data codes code
 */
extern const char *SLMSG_DATA_CODE_DT_lookup_code (SLMSG_DATA_CODE_DT e);



/*! Messages: data types */
typedef enum e_SLMSG_DATA_TYPE {
	SLMSG_DATA_TYPE_START = 0,                   		/*!< \internal start code  */
	SLMSG_DATA_TYPE_STRING = 0,                  		/*!< simple string data    */
	SLMSG_DATA_TYPE_PRINTF,                      		/*!< printf data           */
	SLMSG_DATA_TYPE_CMD,                         		/*!< command               */
	SLMSG_DATA_TYPE_LIMIT = SLMSG_DATA_TYPE_CMD  		/*!< \internal final code  */
} SLMSG_DATA_TYPE;
#define	SLMSG_DATA_TYPE_CNT	(SLMSG_DATA_TYPE_LIMIT - SLMSG_DATA_TYPE_START + 1)

/*! Messages: data types lookup function
 *
 * \param e	Messages: data types
 * \return	Pointer to const string with Messages: data types name
 */
extern const char *SLMSG_DATA_TYPE_lookup (SLMSG_DATA_TYPE e);



/*! Messages: commands */
typedef enum e_SLMSG_CMD {
	SLMSG_CMD_START = 0,                         		/*!< \internal start code */
	SLMSG_CMD_FLUSH = 0,                         		/*!< flush buffers        */
	SLMSG_CMD_LIMIT = SLMSG_CMD_FLUSH            		/*!< \internal final code */
} SLMSG_CMD;
#define	SLMSG_CMD_CNT	(SLMSG_CMD_LIMIT - SLMSG_CMD_START + 1)

/*! Messages: commands lookup function
 *
 * \param e	Messages: commands
 * \return	Pointer to const string with Messages: commands name
 */
extern const char *SLMSG_CMD_lookup (SLMSG_CMD e);



/*! Messages: handler return codes */
typedef enum e_SLMSG_RC {
	SLMSG_RC_START = 0,                          		/*!< \internal start code               */
	SLMSG_RC_OK = 0,                             		/*!< message handled ok                 */
	SLMSG_RC_FAIL,                               		/*!< failed to handle message           */
	SLMSG_RC_PASS,                               		/*!< pass message on to next handler    */
	SLMSG_RC_LIMIT = SLMSG_RC_PASS               		/*!< \internal final code               */
} SLMSG_RC;
#define	SLMSG_RC_CNT	(SLMSG_RC_LIMIT - SLMSG_RC_START + 1)

/*! Messages: handler return codes lookup function
 *
 * \param e	Messages: handler return codes
 * \return	Pointer to const string with Messages: handler return codes name
 */
extern const char *SLMSG_RC_lookup (SLMSG_RC e);
/*! Messages: handler return codes code lookup function
 *
 * \param e	Messages: handler return codes
 * \return	Pointer to const string with Messages: handler return codes code
 */
extern const char *SLMSG_RC_lookup_code (SLMSG_RC e);



/*! Messages: handler modes */
typedef enum e_SLMSG_MODE {
	SLMSG_MODE_START = 0,                        		/*!< \internal start code */
	SLMSG_MODE_NONE = 0,                         		/*!< *none*               */
	SLMSG_MODE_NORMAL,                           		/*!< normal               */
	SLMSG_MODE_EXCLUSIVE,                        		/*!< exclusive            */
	SLMSG_MODE_LIMIT = SLMSG_MODE_EXCLUSIVE      		/*!< \internal final code */
} SLMSG_MODE;
#define	SLMSG_MODE_CNT	(SLMSG_MODE_LIMIT - SLMSG_MODE_START + 1)

/*! Messages: handler modes lookup function
 *
 * \param e	Messages: handler modes
 * \return	Pointer to const string with Messages: handler modes name
 */
extern const char *SLMSG_MODE_lookup (SLMSG_MODE e);



/*! Messages: handler attributes (ID) */
typedef enum e_SLMSG_ATTR_ID {
	SLMSG_ATTR_ID_START = 0,                     		/*!< \internal start code      */
	SLMSG_ATTR_ID_NONE = 0,                      		/*!< *none*                    */
	SLMSG_ATTR_ID_ERR,                           		/*!< output to error stream    */
	SLMSG_ATTR_ID_FILE,                          		/*!< output to file            */
	SLMSG_ATTR_ID_LIMIT = SLMSG_ATTR_ID_FILE     		/*!< \internal final code      */
} SLMSG_ATTR_ID;
#define	SLMSG_ATTR_ID_CNT	(SLMSG_ATTR_ID_LIMIT - SLMSG_ATTR_ID_START + 1)

/*! Messages: handler attributes (ID) lookup function
 *
 * \param e	Messages: handler attributes (ID)
 * \return	Pointer to const string with Messages: handler attributes (ID) name
 */
extern const char *SLMSG_ATTR_ID_lookup (SLMSG_ATTR_ID e);



/*! Messages: handler attributes (flag) */
typedef unsigned long SLMSG_ATTR;

#define	SLMSG_ATTR_NONE	(0x00000000)             		/*!< *none*                    */
#define	SLMSG_ATTR_ERR 	(1 << SLMSG_ATTR_ID_ERR) 		/*!< output to error stream    */
#define	SLMSG_ATTR_FILE	(1 << SLMSG_ATTR_ID_FILE)		/*!< output to file            */



#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif	/* protection____INCLUDE_SL_MESSAGE_IDS_H */
