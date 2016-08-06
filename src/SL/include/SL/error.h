/** \file
 * Slay's Library - error handling.
 *
 * Copyright (C) 2001-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef SL_ERROR_H
#define SL_ERROR_H	1

#include <stdarg.h>
#include <SL/init_ids.h>
#include <SL/codeloc.h>

#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */



/*! Error code datatype */
typedef int SL_ERROR;

/*! System error codes */
typedef enum e_SL_ERROR_SYSTEM {
	SL_ERR_NO_ERROR = 0,		/*!< No Error							*/
	SL_ERR_SHOULD_NOT_SEE_THIS,	/*!< you should NEVER see this error!				*/
	SL_ERR_BAD_USERSTART,		/*!< System error codes go beyond start of user error codes	*/
	SL_ERR_BAD_EC,			/*!< Illegal error code						*/
	SL_ERR_BAD_ECNOMEM,		/*!< Illegal error code and ran out of memory			*/
	SL_ERR_NO_MEM,			/*!< Out of memory						*/
	SL_ERR_PANIC,			/*!< System panic						*/
	SL_ERR_NULLARG,			/*!< Function got unexpected 'NULL' argument			*/
	SL_ERR_INVALARG,		/*!< Function got invalid argument				*/
	SL_ERR_SYSCALL_FAIL,		/*!< System call failed						*/
	SL_ERR_LIBCALL_FAIL,		/*!< C library call failed					*/
	SL_ERR_EC_DBLREG,		/*!< Rereg of user error code attempted				*/
	SL_ERR_INVALID_FATAL,		/*!< Invalid fatality type					*/
	SL_ERR_SIGS_NOT_INIT,		/*!< Signal repository not yet initialized			*/
	SL_ERR_SIGS_HANDLER_NOT_FOUND,	/*!< Signal handler not found					*/
	SL_ERR_BAD_SLID,		/*!< StringList index out of bounds				*/
        SL_ERR_REX_NOTMATCHED,		/*!< Regular expression not yet matched				*/
	SL_ERR_REX_BAD_SMID,		/*!< Invalid submatch id for this regular expression		*/
	SL_ERR_REX_SMTRUNC,		/*!< Truncated submatch						*/
	SL_ERR_REX_ERROR,		/*!< Regular expression error					*/
	SL_ERR_REX_NOMATCH,		/*!< No match found for this regular expression			*/
	SL_ERR_FILE_NOTTHERE,		/*!< File does not exist					*/
	SL_ERR_FILE_EXISTS,		/*!< File exists						*/
	SL_ERR_CONFIG_NOT_FOUND,	/*!< Configuration file not found				*/
	SL_ERR_CONFIG_OPEN_FAILED,	/*!< Failed to open configuration file				*/
	SL_ERR_CONFIG_SECTION_NOT_FOUND,/*!< Failed to find configuration section			*/
	SL_ERR_CONFIG_NAME_NOT_FOUND,	/*!< Failed to find configuration value name			*/
	SL_ERR_CONFIG_BAD_SECTION,	/*!< Invalid syntax for section name				*/
	SL_ERR_CONFIG_BAD_NAME,		/*!< Invalid syntax for entry name				*/
	SL_ERR_CONFIG_NO_VALUE,		/*!< Entry without a value					*/
	SL_ERR_CONFIG_BAD_VALUE,	/*!< Invalid syntax for entry value				*/
	SL_ERR_CONFIG_VALUE_BAD_NUMBER,	/*!< Value is an invalid number					*/
	SL_ERR_UI_BADTYPE,		/*!< Invalid user interface type				*/
	SL_ERR_UI_DOUBLEREG,		/*!< User interface type already registered			*/
	SL_ERR_UI_NOTREG,		/*!< User interface type not registered				*/
	SL_ERR_UIFIELD_DOUBLEREG,	/*!< User interface field already registered			*/
	SL_ERR_LOG_NOT_INIT,		/*!< Logging not yet initialized				*/
	SL_ERR_LOG_INITED,		/*!< Logging already initialized				*/
	SL_ERR_LOG_NOFILE,		/*!< No log file specified					*/
	SL_ERR_ERROR,			/*!< Generic error condition					*/
	SL_ERR_FAILED,			/*!< Generic action failure					*/
	SL_ERR_LAST_ERR_CODE = 		/*!< \internal Last error code					*/
	        SL_ERR_FAILED
} SL_ERROR_SYSTEM;

/*! Start index of user error messages */
#define SL_ERR_USER_START	100

/*! Error fatality types (unsigned enum) */
typedef enum e_SL_ERROR_FATAL {
	SL_ERR_FATAL_UNSET = 0,		/*!< \internal indicates unset fatality code			*/
	SL_ERR_FATAL_IGNORE,		/*!< Ignore fatality						*/
	SL_ERR_FATAL_WARN,		/*!< Warning							*/
	SL_ERR_FATAL_SOFTERR,		/*!< Soft Error -> can be retried				*/
	SL_ERR_FATAL_ERR,		/*!< Error -> exits program					*/
	SL_ERR_FATAL_PANIC,		/*!< Panic -> abort() program					*/
	SL_ERR_FATAL_USAGE,		/*!< Usage error						*/
	SL_ERR_FATAL_LAST_FATAL =	/*!< \internal Last fatality type				*/
		SL_ERR_FATAL_USAGE
} SL_ERROR_FATAL;

/*! Error resolution user request codes */
typedef enum e_SL_ERROR_REQUEST {
	SL_ERR_REQUEST_ACCEPT,		/*!< User requested to accept error				*/
	SL_ERR_REQUEST_IGNORE,		/*!< User requested to ignore error				*/
	SL_ERR_REQUEST_RETRY,		/*!< User requested to retry operation				*/
} SL_ERROR_REQUEST;

/*! Error stack locations */
typedef enum e_SL_ERROR_STACKLOC {
	SL_ERR_STACK_UNSET = 0,		/*!< \internal indicates unset stack location			*/
	SL_ERR_STACK_BOTTOM,		/*!< Bottom of stack						*/
	SL_ERR_STACK_MIDDLE,		/*!< Middle of stack						*/
	SL_ERR_STACK_TOP,		/*!< Top of stack						*/
	SL_ERR_STACK_SINGLE		/*!< Top == Bottom of stack					*/
} SL_ERROR_STACKLOC;

/*! Maximum error string size */
#define SL_ERROR_STRSIZE	255

/*! Error information structure
 *
 * \note This is a readonly structure managed by SL_error!
 */
typedef struct s_SL_ERROR_DATA {
	/* Info on error condition */
	char			msg[SL_ERROR_STRSIZE+1];		/*!< Optional error message			*/
	SL_ERROR_FATAL		fatal;					/*!< Optional fatality type			*/
	char			fatality[SL_ERROR_STRSIZE+1];		/*!< Optional fatality type description		*/
	SL_CODELOC		reporter;				/*!< Optional error reporting location		*/

	/* Info on error location */
	SL_CODELOC		location;				/*!< Error detection location			*/

	/* Info on function error */
	SL_ERROR		func_err;				/*!< Return code for last function		*/
	char			func_err_msg[SL_ERROR_STRSIZE+1];	/*!< Message associated with last return code	*/

	/* Info on system error */
	int			sys_err;				/*!< Errno for last operation			*/
	char			sys_err_msg[SL_ERROR_STRSIZE+1];	/*!< System error message for last operation	*/

	/* Optional extra info */
	char			extra[SL_ERROR_STRSIZE+1];		/*!< Extra info					*/

	/* Error stack */
	SL_ERROR_STACKLOC	stackloc;				/*!< Stack location for this error		*/
	struct s_SL_ERROR_DATA	*prev;					/*!< Pointer to previous error on error stack	*/
	unsigned long		idx;					/*!< Stack location index			*/
} SL_ERROR_DATA;



/***** Function prototypes *****/

/* Initialize error system */
extern SL_SINIT		SL_ERROR_init			(void);

/* Shut down error system */
extern void		SL_ERROR_shutdown		(void);

/* Handle error fatality types */
extern const char *	SL_ERROR_fatality_lookup	(SL_ERROR_FATAL fatal, unsigned int lc);

/* Handle user error codes */
extern SL_ERROR		SL_ERROR_code_register		(int ec, const char *msg);
extern const char *	SL_ERROR_code_lookup		(int ec);

/* Reset internal error state to SL_ERR_NO_ERROR */
extern void		SL_ERROR_none			(void);

/* Store and restore error state */
extern void		SL_ERROR_store			(SL_ERROR_DATA **handle);
extern void		SL_ERROR_restore		(SL_ERROR_DATA **handle);

/* Signal function error */
extern void		SL_ERROR_function		(SL_ERROR rc, const char *file, unsigned long line, const char *func, char *extra);
extern void		SL_ERROR_function_alt		(SL_ERROR rc, SL_CODELOC *where, char *fmt, ...);
extern void		SL_ERROR_function_alt2		(SL_ERROR rc, SL_CODELOC *where, char *fmt, va_list AP);

/* Signal system error */
extern void		SL_ERROR_system			(SL_ERROR rc, const char *file, unsigned long line, const char *func, char *extra);
extern void		SL_ERROR_system_alt		(SL_ERROR rc, SL_CODELOC *where, char *fmt, ...);

/* Handle errors */
extern SL_ERROR_REQUEST	SL_ERROR_handle			(SL_ERROR_FATAL fatal, char *msg, const char *file, unsigned long line, const char *func);
extern SL_ERROR_REQUEST	SL_ERROR_handle_alt		(SL_ERROR_FATAL fatal, char *msg, SL_CODELOC *where);

/* Developer panic */
extern void		SL_ERROR_devpanic		(const char *file, unsigned long line, const char *func, char *fmt, ...);


/***** Macro definitions *****/

/*! Sets SL_ERR_NO_ERROR */
#define	SET_ERR_OK		SL_ERROR_none ()

/*! Test if error set */
#define	SL_HAS_ERROR(_rc)	(_rc != SL_ERR_NO_ERROR)

/*! Returns with no error */
#define RETURN_OK												\
	do {													\
		SL_ERROR_none ();										\
		return (SL_ERR_NO_ERROR);									\
	 } while (0)

/*! Sets a function error
 * \param _rc      Error code
 * \param _extra   Optional extra error information
 */
#define	SET_ERR_FUNCTION(_rc, _extra)										\
	do { SL_ERROR_function (_rc, __FILE__, __LINE__, __FUNCTION__, (char *)_extra); } while (0)

/*! Returns with a function error
 * \param _rc      Error code
 * \param _extra   Optional extra error information
 */
#define RETURN_ERR_FUNCTION(_rc, _extra)									\
	do {													\
		SL_ERROR_function (_rc, __FILE__, __LINE__, __FUNCTION__, (char *)_extra);			\
		return (_rc);											\
	} while (0)

/*! Sets a function error with varargs extra info
 * \param _rc     Error code
 * \param _fmt    Format string for optional extra error information
 *
 * \note Parameters _a1 .. _a4 are optional extra error information arguments #1 .. #4
 */
#define	SET_ERR_FUNCTION_EX0(_rc, _fmt)									\
	do { SL_ERROR_function_alt (_rc, SL_HERE_I_AM, (char *)_fmt); } while (0)
#define	SET_ERR_FUNCTION_EX1(_rc, _fmt, _a1)								\
	do { SL_ERROR_function_alt (_rc, SL_HERE_I_AM, (char *)_fmt, _a1); } while (0)
#define	SET_ERR_FUNCTION_EX2(_rc, _fmt, _a1, _a2)							\
	do { SL_ERROR_function_alt (_rc, SL_HERE_I_AM, (char *)_fmt, _a1, _a2); } while (0)
#define	SET_ERR_FUNCTION_EX3(_rc, _fmt, _a1, _a2, _a3)							\
	do { SL_ERROR_function_alt (_rc, SL_HERE_I_AM, (char *)_fmt, _a1, _a2, _a3); } while (0)
#define	SET_ERR_FUNCTION_EX4(_rc, _fmt, _a1, _a2, _a3, _a4)						\
	do { SL_ERROR_function_alt (_rc, SL_HERE_I_AM, (char *)_fmt, _a1, _a2, _a3, _a4); } while (0)

/*! Returns with a function error and varargs extra info
 * \param _rc     Error code
 * \param _fmt    Format string for optional extra error information
 *
 * \note Parameters _a1 .. _a4 are optional extra error information arguments #1 .. #4
 */
#define RETURN_ERR_FUNCTION_EX0(_rc, _fmt)									\
	do {													\
		SL_ERROR_function_alt (_rc, SL_HERE_I_AM, (char *)_fmt);					\
		return (_rc);											\
	} while (0)
#define RETURN_ERR_FUNCTION_EX1(_rc, _fmt, _a1)									\
	do {													\
		SL_ERROR_function_alt (_rc, SL_HERE_I_AM, (char *)_fmt, _a1);					\
		return (_rc);											\
	} while (0)
#define RETURN_ERR_FUNCTION_EX2(_rc, _fmt, _a1, _a2)								\
	do {													\
		SL_ERROR_function_alt (_rc, SL_HERE_I_AM, (char *)_fmt, _a1, _a2);				\
		return (_rc);											\
	} while (0)
#define RETURN_ERR_FUNCTION_EX3(_rc, _fmt, _a1, _a2, _a3)							\
	do {													\
		SL_ERROR_function_alt (_rc, SL_HERE_I_AM, (char *)_fmt, _a1, _a2, _a3);				\
		return (_rc);											\
	} while (0)
#define RETURN_ERR_FUNCTION_EX4(_rc, _fmt, _a1, _a2, _a3, _a4)							\
	do {													\
		SL_ERROR_function_alt (_rc, SL_HERE_I_AM, (char *)_fmt, _a1, _a2, _a3, _a4);			\
		return (_rc);											\
	} while (0)

/*! Sets a system error
 * \param _rc      Error code
 * \param _extra   Optional extra error information
 */
#define	SET_ERR_SYSTEM(_rc, _extra)										\
	do { SL_ERROR_system (_rc, __FILE__, __LINE__, __FUNCTION__, (char *)_extra); } while (0)

/*! Returns with a system error
 * \param _rc      Error code
 * \param _extra   Optional extra error information
 */
#define RETURN_ERR_SYSTEM(_rc, _extra)										\
	do {													\
		SL_ERROR_system (_rc, __FILE__, __LINE__, __FUNCTION__, (char *)_extra);			\
		return (_rc);											\
	} while (0)

/*! Sets a system error with varargs extra info
 * \param _rc     Error code
 * \param _fmt    Format string for optional extra error information
 *
 * \note Parameters _a1 .. _a4 are optional extra error information arguments #1 .. #4
 */
#define	SET_ERR_SYSTEM_EX0(_rc, _fmt)										\
	do { SL_ERROR_system_alt (_rc, SL_HERE_I_AM, (char *)_fmt); } while (0)
#define	SET_ERR_SYSTEM_EX1(_rc, _fmt, _a1)									\
	do { SL_ERROR_system_alt (_rc, SL_HERE_I_AM, (char *)_fmt, _a1); } while (0)
#define	SET_ERR_SYSTEM_EX2(_rc, _fmt, _a1, _a2)									\
	do { SL_ERROR_system_alt (_rc, SL_HERE_I_AM, (char *)_fmt, _a1, _a2); } while (0)
#define	SET_ERR_SYSTEM_EX3(_rc, _fmt, _a1, _a2, _a3)								\
	do { SL_ERROR_system_alt (_rc, SL_HERE_I_AM, (char *)_fmt, _a1, _a2, _a3); } while (0)
#define	SET_ERR_SYSTEM_EX4(_rc, _fmt, _a1, _a2, _a3, _a4)							\
	do { SL_ERROR_system_alt (_rc, SL_HERE_I_AM, (char *)_fmt, _a1, _a2, _a3, _a4); } while (0)

/*! Returns with a system error and varargs extra info
 * \param _rc     Error code
 * \param _fmt    Format string for optional extra error information
 *
 * \note Parameters _a1 .. _a4 are optional extra error information arguments #1 .. #4
 */
#define RETURN_ERR_SYSTEM_EX0(_rc, _fmt)									\
	do {													\
		SL_ERROR_system_alt (_rc, SL_HERE_I_AM, (char *)_fmt);						\
		return (_rc);											\
	} while (0);
#define RETURN_ERR_SYSTEM_EX1(_rc, _fmt, _a1)									\
	do {													\
		SL_ERROR_system_alt (_rc, SL_HERE_I_AM, (char *)_fmt, _a1);					\
		return (_rc);											\
	} while (0);
#define RETURN_ERR_SYSTEM_EX2(_rc, _fmt, _a1, _a2)								\
	do {													\
		SL_ERROR_system_alt (_rc, SL_HERE_I_AM, (char *)_fmt, _a1, _a2);				\
		return (_rc);											\
	} while (0);
#define RETURN_ERR_SYSTEM_EX3(_rc, _fmt, _a1, _a2, _a3)								\
	do {													\
		SL_ERROR_system_alt (_rc, SL_HERE_I_AM, (char *)_fmt, _a1, _a2, _a3);				\
		return (_rc);											\
	} while (0);
#define RETURN_ERR_SYSTEM_EX4(_rc, _fmt, _a1, _a2, _a3, _a4)							\
	do {													\
		SL_ERROR_system_alt (_rc, SL_HERE_I_AM, (char *)_fmt, _a1, _a2, _a3, _a4);			\
		return (_rc);											\
	} while (0);

/*! Sets a system error with location and varargs extra info
 * \param _rc      Error code
 * \param _where   Location info
 * \param _fmt     Format string for optional extra error information
 *
 * \note Parameters _a1 .. _a4 are optional extra error information arguments #1 .. #4
 */
#define	SET_ERR_SYSTEM_ALT0(_rc, _where, _fmt)									\
	do {													\
		SL_ERROR_system_alt (_rc, _where, (char *)_fmt);						\
	} while (0)
#define	SET_ERR_SYSTEM_ALT1(_rc, _where, _fmt, _a1)								\
	do {													\
		SL_ERROR_system_alt (_rc, _where, (char *)_fmt, _a1);						\
	} while (0)
#define	SET_ERR_SYSTEM_ALT2(_rc, _where, _fmt, _a1, _a2)							\
	do {													\
		SL_ERROR_system_alt (_rc, _where, (char *)_fmt, _a1, _a2);					\
	} while (0)
#define	SET_ERR_SYSTEM_ALT3(_rc, _where, _fmt, _a1, _a2, _a3)							\
	do {													\
		SL_ERROR_system_alt (_rc, _where, (char *)_fmt, _a1, _a2, _a3);					\
	} while (0)
#define	SET_ERR_SYSTEM_ALT4(_rc, _where, _fmt, _a1, _a2, _a3, _a4)						\
	do {													\
		SL_ERROR_system_alt (_rc, _where, (char *)_fmt, _a1, _a2, _a3, _a4);				\
	} while (0)

/*! Handles errors
 * \param _fatal   Error fatality type
 * \param _msg     Optional error message
 */
#define SL_ERROR_HANDLE(_fatal, _msg)										\
	SL_ERROR_handle (_fatal, (char *)_msg, __FILE__, __LINE__, __FUNCTION__);

/*! Sets and handles a function error
 * \param _fatal	Error fatality type
 * \param _msg		Optional error message
 * \param _rc		Error code
 * \param _extra	Optional extra error information
 * \param _req		Storage for error reslution user request
 */
#define HANDLE_ERR_FUNCTION(_fatal, _msg, _rc, _extra, _req)							\
	do {													\
		SET_ERR_FUNCTION (_rc, _extra);									\
		_req = SL_ERROR_HANDLE (_fatal, _msg);								\
	} while (0)

/*! Sets and handles a function error with varargs extra info
 * \param _fatal	Error fatality type
 * \param _msg		Optional error message
 * \param _rc		Error code
 * \param _fmt		Format string for optional extra error information
 * \param _args		Extra error information arguments
 * \param _req		Storage for error resolution user request
 */
#define HANDLE_ERR_FUNCTION_EX(_fatal, _msg, _rc, _fmt, _args, _req)						\
	do {													\
		SET_ERR_FUNCTION_EX1 (_rc, _fmt, _args);							\
		_req = SL_ERROR_HANDLE (_fatal, _msg);								\
	} while (0)

/*! Sets and handles a system error
 * \param _fatal	Error fatality type
 * \param _msg		Optional error message
 * \param _rc		Error code
 * \param _extra	Optional extra error information
 * \param _req		Storage for error resolution user request
 */
#define HANDLE_ERR_SYSTEM(_fatal, _msg, _rc, _extra, _req)							\
	do {													\
		SET_ERR_SYSTEM (_rc, _extra);									\
		_req = SL_ERROR_HANDLE (_fatal, _msg);								\
	} while (0)

/*! Sets and handles a system error with varargs extra info
 * \param _fatal	Error fatality type
 * \param _msg		Optional error message
 * \param _rc		Error code
 * \param _fmt		Format string for optional extra error information
 * \param _args		Extra error information arguments
 * \param _req		Storage for error reslution user request
 */
#define HANDLE_ERR_SYSTEM_EX(_fatal, _msg, _rc, _fmt, _args, _req)						\
	do {													\
		SET_ERR_SYSTEM_EX1 (_rc, _fmt, _args);								\
		_req = SL_ERROR_HANDLE (_fatal, _msg);								\
	} while (0)

/*! Checks for NULL argument and returns with error if NULL
 * \param _arg   Variable to check
 */
#define SL_CWENULLARG(_arg)											\
	do {													\
		if (_arg == NULL) {										\
			RETURN_ERR_FUNCTION (SL_ERR_NULLARG, "Unexpected NULL value for " #_arg " argument");	\
		}												\
	} while (0)

/*! Returns with error code in case of error
 * \param _rc   Error code
 */
#define SL_ROE(_rc)												\
	do {													\
	        if (_rc != SL_ERR_NO_ERROR) { return (_rc); }							\
	} while (0)

/*! Convenience macro to suppress compiler warnings */
#define	DOES_NOT_RETURN												\
	return (SL_ERR_SHOULD_NOT_SEE_THIS)

/*! Developer panic */
#define	DEV_PANIC0(_str)											\
	SL_ERROR_devpanic (__FILE__, __LINE__, __FUNCTION__, (char *)_str)
#define	DEV_PANIC1(_fmt, _a1)											\
	SL_ERROR_devpanic (__FILE__, __LINE__, __FUNCTION__, (char *)_fmt, _a1)
#define	DEV_PANIC2(_fmt, _a1, _a2)										\
	SL_ERROR_devpanic (__FILE__, __LINE__, __FUNCTION__, (char *)_fmt, _a1, _a2)
#define	DEV_PANIC3(_fmt, _a1, _a2, _a3)										\
	SL_ERROR_devpanic (__FILE__, __LINE__, __FUNCTION__, (char *)_fmt, _a1, _a2, _a3)
#define	DEV_PANIC4(_fmt, _a1, _a2, _a3, _a4)									\
	SL_ERROR_devpanic (__FILE__, __LINE__, __FUNCTION__, (char *)_fmt, _a1, _a2, _a3, _a4)

/*! Developer convenience: NULL argument check with panic */
#define	CAPNULLARG(arg)												\
	if (arg == NULL) {											\
		DEV_PANIC1 ("Unexpected NULL for '%s' argument!", #arg);					\
	}

/*! Developer convenience: assert with panic */
#define	DEVASSERT(exp)												\
	if (!(exp)) {												\
		DEV_PANIC1 ("Failed assertion: '%s'!", #exp);							\
	}


#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif /* SL_ERROR_H */

