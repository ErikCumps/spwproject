/** \file
 * The SPWaW war cabinet - configuration dialog.
 *
 * Copyright (C) 2005-2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	CFG_DLG_H
#define	CFG_DLG_H	1

#include "intel_mode.h"
#include "smap/smap_renderdata.h"

/*! Max number of supported game configurations */
#define	GAMECFG_CNT	4

class CfgDlgDataGame
{
public:
	CfgDlgDataGame	(void);

public:
	bool		active;
	QString		name;
	SPWAW_GAME_TYPE	type;
	QString		path;
};

class CfgDlgData
{
public:
	CfgDlgData	(void);

public:
	bool		isfirstrun;
	bool		locprf;
	QString		snp;
	bool		compress;
	bool		autoload;
	bool		fhistory;
	int		imode;
	int		hcftype;
	bool		gecross;
	int		def_game;
	CfgDlgDataGame	games[GAMECFG_CNT];
};

class CfgDlgGuiGame
{
public:
	typedef enum e_STATUS {
		EMPTY = 0,
		PARTIAL,
		COMPLETE,
		CORRECT
	} STATUS;

public:
	CfgDlgGuiGame	(void);

public:
	void	update_status	(void);

public:
	struct s_data {
		STATUS		status;
		QGroupBox	*box;
		QGridLayout	*layout;
		QLabel		*name_label;
		QLineEdit	*name_edit;
		QColor		name_def_status;
		QColor		name_hlt_status;
		QLabel		*type_label;
		QComboBox	*type_select;
		QColor		type_def_status;
		QColor		type_hlt_status;
		QLabel		*path_label;
		QLineEdit	*path_edit;
		QColor		path_def_status;
		QColor		path_hlt_status;
		QPushButton	*path_browse;
	} d;

private:
	void	unhighlight	(void);
	void	highlight	(bool needs_name, bool needs_type, bool needs_path, bool path_not_found, bool invalid_path);
};

class CfgDlg	: public QDialog
{
	Q_OBJECT

public:
	CfgDlg (CfgDlgData *data);
	~CfgDlg (void);

private:
	struct s_data {
		CfgDlgData		*dlg_data;

		QPixmap			*dlgi;
		QFont			*dlgf;
		QFontMetrics		*dlgfm;
		QDialogButtonBox	*buttons;
		QWidget			*body;
		QGridLayout		*layout;
		QLabel			*locprf_label;
		QCheckBox		*locprf_edit;
		QFrame			*separator1;
		QLabel			*snp_label;
		QLineEdit		*snp_edit;
		QPushButton		*snp_browse;
		QLabel			*compress_label;
		QCheckBox		*compress_edit;
		QLabel			*autoload_label;
		QCheckBox		*autoload_edit;
		QLabel			*fhistory_label;
		QCheckBox		*fhistory_edit;
		QLabel			*imode_label;
		QComboBox		*imode_edit;
		QLabel			*hcftype_label;
		QComboBox		*hcftype_edit;
		QLabel			*gecross_label;
		QCheckBox		*gecross_edit;
		QFrame			*separator2;
		QLabel			*defg_label;
		QComboBox		*defg_select;

		int			bbw;
		int			bbh;
	} d;
	struct s_objects {
		CfgDlgGuiGame		gui_games[GAMECFG_CNT];
	} o;

private:
	void	create_locprf		(int &row, QWidget* &tcw);
	void	create_snp		(int &row, QWidget* &tcw);
	void	create_compress		(int &row, QWidget* &tcw);
	void	create_autoload		(int &row, QWidget* &tcw);
	void	create_fhistory		(int &row, QWidget* &tcw);
	void	create_imode		(int &row, QWidget* &tcw);
	void	create_hcftype		(int &row, QWidget* &tcw);
	void	create_gecross		(int &row, QWidget* &tcw);
	void	create_defg		(int &row, QWidget* &tcw);
	void	create_gui_game		(int &row, int idx, QWidget* &tcw);

private:
	int	current_defg		(void);
	void	select_defg		(int idx);
	void	update_defg_gui		(void);

private:
	void	load_data_game		(CfgDlgDataGame &data, CfgDlgGuiGame &game);
	void	load_data		(void);

private:
	void	save_data_game		(CfgDlgGuiGame &game, CfgDlgDataGame &data);
	void	save_data		(void);

private:
	CfgDlgGuiGame	*signal_sender_gui_game	(void);

public slots:
	int	exec			(void);

private slots:
	void	locprf_edit_clicked	(bool checked);
	void	snp_browse_clicked	(bool checked);
	void	game_name_edited	(const QString &text);
	void	game_type_selected	(int index);
	void	game_path_edited	(const QString &text);
	void	game_path_browse_clicked(bool checked);
};

#endif	/* CFG_DLG_H */

