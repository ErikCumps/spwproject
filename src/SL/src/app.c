/** \file
 * Slay's Library - application information handling.
 *
 * Copyright (C) 2001-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "internal_debug.h"
#include <SL/system.h>
#include <SL/safe_mem.h>
#include <SL/bool.h>
#include <SL/stdbg.h>
#include <SL/app.h>



/* --- forward declarations --- */

static void	info_cleanup	(void);
static void	update_info	(SL_APP_INFO *src);
static void	statereport	(SL_STDBG_INFO_LEVEL level);



/* --- private data types --- */

/*! Internal application info structure */
typedef struct s_slapp_info {
	struct {
		char	*name;				/*!< application name			*/
		char	*desc;				/*!< application description		*/
		char	*vers;				/*!< application version		*/
		char	*copy;				/*!< copyright string			*/
		char	*auth;				/*!< main author name			*/
		char	*mail;				/*!< main author email			*/
	} info;		/*!< general information	*/
	struct {
		char	*build;				/*!< build number			*/
		char	*date;				/*!< build datestamp			*/
		char	*time;				/*!< build timestamp			*/
	} build;	/*!< build information		*/
	struct {
		int	argc;				/*!< number of argument strings		*/
		char	**argv;				/*!< copy of argument strings		*/
		char	*cwd;				/*!< initial cwd			*/
		char	*exe;				/*!< executable invocation name		*/
		char	*exe_name;			/*!< executable filename		*/
		char	*exe_path;			/*!< executable path			*/
		char	*data_path;			/*!< user data storage path		*/
	} context;	/*!< context information	*/
	struct {
		char	*longvers;			/*!< extended version string		*/
		char	*info;				/*!< info string			*/
		char	*longinfo;			/*!< extended info string		*/
	} extra;	/*!< extra information		*/
} slapp_info;



/* --- private macros --- */

#define	MAXSTRLEN	256

#define	FREEPTR(p_)	if ((p_)) SL_SAFE_FREE ((p_))


/* --- private variables --- */

/*! Module name */
static const char	*MODULE = "SL_APP";

/*! Initialization status */
static SL_BOOL		initialized = SL_false;

/*! Internal storage of application info */
static slapp_info	info;



/* --- code --- */

/*! Initializes application information handling subsystem
 *  and sets private application info from callers appinfo
 *
 * \param appinfo	pointer to caller's application information structure
 * \return		Error status
 */
SL_SINIT
SL_APP_init (SL_APP_INFO *appinfo)
{
	IDBGLOG0 ("start");

	if (initialized) {
		IDBGLOG0 ("already initialized");
		return (SL_SINIT_REINIT);
	}

	info_cleanup ();
	update_info (appinfo);

	SL_STDBG_add (statereport, MODULE);

	initialized = SL_true;
	IDBGLOG0 ("done");
	return (SL_SINIT_OK);
}

void
SL_APP_shutdown (void)
{
	IDBGLOG0 ("start");

	if (!initialized) {
		IDBGLOG0 ("not yet initialized");
		return;
	}
	initialized = SL_false;

	SL_STDBG_delete (statereport);

	info_cleanup ();

	IDBGLOG0 ("done");
}

static void
info_cleanup (void)
{
	unsigned int	i;

	FREEPTR (info.info.name);
	FREEPTR (info.info.desc);
	FREEPTR (info.info.vers);
	FREEPTR (info.info.copy);
	FREEPTR (info.info.auth);
	FREEPTR (info.info.mail);
	FREEPTR (info.build.build);
	FREEPTR (info.build.date);
	FREEPTR (info.build.time);
	for (i = 0; i < (unsigned int)info.context.argc; i++) FREEPTR (info.context.argv[i]);
	FREEPTR (info.context.argv);
	FREEPTR (info.context.cwd);
	FREEPTR (info.context.exe);
	FREEPTR (info.context.exe_name);
	FREEPTR (info.context.exe_path);
	FREEPTR (info.context.data_path);
	FREEPTR (info.extra.longvers);
	FREEPTR (info.extra.info);
	FREEPTR (info.extra.longinfo);

	memset (&info, 0, sizeof (info));
}

static void
build_context (SL_APP_INFO *src)
{
	unsigned int	i;

	info.context.argc = src->context.argc;
	if (src->context.argv != NULL) {
		SL_SAFE_CALLOC (info.context.argv, (info.context.argc + 1), sizeof (char *));
		for (i = 0; i < (unsigned int)info.context.argc; i++) {
			SL_SAFE_STRDUP (info.context.argv[i], src->context.argv[i]);
		}
	} else {
		info.context.argv = NULL;
	}
}

static void
build_version (SL_APP_INFO *src)
{
	char	*vpre, *vdebug, *vtrace, *vpost, *vsplit;
	char	buffer[MAXSTRLEN];

#if (DEBUG || DEEPTRACE)
	vpre   = " (";
	vdebug = "";
	vtrace = "";
	vpost  = ")";
	vsplit = "";
#	if DEBUG
	vdebug = "debug";
#	endif	/* DEBUG extra info */
#	if DEEPTRACE
	vtrace = "deeptrace";
#	endif	/* DEEPTRACE EXTRA INFO */
#	if (DEBUG && TRACE)
	vsplit = "/";
#	endif	/* DEBUG and DEEPTRACE both have info */
#else	/* no DEBUG or DEEPTRACE */
	vpre   = "";
	vdebug = "";
	vtrace = "";
	vpost  = "";
	vsplit = "";
#endif	/* DEBUG or DEEPTRACE */

	memset (buffer, 0, sizeof (buffer));
	snprintf (buffer, sizeof (buffer) - 1, "%s%s%s%s%s%s", src->info.vers, vpre, vdebug, vsplit, vtrace, vpost);
	SL_SAFE_STRDUP (info.info.vers, buffer);

	memset (buffer, 0, sizeof (buffer));
	snprintf (buffer, sizeof (buffer) - 1, "%s version %s", info.info.desc, info.info.vers);
	SL_SAFE_STRDUP (info.extra.longvers, buffer);
}

static SL_BOOL
get_cwd (char **dst)
{
	DWORD	len;

	ASSERT (dst != NULL);
	if (*dst) SL_SAFE_FREE (*dst);

	len = GetCurrentDirectory (0, NULL);
	SL_SAFE_MALLOC (*dst, len);
	return (GetCurrentDirectoryA (len, *dst) == (len-1));
}

static SL_BOOL
set_cwd (char *tgt)
{
	ASSERT (tgt != NULL);

	return (SetCurrentDirectoryA (tgt));
}

/*! Discovers executable information
 */
static void
discover_exe (void)
{
	char	*exe = NULL;
	char	*p;
	char	buffer[MAX_PATH+1];

	if (info.context.argv && info.context.argv[0]) {
		SL_SAFE_STRDUP (info.context.exe, info.context.argv[0]);
	}
	/* there is currently no fallback path for missing argv[0] */
	ASSERT (info.context.exe != NULL);

	/* Create relative app directory */
	SL_SAFE_STRDUP (exe, info.context.exe);
	p = strrchr (exe, '\\');
	if (p)	{
		SL_SAFE_STRDUP (info.context.exe_name, p+1);
		*p = '\0';
	} else	{
		SL_SAFE_STRDUP (info.context.exe_name, info.context.exe);
		SL_SAFE_FREE (exe);
		SL_SAFE_STRDUP (exe, ".\\");
	}

	/* Discover current working dir */
	if (!get_cwd (&info.context.cwd))	goto handle_error;

	/* Discover application path */
	if (!set_cwd (exe))			goto handle_error;
	if (!get_cwd (&info.context.exe_path))	goto handle_error;

	/* Reset initial cwd */
	if (!set_cwd (info.context.cwd))	goto handle_error;

	/* Discover user data storage path */
	memset (buffer, 0, sizeof (buffer));
	p = getenv ("APPDATA");
	if (!p) p = info.context.exe_path;
	snprintf (buffer, sizeof (buffer) - 1, "%s\\%s", p, info.info.name);
	SL_SAFE_STRDUP (info.context.data_path, buffer);

handle_error:
	if (exe) SL_SAFE_FREE (exe);
}

/*! Builds application info string
 */
static void
build_info (void)
{
	char	buffer[MAXSTRLEN];

	memset (buffer, 0, sizeof (buffer));
	snprintf (buffer, sizeof (buffer) - 1,
		"This is %s version %s\nCopyright (C) %s by %s <%s>",
			info.info.name, info.info.vers, info.info.copy, info.info.auth, info.info.mail);
	SL_SAFE_STRDUP (info.extra.info, buffer);

	memset (buffer, 0, sizeof (buffer));
	snprintf (buffer, sizeof (buffer) - 1,
		"%s\n%s version %s (build %s)\nCopyright (C) %s by %s <%s>\n\n"
		"Executable file: %s\nBuild timestamp: %s %s",
			info.info.name, info.info.desc, info.info.vers, info.build.build, info.info.copy, info.info.auth, info.info.mail,
			info.context.exe_name, info.build.date, info.build.time);
	SL_SAFE_STRDUP (info.extra.longinfo, buffer);
}

#define	DUPSTR(src_,dst_)	do { if (src_) { SL_SAFE_STRDUP (dst_, src_); } } while (0)

/*! Updates internal application info from callers appinfo
 *
 * \param src	pointer to caller's application information structure
 */
static void
update_info (SL_APP_INFO *src)
{
	if (!src) return;

	DUPSTR (src->info.name, info.info.name);
	DUPSTR (src->info.desc, info.info.desc);
	DUPSTR (src->info.copy, info.info.copy);
	DUPSTR (src->info.auth, info.info.auth);
	DUPSTR (src->info.mail, info.info.mail);
	DUPSTR (src->build.build, info.build.build);
	DUPSTR (src->build.date, info.build.date);
	DUPSTR (src->build.time, info.build.time);

	build_context (src);
	build_version (src);
	discover_exe ();
	build_info ();
}

/*! Returns application name
 *
 * \return	pointer to string with application name
 */
char *
SL_APP_name (void)
{
	return (info.info.name);
}

/*! Returns application description
 *
 * \return	pointer to string with application description
 */
char *
SL_APP_description (void)
{
	return (info.info.desc);
}

/*! Returns application executable name
 *
 * \return	pointer to string with application executable name
 */
char *
SL_APP_exe_name (void)
{
	return (info.context.exe_name);
}

char *
SL_APP_auth (void)
{
	return (info.info.auth);
}

/*! Returns application version
 *
 * \return	pointer to string with version info
 */
char *
SL_APP_version (void)
{
	return (info.info.vers);
}

/*! Returns extended application name and version info string
 *
 * \return	pointer to string with long info
 */
char *
SL_APP_version_ex (void)
{
	return (info.extra.longvers);
}

/*! Returns application info string
 *
 * \return	pointer to string with application info
 */
char *
SL_APP_info (void)
{
	return (info.extra.info);
}

/*! Returns extended application info string
 *
 * \return	pointer to string with extended application info
 */
char *
SL_APP_info_ex (void)
{
	return (info.extra.longinfo);
}

char *
SL_APP_cwd (void)
{
	return (info.context.cwd);
}

char *
SL_APP_exe_path (void)
{
	return (info.context.exe_path);
}

char *
SL_APP_data_path (void)
{
	return (info.context.data_path);
}


/*! Reports application information handler state
 *
 * \param level	state report level
 */
static void
statereport (SL_STDBG_INFO_LEVEL level)
{
	unsigned int	i;

	SAYSTATE0 ("### Application information handler state:\n");

	/* basic information */
	if (level >= SL_STDBG_LEVEL_BAS) {
		SAYSTATE1 ("\tinitialized = %s\n", SL_BOOL_tostr (initialized));
		if (initialized) {
			SAYSTATE0 ("\tgeneral information:\n");
			SAYSTATE1 ("\t\tname              = %s\n", info.info.name);
			SAYSTATE1 ("\t\tdescription       = %s\n", info.info.desc);
			SAYSTATE1 ("\t\tversion           = %s\n", info.info.vers);
			SAYSTATE1 ("\t\tcopyright         = %s\n", info.info.copy);
			SAYSTATE1 ("\t\tmain author name  = %s\n", info.info.auth);
			SAYSTATE1 ("\t\tmain author email = %s\n", info.info.mail);
			SAYSTATE0 ("\tbuild information:\n");
			SAYSTATE1 ("\t\tbuild number      = %s\n", info.build.build);
			SAYSTATE1 ("\t\tbuild date        = %s\n", info.build.date);
			SAYSTATE1 ("\t\tbuild time        = %s\n", info.build.time);
			SAYSTATE0 ("\tcontext information:\n");
			SAYSTATE1 ("\t\targument count    = %d\n", info.context.argc);
			SAYSTATE1 ("\t\tinitial cwd       = \"%s\"\n", info.context.cwd);
			SAYSTATE1 ("\t\tinvocation name   = \"%s\"\n", info.context.exe);
			SAYSTATE1 ("\t\texecutable name   = \"%s\"\n", info.context.exe_name);
			SAYSTATE1 ("\t\texecutable path   = \"%s\"\n", info.context.exe_path);
			SAYSTATE1 ("\t\tuser data path    = \"%s\"\n", info.context.data_path);
		}
	}

	/* extended information */
	if (level >= SL_STDBG_LEVEL_EXT) {
		if (initialized) {
			SAYSTATE0 ("\textra information:\n");
			SAYSTATE1 ("\t\tlong version      = %s\n", info.extra.longvers);
			SAYSTATE1 ("\t\tinfo              = +++++\n%s\n+++++\n", info.extra.info);
			SAYSTATE1 ("\t\tlong info         = +++++\n%s\n+++++\n", info.extra.longinfo);
		}
	}

	/* deep probe */
	if (level >= SL_STDBG_LEVEL_DEEP) {
		if (initialized) {
			SAYSTATE0 ("\targuments:\n");
			for (i = 0; i < (unsigned int)info.context.argc; i++) {
				SAYSTATE2 ("\t\t[%3u] %s\n", i, info.context.argv[i]);
			}
		}
	}

	SAYSTATE0 ("\n");
}
