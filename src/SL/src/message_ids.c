/** \file
 * Slay's Library - Message handling codes
 * 
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */ 

#include <SL/message_ids.h>



/*! Message types */
static const char *SLMSG_TYPE_names[SLMSG_TYPE_LIMIT+1] = {
	"state info",                                		/*!< SLMSG_TYPE_STATE      */
	"application exit info",                     		/*!< SLMSG_TYPE_EXIT       */
	"streamed error info",                       		/*!< SLMSG_TYPE_ERRSTR     */
	"UI: informational message",                 		/*!< SLMSG_TYPE_UI_INFO    */
	"*ANY*",                                     		/*!< SLMSG_TYPE_ANY        */
};

/*! Message types lookup function
 *
 * \param e	Message types
 * \return	Pointer to const string with Message types name
 */
const char *
SLMSG_TYPE_lookup (SLMSG_TYPE e)
{
	return (SLMSG_TYPE_names[e]);
}



/*! Message types: code strings */
static const char *SLMSG_TYPE_codes[SLMSG_TYPE_LIMIT+1] = {
	"STATE",                                     		/*!< SLMSG_TYPE_STATE      */
	"EXIT",                                      		/*!< SLMSG_TYPE_EXIT       */
	"ERRSTR",                                    		/*!< SLMSG_TYPE_ERRSTR     */
	"UI_INFO",                                   		/*!< SLMSG_TYPE_UI_INFO    */
	"*ANY*",                                     		/*!< SLMSG_TYPE_ANY        */
};

/*! Message types code lookup function
 *
 * \param e	Message types
 * \return	pointer to const string with Message types code
 */
const char *
SLMSG_TYPE_lookup_code (SLMSG_TYPE e)
{
	return (SLMSG_TYPE_codes[e]);
}



/*! Message senders */
static const char *SLMSG_FROM_names[SLMSG_FROM_LIMIT+1] = {
	"message subsystem",                         		/*!< SLMSG_FROM_SLMSG    */
	"state debug subsystem",                     		/*!< SLMSG_FROM_STDBG    */
	"basic output subsystem",                    		/*!< SLMSG_FROM_SLBO     */
	"application exit handling subsystem",       		/*!< SLMSG_FROM_EXIT     */
	"error handling subsystem",                  		/*!< SLMSG_FROM_ERROR    */
	"*ANY*",                                     		/*!< SLMSG_FROM_ANY      */
};

/*! Message senders lookup function
 *
 * \param e	Message senders
 * \return	Pointer to const string with Message senders name
 */
const char *
SLMSG_FROM_lookup (SLMSG_FROM e)
{
	return (SLMSG_FROM_names[e]);
}



/*! Message senders: code strings */
static const char *SLMSG_FROM_codes[SLMSG_FROM_LIMIT+1] = {
	"SLMSG",                                     		/*!< SLMSG_FROM_SLMSG    */
	"SL_STDBG",                                  		/*!< SLMSG_FROM_STDBG    */
	"SLBO",                                      		/*!< SLMSG_FROM_SLBO     */
	"SL_EXIT",                                   		/*!< SLMSG_FROM_EXIT     */
	"SL_ERROR",                                  		/*!< SLMSG_FROM_ERROR    */
	"*ANY*",                                     		/*!< SLMSG_FROM_ANY      */
};

/*! Message senders code lookup function
 *
 * \param e	Message senders
 * \return	pointer to const string with Message senders code
 */
const char *
SLMSG_FROM_lookup_code (SLMSG_FROM e)
{
	return (SLMSG_FROM_codes[e]);
}



/*! Message priorities */
static const char *SLMSG_PRIO_names[SLMSG_PRIO_LIMIT+1] = {
	"*default priority*",                        		/*!< SLMSG_PRIO_DEFAULT      */
	"system panic",                              		/*!< SLMSG_PRIO_PANIC        */
	"critical message",                          		/*!< SLMSG_PRIO_CRITICAL     */
	"error message",                             		/*!< SLMSG_PRIO_ERROR        */
	"warning message",                           		/*!< SLMSG_PRIO_WARNING      */
	"notification",                              		/*!< SLMSG_PRIO_NOTICE       */
	"informational message",                     		/*!< SLMSG_PRIO_INFO         */
	"debug message",                             		/*!< SLMSG_PRIO_DEBUG        */
	"trace message",                             		/*!< SLMSG_PRIO_TRACE        */
	"deeptrace message",                         		/*!< SLMSG_PRIO_DEEPTRACE    */
};

/*! Message priorities lookup function
 *
 * \param e	Message priorities
 * \return	Pointer to const string with Message priorities name
 */
const char *
SLMSG_PRIO_lookup (SLMSG_PRIO e)
{
	return (SLMSG_PRIO_names[e]);
}



/*! Message priorities: code strings */
static const char *SLMSG_PRIO_codes[SLMSG_PRIO_LIMIT+1] = {
	"  ",                                        		/*!< SLMSG_PRIO_DEFAULT      */
	"!!",                                        		/*!< SLMSG_PRIO_PANIC        */
	"! ",                                        		/*!< SLMSG_PRIO_CRITICAL     */
	"**",                                        		/*!< SLMSG_PRIO_ERROR        */
	"* ",                                        		/*!< SLMSG_PRIO_WARNING      */
	"+ ",                                        		/*!< SLMSG_PRIO_NOTICE       */
	"+ ",                                        		/*!< SLMSG_PRIO_INFO         */
	"  ",                                        		/*!< SLMSG_PRIO_DEBUG        */
	"  ",                                        		/*!< SLMSG_PRIO_TRACE        */
	"  ",                                        		/*!< SLMSG_PRIO_DEEPTRACE    */
};

/*! Message priorities code lookup function
 *
 * \param e	Message priorities
 * \return	pointer to const string with Message priorities code
 */
const char *
SLMSG_PRIO_lookup_code (SLMSG_PRIO e)
{
	return (SLMSG_PRIO_codes[e]);
}



/*! Messages: deeptrace data codes */
static const char *SLMSG_DATA_CODE_DT_names[SLMSG_DATA_CODE_DT_LIMIT+1] = {
	"simple event",                              		/*!< SLMSG_DATA_CODE_DT_NORMAL    */
	"scope entry event",                         		/*!< SLMSG_DATA_CODE_DT_ENTRY     */
	"scope exit event",                          		/*!< SLMSG_DATA_CODE_DT_EXIT      */
};

/*! Messages: deeptrace data codes lookup function
 *
 * \param e	Messages: deeptrace data codes
 * \return	Pointer to const string with Messages: deeptrace data codes name
 */
const char *
SLMSG_DATA_CODE_DT_lookup (SLMSG_DATA_CODE_DT e)
{
	return (SLMSG_DATA_CODE_DT_names[e]);
}



/*! Messages: deeptrace data codes: code strings */
static const char *SLMSG_DATA_CODE_DT_codes[SLMSG_DATA_CODE_DT_LIMIT+1] = {
	"",                                          		/*!< SLMSG_DATA_CODE_DT_NORMAL    */
	"[ENTRY]",                                   		/*!< SLMSG_DATA_CODE_DT_ENTRY     */
	"[EXIT-]",                                   		/*!< SLMSG_DATA_CODE_DT_EXIT      */
};

/*! Messages: deeptrace data codes code lookup function
 *
 * \param e	Messages: deeptrace data codes
 * \return	pointer to const string with Messages: deeptrace data codes code
 */
const char *
SLMSG_DATA_CODE_DT_lookup_code (SLMSG_DATA_CODE_DT e)
{
	return (SLMSG_DATA_CODE_DT_codes[e]);
}



/*! Messages: data types */
static const char *SLMSG_DATA_TYPE_names[SLMSG_DATA_TYPE_LIMIT+1] = {
	"simple string data",                        		/*!< SLMSG_DATA_TYPE_STRING    */
	"printf data",                               		/*!< SLMSG_DATA_TYPE_PRINTF    */
	"command",                                   		/*!< SLMSG_DATA_TYPE_CMD       */
};

/*! Messages: data types lookup function
 *
 * \param e	Messages: data types
 * \return	Pointer to const string with Messages: data types name
 */
const char *
SLMSG_DATA_TYPE_lookup (SLMSG_DATA_TYPE e)
{
	return (SLMSG_DATA_TYPE_names[e]);
}



/*! Messages: commands */
static const char *SLMSG_CMD_names[SLMSG_CMD_LIMIT+1] = {
	"flush buffers",                             		/*!< SLMSG_CMD_FLUSH    */
};

/*! Messages: commands lookup function
 *
 * \param e	Messages: commands
 * \return	Pointer to const string with Messages: commands name
 */
const char *
SLMSG_CMD_lookup (SLMSG_CMD e)
{
	return (SLMSG_CMD_names[e]);
}



/*! Messages: handler return codes */
static const char *SLMSG_RC_names[SLMSG_RC_LIMIT+1] = {
	"message handled ok",                        		/*!< SLMSG_RC_OK      */
	"failed to handle message",                  		/*!< SLMSG_RC_FAIL    */
	"pass message on to next handler",           		/*!< SLMSG_RC_PASS    */
};

/*! Messages: handler return codes lookup function
 *
 * \param e	Messages: handler return codes
 * \return	Pointer to const string with Messages: handler return codes name
 */
const char *
SLMSG_RC_lookup (SLMSG_RC e)
{
	return (SLMSG_RC_names[e]);
}



/*! Messages: handler return codes: code strings */
static const char *SLMSG_RC_codes[SLMSG_RC_LIMIT+1] = {
	"OK",                                        		/*!< SLMSG_RC_OK      */
	"FAIL",                                      		/*!< SLMSG_RC_FAIL    */
	"PASS",                                      		/*!< SLMSG_RC_PASS    */
};

/*! Messages: handler return codes code lookup function
 *
 * \param e	Messages: handler return codes
 * \return	pointer to const string with Messages: handler return codes code
 */
const char *
SLMSG_RC_lookup_code (SLMSG_RC e)
{
	return (SLMSG_RC_codes[e]);
}



/*! Messages: handler modes */
static const char *SLMSG_MODE_names[SLMSG_MODE_LIMIT+1] = {
	"*none*",                                    		/*!< SLMSG_MODE_NONE         */
	"normal",                                    		/*!< SLMSG_MODE_NORMAL       */
	"exclusive",                                 		/*!< SLMSG_MODE_EXCLUSIVE    */
};

/*! Messages: handler modes lookup function
 *
 * \param e	Messages: handler modes
 * \return	Pointer to const string with Messages: handler modes name
 */
const char *
SLMSG_MODE_lookup (SLMSG_MODE e)
{
	return (SLMSG_MODE_names[e]);
}



/*! Messages: handler attributes (ID) */
static const char *SLMSG_ATTR_ID_names[SLMSG_ATTR_ID_LIMIT+1] = {
	"*none*",                                    		/*!< SLMSG_ATTR_ID_NONE    */
	"output to error stream",                    		/*!< SLMSG_ATTR_ID_ERR     */
	"output to file",                            		/*!< SLMSG_ATTR_ID_FILE    */
};

/*! Messages: handler attributes (ID) lookup function
 *
 * \param e	Messages: handler attributes (ID)
 * \return	Pointer to const string with Messages: handler attributes (ID) name
 */
const char *
SLMSG_ATTR_ID_lookup (SLMSG_ATTR_ID e)
{
	return (SLMSG_ATTR_ID_names[e]);
}



