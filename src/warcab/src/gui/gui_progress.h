/** \file
 * The SPWaW war cabinet - GUI - progress dialog box.
 *
 * Copyright (C) 2005-2021 Erik Cumps <erik.cumps@gmail.com>
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
	void	set	(int pos);
	int	get	(void);
	void	done	(void);

private:
	struct s_data {
		bool	active;
		int	min;
		int	max;
		int	v;
	} d;
};

class GuiProgressEngine
{
public:
	GuiProgressEngine	(GuiProgress *gp, int limit);

public:
	void	set_steps	(int steps);
	void	step	(void);

private:
	struct s_data {
		GuiProgress	*gp;
		int		limit;
		int		steps;
		int		start;
		double		step_size;
		int		step;
	} d;
};

#endif	/* GUI_PROGRESS_H */
