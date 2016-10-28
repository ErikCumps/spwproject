/** \file
 * The SPWaW war cabinet - GUI - progress dialog box.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_PROGRESS_H
#define	GUI_PROGRESS_H	1

class GuiProgress	: public QProgressDialog
{
	Q_OBJECT

public:
	GuiProgress	(const QString &label, const QString &cancel, bool mustshow = true, QWidget *P = 0, Qt::WindowFlags F = 0);

public slots:
	void	setRange	(int min, int max);

public:
	void	inc	(int step = 1);
	void	dec	(int step = 1);
	void	done	(void);

private:
	struct s_data {
		bool	active;
		int	min;
		int	max;
		int	v;
	} d;
};

extern void	GUI_fixme	(char *caller);

#define	GUI_FIXME	do { GUI_fixme (__FUNCTION__); } while (0)

#endif	/* GUI_PROGRESS_H */
