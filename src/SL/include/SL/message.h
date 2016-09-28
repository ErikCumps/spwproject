/** \file
 * Slay's Library - internal message handling.
 *
 * Copyright (C) 2006-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef SL_MESSAGE_H
#define SL_MESSAGE_H	1

#include <stdarg.h>
#include <time.h>
#include <SL/init_ids.h>
#include <SL/message_ids.h>
#include <SL/bool.h>

#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */



/* --- structures and types --- */

/*! Message payload data structure */
typedef struct s_SLMSG_DATA {
	union u_code {
		SLMSG_DATA_CODE_DT	dt;	/*!< optional DeepTrace data code	*/
	} code;
	const char	*source;		/*!< optional extra source name		*/
	SLMSG_DATA_TYPE	type;			/*!< payload data type			*/
	union u_u {
		SLMSG_CMD	cmd;		/*!< command				*/
		struct s_string {
			char	*str;		/*!< string data: string value		*/
		} string;			/*!< string data			*/
		struct s_printf {
			char	*fmt;		/*!< printf data: format string		*/
			va_list	args;		/*!< printf data: arguments		*/
		} printf;			/*!< printf data			*/
	} u;
} SLMSG_DATA;

/*! Message time data type */
typedef time_t SLMSG_TIME;

/*! Message thread id data type */
typedef DWORD SLMSG_THID;

/*! Message data structure */
typedef struct s_SLMSG {
	SLMSG_TYPE	type;	/*!< message type			*/
	SLMSG_FROM	from;	/*!< message source			*/
	SLMSG_PRIO	prio;	/*!< message priority			*/
	SLMSG_DATA	data;	/*!< message data			*/
	SLMSG_TIME	time;	/*!< message time, not user settable!	*/
	SLMSG_THID	thid;	/*!< thread id, not user settable!	*/
} SLMSG;

/*! Message handler function */
typedef SLMSG_RC (*SLMSG_HDLR_FUNC)(SLMSG *msg);

/*! Message handler */
typedef struct s_SLMSG_HDLR {
	const char	*name;	/*!< handler name	*/
	SLMSG_HDLR_FUNC	func;	/*!< handler function	*/
	SLMSG_MODE	mode;	/*!< handler mode	*/
	SLMSG_ATTR	attr;	/*!< handler attributes	*/
} SLMSG_HDLR;

/*! Message handler query list element */
typedef struct s_SLMSG_HQL {
	SLMSG_HDLR		hdlr;
	struct s_SLMSG_HQL	*next;
} SLMSG_HQL;


/* --- API --- */

extern SL_SINIT	SLMSG_init		(void);
extern void	SLMSG_shutdown		(void);
extern void	SLMSG_hdlr_add		(SLMSG_TYPE type, SLMSG_FROM from, const SLMSG_HDLR *hdlr);
extern void	SLMSG_hdlr_replace	(SLMSG_TYPE type, SLMSG_FROM from, const SLMSG_HDLR *hdlr);
extern void	SLMSG_hdlr_set		(SLMSG_TYPE type, SLMSG_FROM from, const SLMSG_HDLR *hdlr);
extern void	SLMSG_hdlr_delete	(SLMSG_TYPE type, SLMSG_FROM from, const SLMSG_HDLR *hdlr);
extern void	SLMSG_hdlr_query	(SLMSG_TYPE type, SLMSG_FROM from, SLMSG_HQL **list);
extern void	SLMSG_hdlr_query_free	(SLMSG_HQL **list);
extern void	SLMSG_send		(SLMSG *msg);
extern void	SLMSG_statereport	(SL_BOOL extended);



#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif	/* SL_MESSAGE_H */

