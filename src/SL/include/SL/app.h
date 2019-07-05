/** \file
 * Slay's Library - application information handling.
 *
 * Copyright (C) 2001-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef SL_APP_H
#define SL_APP_H	1

#include <SL/init_ids.h>

#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */



/* --- structures and types --- */

/*! Application info structure (callerside) */
typedef struct s_SL_APP_INFO {
	struct s_info {
		char	*name;		/*!< application name			*/
		char	*desc;		/*!< application description		*/
		char	*vers;		/*!< application version		*/
		char	*copy;		/*!< copyright string			*/
		char	*auth;		/*!< main author name			*/
		char	*mail;		/*!< main author email			*/
	} info;		/*!< general information	*/
	struct s_build {
		char	*build;		/*!< build number			*/
		char	*date;		/*!< build datestamp			*/
		char	*time;		/*!< build timestamp			*/
	} build;	/*!< build information		*/
	struct s_context {
		int	argc;		/*!< argument count, from argc		*/
		char	**argv;		/*!< arguments, from argv		*/
	} context;	/*!< context information	*/
} SL_APP_INFO;



/* --- API --- */

extern SL_SINIT	SL_APP_init		(SL_APP_INFO *info);
extern void	SL_APP_shutdown		(void);

extern char*	SL_APP_name		(void);
extern char*	SL_APP_description	(void);
extern char*	SL_APP_version		(void);
extern char*	SL_APP_version_ex	(void);
extern char*	SL_APP_copyright	(void);
extern char*	SL_APP_author		(void);
extern char*	SL_APP_email		(void);
extern char*	SL_APP_build_number	(void);
extern char*	SL_APP_build_date	(void);
extern char*	SL_APP_build_time	(void);
extern char*	SL_APP_cwd		(void);
extern char*	SL_APP_exe_name		(void);
extern char*	SL_APP_exe_path		(void);
extern char*	SL_APP_data_path	(void);
extern char*	SL_APP_info		(void);
extern char*	SL_APP_info_ex		(void);




#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif /* SL_APP_H */
