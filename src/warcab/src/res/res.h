/** \file
 * The SPWaW war cabinet - resource handling.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	RES_COMMON_H
#define	RES_COMMON_H	1

extern QPixmap	*RES_PIXMAP_NONE;
extern QColor	*RES_COLOR_NONE;

extern SL_ERROR	RES_init	(void);
extern void	RES_shutdown	(void);

extern QPixmap *	RES_pixmap	(unsigned long rid);
extern QPixmap *	RES_pixmap	(unsigned long rid, unsigned long sid);
extern QColor *		RES_color	(unsigned long rid);
extern QString		RES_htmlcolor	(unsigned long rid);

extern QPixmap *	RES_flag	(SPWAW_GAME_TYPE gametype, BYTE oob);
extern QPixmap *	RES_flagbyid	(const char *flagid);
extern QPixmap *	RES_mission	(SPWAW_MISSION mission, bool meeting);

extern QColor *		RES_GUI_color	(SPWAW_RANK id);
extern QColor *		RES_GUI_color	(SPWAW_USTATUS id);
extern QColor *		RES_GUI_color	(SPWAW_ENTR id);
extern QColor *		RES_GUI_color	(SPWAW_EXP id);
extern QColor *		RES_GUI_color	(SPWOOB_FTYPE id);

extern QColor *		RES_PLOT_color	(SPWAW_EXP id);
extern QColor *		RES_PLOT_color	(SPWAW_RANK id);
extern QColor *		RES_PLOT_color	(SPWOOB_UCLASS id);

#endif	/* RES_COMMON_H */
