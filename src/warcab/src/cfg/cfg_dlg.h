/** \file
 * The SPWaW war cabinet - configuration dialog.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	CFG_DLG_H
#define	CFG_DLG_H	1

typedef struct s_CfgDlgData {
	char	*oob;
	char	*sve;
	char	*snp;
	bool	compress;
	bool	autoload;
} CfgDlgData;

class CfgDlg	: public QDialog
{
	Q_OBJECT

public:
	CfgDlg (void);
	~CfgDlg (void);

	void	data_set	(CfgDlgData *data);
	void	data_get	(CfgDlgData *data);
	void	data_free	(CfgDlgData *data);

private:
	struct s_data {
		QPixmap			*dlgi;
		QFont			*dlgf;
		QFontMetrics		*dlgfm;
		QDialogButtonBox	*buttons;
		QWidget			*body;
		QGridLayout		*layout;
		QLabel			*oob_label;
		QLineEdit		*oob_edit;
		QPushButton		*oob_browse;
		QLabel			*sve_label;
		QLineEdit		*sve_edit;
		QPushButton		*sve_browse;
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

private slots:
	void	oob_browse_clicked	(bool checked);
	void	sve_browse_clicked	(bool checked);
	void	snp_browse_clicked	(bool checked);
};

#endif	/* CFG_DLG_H */

