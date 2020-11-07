/** \file
 * The SPWaW war cabinet - savegame file tracking.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "common.h"
#include "tracking.h"
#include "gui/gui.h"
#include "gui/gui_private.h"
#include "mdld_tree.h"
#include "resource.h"

#define MSGTIME_GOOD		1000
#define	MSGTIME_BAD		5000
#define	FILE_SETTLE_TIME	250

/* --- private variables --- */

/*! Module name */
static const char	*MODULE = "WARCABTracking";

WARCABTracking::WARCABTracking (WARCABState *warcab)
{
	/* Initialize */
	memset (&d, 0, sizeof (d));

	d.ready = SL_false;

	if (!warcab) return;
	d.warcab = warcab;

	d.watcher = new QFileSystemWatcher;
	if (!d.watcher) return;
	connect(d.watcher, SIGNAL(fileChanged(const QString&)), this, SLOT(watcher_event(const QString&)));

	d.processed = new QDateTime();
	if (!d.processed) return;

	/* Connect private slots */
	bool connected = true;
	connected &= connect (d.warcab, SIGNAL (was_loaded(MDLD_TREE_ITEM *)), this, SLOT (loaded(MDLD_TREE_ITEM *)));
	connected &= connect (d.warcab, SIGNAL (will_close()), this, SLOT (closed()));
	connected &= connect (d.warcab, SIGNAL (was_added(MDLD_TREE_ITEM *)), this, SLOT (added(MDLD_TREE_ITEM *)));

	if (!connected) return;

	d.ready = SL_true;
}

WARCABTracking::~WARCABTracking ()
{
	if (d.watcher) delete d.watcher;
	if (d.processed) delete d.processed;
}

bool
WARCABTracking::is_ready (void)
{
	return (d.ready);
}

static void
filedate2qdatetime (FILETIME &filedate, QDateTime &qdt)
{
	SYSTEMTIME st;

	FileTimeToSystemTime (&filedate, &st);
	QDate d(st.wYear, st.wMonth, st.wDay);
	QTime t(st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	qdt = QDateTime (d, t);
}

void
WARCABTracking::activate (SPWAW_DOSSIER_TRACKING &tracking)
{
	if (d.active) {
		if (SPWAW_savegame_descriptor_equal (tracking.sgd, d.tracking.sgd)) {
			// already active, no need to activate
			return;
		} else {
			// already active but with different tracking
			deactivate();
		}
	}

	//DBG_log ("activating...\n");

	/* Store copy of tracking data */
	tracking_set (tracking);
	
	/* Create status base string */
	UtilStrbuf str(d.status_base, sizeof (d.status_base), true, true);
	str.printf ("%s savegame - ", SPWAW_savetype2str(d.tracking.sgd.savetype));
	if (d.tracking.sgd.numeric_id) {
		str.printf ("\"index %d\"", d.tracking.sgd.id.number);
	} else {
		str.printf ("\"%s\"", d.tracking.sgd.id.name);
	}

	/* Record current tracking filedate */
	filedate2qdatetime (d.tracking.filedate, *d.processed);

	/* Start watching file */
	d.watcher->addPath(QString(d.tracking.filename));

	/* Set status */
	set_status (TRACKING);

	d.active = SL_true;
	//DBG_log ("activated!\n");
}

void
WARCABTracking::deactivate (void)
{
	if (!d.active) return;

	//DBG_log ("deactivating...\n");
	d.active = SL_false;

	/* Stop watching file */
	d.watcher->removePath (QString(d.tracking.filename));

	/* Reset tracking filedate */
	d.processed->setTime_t(0);

	/* Clear status base string */
	UtilStrbuf str(d.status_base, sizeof (d.status_base), true, true);

	/* Clear tracking data */
	tracking_clear();

	set_status (INACTIVE);

	//DBG_log ("deactivated!\n");
}

void
WARCABTracking::set_status (STATE state)
{
	char		buf[4096];
	UtilStrbuf	str(buf, sizeof (buf), true, true);

	switch (state) {
		case INACTIVE:
			GUI_WIN->get_status()->clear();
			break;
		case TRACKING:
			str.printf ("<b>tracking</b>");
			str.printf (" - %s", d.status_base);
			GUI_WIN->get_status()->status (buf);
			break;
		case ADDING:
			str.printf ("<font color=%s><b>adding</b></font>", qPrintable(RES_htmlcolor (RID_GM_DLT_NTR)));
			str.printf (" - %s", d.status_base);
			GUI_WIN->get_status()->status (buf);
			break;
		case ADDED:
			str.printf ("<font color=%s><b>added</b></font>", qPrintable(RES_htmlcolor (RID_GM_DLT_POS)));
			str.printf (" - %s", d.status_base);
			GUI_WIN->get_status()->message (buf, MSGTIME_GOOD);
			set_status (TRACKING);
			break;
		case FAILED:
			str.printf ("<font color=%s><b>failed to add</b></font>", qPrintable(RES_htmlcolor (RID_GM_DLT_NEG)));
			str.printf (" - %s", d.status_base);
			GUI_WIN->get_status()->message (buf, MSGTIME_BAD);
			set_status (TRACKING);
			break;
		default:
			str.printf ("%s", d.status_base);
			break;
	}
}

void
WARCABTracking::tracking_set (SPWAW_DOSSIER_TRACKING &tracking)
{
	tracking_clear();

	d.tracking.sgd = tracking.sgd;
	SL_SAFE_STRDUP (d.tracking.filename, tracking.filename);
	d.tracking.filedate = tracking.filedate;
}

void
WARCABTracking::tracking_clear (void)
{
	if (d.tracking.filename) SL_SAFE_FREE (d.tracking.filename);
	memset (&(d.tracking), 0, sizeof (d.tracking));
}

void
WARCABTracking::loaded (MDLD_TREE_ITEM *tree)
{
	MDLD_TREE_ITEM	*dossier = NULL;

	//DBG_log ("[%s] tree=0x%8.8x\n", __FUNCTION__, tree);

	if (tree->dossier_type == SPWAW_MEGACAM_DOSSIER) {
		dossier = MDLD_TREE_raise_to (tree, MDLD_TREE_DOSSIER);
	}

	if (dossier && dossier->data.d) {
		activate (dossier->data.d->tracking);
	} else {
		deactivate();
	}

	//statereport (SL_STDBG_LEVEL_DEEP);
}

void
WARCABTracking::closed (void)
{
	//DBG_log ("[%s]\n", __FUNCTION__);

	deactivate();

	//statereport (SL_STDBG_LEVEL_DEEP);
}

void
WARCABTracking::added (MDLD_TREE_ITEM *item)
{
	MDLD_TREE_ITEM	*dossier = NULL;

	//DBG_log ("[%s] item=0x%8.8x\n", __FUNCTION__, item);

	if (item->dossier_type == SPWAW_MEGACAM_DOSSIER) {
		dossier = MDLD_TREE_raise_to (item, MDLD_TREE_DOSSIER);
	}

	if (dossier && dossier->data.d) {
		activate (dossier->data.d->tracking);
	} else {
		deactivate();
	}

	//statereport (SL_STDBG_LEVEL_DEEP);
}

#if	ENABLE_TRACKING_BACKUPS
typedef struct s_GAMEFILE_META {
	const char *base;
	const char *ext_data;
	const char *ext_metadata;
} GAMEFILE_META;

static bool
gamefile_info (SPWAW_GAME_TYPE gametype, SPWAW_SAVE_TYPE savetype, GAMEFILE_META *meta)
{
	if (!meta) return (false);
	meta->base = meta->ext_data = meta->ext_metadata = NULL;

	switch (gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			switch (savetype) {
				case SPWAW_SAVE_TYPE_REGULAR:
					meta->base         = "save";
					meta->ext_data     = "dat";
					meta->ext_metadata = "cmt";
					break;
				case SPWAW_SAVE_TYPE_MEGACAM:
					meta->base         = NULL;
					meta->ext_data     = "sav";
					meta->ext_metadata = "cam";
					break;
				default:
					/* unsupported save type */
					break;
			}
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			switch (savetype) {
				case SPWAW_SAVE_TYPE_REGULAR:
					meta->base         = "SpSv";
					meta->ext_data     = "dat";
					meta->ext_metadata = "cmt";
					break;
				default:
					/* unsupported save type */
					break;
			}
			break;
		case SPWAW_GAME_TYPE_UNKNOWN:
		default:
			return (false);
			break;
	}

	return (true);
}

static void
tracking_backup (SPWAW_DOSSIER_TRACKING &tracking)
{
	static time_t	stamp = time(NULL);
	static int	idx = 0;
	GAMEFILE_META	gfm;
	char		tgtbuf[MAX_PATH+1];
	char		fltbuf[MAX_PATH+1];

	if (!gamefile_info (tracking.sgd.gametype, tracking.sgd.savetype, &gfm)) return;

	memset (tgtbuf, 0, sizeof(tgtbuf));
	snprintf (tgtbuf, sizeof(tgtbuf) - 1, "%lu_tracking_backup_%04u", stamp, idx);

	QDir tgt (CFG_snap_path());
	if (!tgt.mkdir (tgtbuf)) return;
	if (!tgt.cd (tgtbuf)) return;

	//DBG_log ("[%s] tgt=\"%s\"\n", __FUNCTION__, qPrintable(tgt.absolutePath()));

	memset (fltbuf, 0, sizeof (fltbuf));
	if (gfm.base) {
		snprintf (fltbuf, sizeof (fltbuf) - 1, "%s%03u.*", gfm.base, tracking.sgd.id.number);
	} else {
		snprintf (fltbuf, sizeof (fltbuf) - 1, "%s.*", tracking.sgd.id.name);
	}

	QDir src (tracking.sgd.path, fltbuf);
	//DBG_log ("[%s] src=\"%s\"\n", __FUNCTION__, qPrintable(src.absolutePath()));

	QFileInfoList files = src.entryInfoList();
        for (int i=0; i < files.count(); i++) {
		//DBG_log ("[%s] file=\"%s\"\n", __FUNCTION__, qPrintable(files[i].fileName()));

		//DBG_log ("[%s] copying...\n", __FUNCTION__);
		bool b = QFile(src.absolutePath() + "/" + files[i].fileName()).copy(tgt.absolutePath() + "/" + files[i].fileName());
		//DBG_log ("[%s] %s\n", __FUNCTION__, b ? "copied" : "failed to copy file!");
	}

	idx++;
}
#endif	/* ENABLE_TRACKING_BACKUPS */

void
WARCABTracking::watcher_event (const QString& path)
{
	SPWAW_ERROR		arc;
	static volatile int	refcnt = 0;
	QFileInfo		fi;

	//DBG_log ("[%s] ENTER: path=\"%s\"\n", __FUNCTION__, qPrintable(path));

	// Prevent reentrant calls
	if (q_atomic_increment (&refcnt) != 1) goto early_exit;

	if (!d.active) {
		DBG_log ("[%s] tracking not active, ignoring event\n", __FUNCTION__);
		goto early_exit;
	}

	// Allow file activity to settle
	Sleep (FILE_SETTLE_TIME);

	//fi.setCaching (false);
	fi.setFile(path);

	//DBG_log ("[%s] fi.lastModified = %s\n", __FUNCTION__, qPrintable(fi.lastModified().toString("yyyy/MM/dd hh:mm:ss.zzz")));
	//DBG_log ("[%s] d.processed     = %s\n", __FUNCTION__, qPrintable(d.processed->toString("yyyy/MM/dd hh:mm:ss.zzz")));

	if (fi.lastModified() <= *d.processed) {
		DBG_log ("[%s] file already processed, ignoring event\n", __FUNCTION__);
		goto early_exit;
	}

	DBG_log ("[%s] loading updated file\n", __FUNCTION__);
	*d.processed = fi.lastModified();

	set_status (ADDING);

#if	ENABLE_TRACKING_BACKUPS
	tracking_backup (d.tracking);
#endif	/* ENABLE_TRACKING_BACKUPS */

	arc = d.warcab->basic_campaign_add (&d.tracking.sgd);
	//DBG_log ("[%s] d.warcab->add_campaign returned: %s\n", __FUNCTION__, SPWAW_errstr(arc));

	switch (arc) {
		case SPWERR_OK:
		case SPWERR_DUPTURN:
			set_status (ADDED);
			break;
		default:
			set_status (FAILED);
			break;
	}

early_exit:
	q_atomic_decrement (&refcnt);

	//DBG_log ("[%s] d.processed     = %s\n", __FUNCTION__, qPrintable(d.processed->toString("yyyy/MM/dd hh:mm:ss.zzz")));

	//DBG_log ("[%s] EXIT\n", __FUNCTION__);
}

static void
filedate2str (FILETIME &filedate, char *buf, int len)
{
	SYSTEMTIME st;

	if (!buf || !len) return;
	memset (buf, 0, len);

	FileTimeToSystemTime (&filedate, &st);
	snprintf (buf, len - 1, "%4d/%02d/%02d %02d:%02d:%02d.%03d",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
}

void
WARCABTracking::statereport (SL_STDBG_INFO_LEVEL level)
{
	char	buf[256];

	SAYSTATE2 ("### %s, instance 0x%8.8x, state:\n", MODULE, this);

	/* basic information */
	if (level >= SL_STDBG_LEVEL_BAS) {
		SAYSTATE1 ("\tready    = %s\n", SL_BOOL_tostr (d.ready));
		if (d.watcher) {
			QStringList files = d.watcher->files();
			if (files.size()) {
				SAYSTATE0 ("\twatching =\n");
				for (int i=0; i<files.size(); i++) {
					SAYSTATE2 ("\t\t[%d] \"%s\"\n", i, qPrintable(files.at(i)));
				}
			}
		}
		SAYSTATE1 ("\tactive   = %s\n", SL_BOOL_tostr (d.active));
		if (d.active) {
			SAYSTATE1 ("\tfilename = \"%s\"\n", d.tracking.filename);
		}
	}

	/* extended information */
	if (level >= SL_STDBG_LEVEL_EXT) {
		SAYSTATE1 ("\twarcab   = 0x%8.8x\n", d.warcab);
		SAYSTATE1 ("\twatcher  = 0x%8.8x\n", d.watcher);
		if (d.active) {
			SAYSTATE1 ("\tsgd: gametype   = %s\n", SPWAW_gametype2str (d.tracking.sgd.gametype));
			SAYSTATE1 ("\tsgd: savetype   = %s\n", SPWAW_savetype2str (d.tracking.sgd.savetype));
			SAYSTATE1 ("\tsgd: path       = \"%s\"\n", d.tracking.sgd.path);
			SAYSTATE1 ("\tsgd: numeric_id = %s\n", d.tracking.sgd.numeric_id ? "true" : "false");
			if (d.tracking.sgd.numeric_id) {
				SAYSTATE1 ("\tsgd: number     = %u\n", d.tracking.sgd.id.number);
			} else {
				SAYSTATE1 ("\tsgd: name       = \"%s\"\n", d.tracking.sgd.id.name);
			}
			filedate2str (d.tracking.filedate, buf, sizeof(buf));
			SAYSTATE1 ("\tfiledate        = %s\n", buf);
			SAYSTATE1 ("\tstate base      = \"%s\"\n", d.status_base);
		}
	}

	/* deep probe */
	if (level >= SL_STDBG_LEVEL_DEEP) {
	}

	SAYSTATE0 ("\n");
}
