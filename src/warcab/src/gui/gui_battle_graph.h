/** \file
 * The SPWaW war cabinet - GUI - battle graphs.
 *
 * Copyright (C) 2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	GUI_BATTLE_GRAPH_H
#define	GUI_BATTLE_GRAPH_H	1

#include "gui_private.h"
#include "model/model_plot_table.h"

typedef enum e_GUI_BATTLE_GRAPH_TYPE {
	GUI_BATTLE_GRAPH_TYPE_EXP = 0,
	GUI_BATTLE_GRAPH_TYPE_RNK,
	GUI_BATTLE_GRAPH_TYPE_CLS,
	GUI_BATTLE_GRAPH_TYPE_KAL,
	GUI_BATTLE_GRAPH_TYPE_LIMIT = GUI_BATTLE_GRAPH_TYPE_KAL
} GUI_BATTLE_GRAPH_TYPE;

class GuiBattleGraph
{
public:
	virtual	~GuiBattleGraph();

	char		*name;
	MDLPT_DEF	definition;

	void		set_plot_type		(PLOT_TYPE type);
	void		set_plot_stacked	(bool stacked);
	void		set_axis_type		(AXIS_TYPE type);

	bool		changed			(void);

	virtual int	battle_value		(SPWAW_BATTLE *battle, int col);

protected:
	struct s_data {
		GUI_BATTLE_GRAPH_TYPE	type;
		MDLPT_COLDEF		*coldefs;
		bool			changed;
	} d;

protected:
	void	setup			(PLOT_TYPE plot_type, bool plot_stacked, AXIS_TYPE axis_type);
};

extern GuiBattleGraph *	GUI_create_battle_graph (GUI_BATTLE_GRAPH_TYPE type);

#endif	/* GUI_BATTLE_GRAPH_H */
