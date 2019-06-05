/** \file
 * The SPWaW war cabinet - configuration dialog.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	CFG_DLG_H
#define	CFG_DLG_H	1

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
	CfgDlgData(SPWAW_GAME_TYPE type, char *snp, bool compress, bool autoload) :
		def_game(type), snp(snp), compress(compress), autoload(autoload)
	{
		types.append(CfgGuiGameType(SPWAW_GAME_TYPE_UNKNOWN, "none"));
	}

	void add (CfgDlgGame *game)
	{
		games.append(game);
		types.append(CfgGuiGameType(game->type, game->name));
	}

	SPWAW_GAME_TYPE		def_game;
	QList<CfgGuiGameType>	types;
	QList<CfgDlgGame*>	games;
	QString			snp;
	bool			compress;
	bool			autoload;
};

typedef struct s_CfgDlgGuiGame {
		QGroupBox	*box;
		QGridLayout	*layout;
		QLabel		*oob_label;
		QLineEdit	*oob_edit;
		QPushButton	*oob_browse;
		QLabel		*sve_label;
		QLineEdit	*sve_edit;
		QPushButton	*sve_browse;
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
		QLabel			*dgt_label;
		QComboBox		*dgt_select;
		QVector<CfgDlgGuiGame>	*games_gui;
		QLabel			*snp_label;
		QLineEdit		*snp_edit;
		QPushButton		*snp_browse;
		QLabel			*compress_label;
		QCheckBox		*compress_edit;
		QLabel			*autoload_label;
		QCheckBox		*autoload_edit;

		int			bbw;
		int			bbh;
	} d;

public:
	void	prepare		(void);
	void	update		(void);

public slots:
	int	exec		(void);

private slots:
	void	snp_browse_clicked	(bool checked);
	void	oob_browse_clicked	(bool checked);
	void	sve_browse_clicked	(bool checked);
};

#endif	/* CFG_DLG_H */

