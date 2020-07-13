/** \file
 * The SPWaW war cabinet - configuration dialog.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	CFG_DLG_H
#define	CFG_DLG_H	1

#include "intel_mode.h"

class CfgGuiGameType
{
public:
	CfgGuiGameType (SPWAW_GAME_TYPE type, const char *name)	: type(type), name(name) {}
	CfgGuiGameType (SPWAW_GAME_TYPE type, QString name)	: type(type), name(name) {}

	SPWAW_GAME_TYPE		type;
	QString			name;
};

class CfgDlgGame
{
public:
	CfgDlgGame(SPWAW_GAME_TYPE type, const char *name, char *oob, char *sve) :
		type(type), name(name), oob(oob), sve(sve) {}

	SPWAW_GAME_TYPE		type;
	QString			name;
	QString			oob;
	QString			sve;
};

class CfgDlgData
{
public:
	CfgDlgData(bool isfirstrun, bool locprf, SPWAW_GAME_TYPE type, char *snp, bool compress, bool autoload, bool fhistory, int imode, int hcftype, bool gecross) :
		isfirstrun(isfirstrun), locprf(locprf), def_game(type), snp(snp), compress(compress), autoload(autoload), fhistory(fhistory), imode(imode), hcftype(hcftype), gecross(gecross)
	{
		types.append(CfgGuiGameType(SPWAW_GAME_TYPE_UNKNOWN, "none"));
	}

	void add (CfgDlgGame *game)
	{
		games.append(game);
		types.append(CfgGuiGameType(game->type, game->name));
	}

	bool			isfirstrun;
	bool			locprf;
	SPWAW_GAME_TYPE		def_game;
	QList<CfgGuiGameType>	types;
	QList<CfgDlgGame*>	games;
	QString			snp;
	bool			compress;
	bool			autoload;
	bool			fhistory;
	int			imode;
	int			hcftype;
	bool			gecross;
};

typedef struct s_CfgDlgGuiGame {
	QGroupBox		*box;
	QGridLayout		*layout;
	QLabel			*oob_label;
	QLineEdit		*oob_edit;
	QPushButton		*oob_browse;
	QLabel			*sve_label;
	QLineEdit		*sve_edit;
	QPushButton		*sve_browse;
} CfgDlgGuiGame;

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
		QLabel			*dgt_label;
		QComboBox		*dgt_select;
		QVector<CfgDlgGuiGame>	*games_gui;

		int			bbw;
		int			bbh;
	} d;

public:
	void	prepare		(void);
	void	update		(void);

public slots:
	int	exec		(void);

private slots:
	void	locprf_edit_clicked	(bool checked);
	void	snp_browse_clicked	(bool checked);
	void	oob_browse_clicked	(bool checked);
	void	sve_browse_clicked	(bool checked);
};

#endif	/* CFG_DLG_H */

