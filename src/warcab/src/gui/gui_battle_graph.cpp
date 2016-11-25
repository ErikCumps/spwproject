/** \file
 * The SPWaW war cabinet - GUI - battle graphs.
 *
 * Copyright (C) 2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "gui_battle_graph.h"

#define	BASE_SIZE	40


class GuiBattleGraphExp : public GuiBattleGraph
{
public:
	GuiBattleGraphExp();
	~GuiBattleGraphExp();
public:
	int	battle_value	(SPWAW_BATTLE *battle, int col);
};
GuiBattleGraphExp::GuiBattleGraphExp()
{
	unsigned int	cnt = SPWAW_ELASTCODE + 1;
	unsigned int	i;

	/* Initialize */
	memset (&d, 0, sizeof (d));
	d.type = GUI_BATTLE_GRAPH_TYPE_EXP;

	memset (&definition, 0, sizeof (definition));

	name = "Experience level";

	setup (PLOT_AREA, true, AXIS_TIME);

	SL_SAFE_CALLOC (d.coldefs, cnt, sizeof(MDLPT_COLDEF));

	d.coldefs[0].name = "Date";
	d.coldefs[0].type = MDLPT_DATA_DATE;
	d.coldefs[0].width = BASE_SIZE * 3;

	for (i=SPWAW_ESTARTCODE; i<SPWAW_ELASTCODE; i++) {
		d.coldefs[i+1].name = (char *)SPWAW_exp2str ((SPWAW_EXP)i);
		d.coldefs[i+1].type = MDLPT_DATA_INT;
		d.coldefs[i+1].width = BASE_SIZE * 2;
		d.coldefs[i+1].plot_color = RES_PLOT_color ((SPWAW_EXP)i);
	}

	definition.col_cnt = cnt;
	definition.col_lst = d.coldefs;
}
GuiBattleGraphExp::~GuiBattleGraphExp()
{
	SL_SAFE_FREE (d.coldefs);
}
int
GuiBattleGraphExp::battle_value (SPWAW_BATTLE *battle, int col)
{
	return (battle->snap->OOBp1.core.stats.ustats.exp_cnt[col]);
}

class GuiBattleGraphRnk : public GuiBattleGraph
{
public:
	GuiBattleGraphRnk();
	~GuiBattleGraphRnk();
public:
	int	battle_value	(SPWAW_BATTLE *battle, int col);
};
GuiBattleGraphRnk::GuiBattleGraphRnk()
{
	unsigned int	cnt = SPWAW_RLASTCODE - 2 + 1;
	unsigned int	i;

	/* Initialize */
	memset (&d, 0, sizeof (d));
	d.type = GUI_BATTLE_GRAPH_TYPE_RNK;

	memset (&definition, 0, sizeof (definition));

	name = "Rank distribution";

	setup (PLOT_BAR, true, AXIS_TIME);

	SL_SAFE_CALLOC (d.coldefs, cnt, sizeof(MDLPT_COLDEF));

	d.coldefs[0].name = "Date";
	d.coldefs[0].type = MDLPT_DATA_DATE;
	d.coldefs[0].width = BASE_SIZE * 3;

	for (i=SPWAW_RSTARTCODE; i<(SPWAW_RLASTCODE-2); i++) {
		d.coldefs[i+1].name = (char *)SPWAW_rank2str ((SPWAW_RANK)(i+1));
		d.coldefs[i+1].type = MDLPT_DATA_INT;
		d.coldefs[i+1].width = BASE_SIZE * 2;
		d.coldefs[i+1].plot_color = RES_PLOT_color ((SPWAW_RANK)(i+1));
	}

	definition.col_cnt = cnt;
	definition.col_lst = d.coldefs;
}
GuiBattleGraphRnk::~GuiBattleGraphRnk()
{
	SL_SAFE_FREE (d.coldefs);
}
int
GuiBattleGraphRnk::battle_value (SPWAW_BATTLE *battle, int col)
{
	return (battle->snap->OOBp1.core.stats.ustats.rnk_cnt[col+1]);
}


class GuiBattleGraphCls : public GuiBattleGraph
{
public:
	GuiBattleGraphCls();
	~GuiBattleGraphCls();
public:
	int	battle_value	(SPWAW_BATTLE *battle, int col);
};
GuiBattleGraphCls::GuiBattleGraphCls()
{
	unsigned int	cnt = SPWOOB_UCLASS_LIMIT + 1;
	unsigned int	i;

	/* Initialize */
	memset (&d, 0, sizeof (d));
	d.type = GUI_BATTLE_GRAPH_TYPE_CLS;

	memset (&definition, 0, sizeof (definition));

	name = "Force composition";

	setup (PLOT_BAR, true, AXIS_INDEX);

	SL_SAFE_CALLOC (d.coldefs, cnt, sizeof(MDLPT_COLDEF));

	d.coldefs[0].name = "Date";
	d.coldefs[0].type = MDLPT_DATA_DATE;
	d.coldefs[0].width = BASE_SIZE * 3;

	for (i=SPWOOB_UCLASS_START; i<SPWOOB_UCLASS_LIMIT; i++) {
		d.coldefs[i+1].name = (char *)SPWAW_oob_uclass ((SPWOOB_UCLASS)i);
		d.coldefs[i+1].type = MDLPT_DATA_INT;
		d.coldefs[i+1].width = BASE_SIZE * 2;
		d.coldefs[i+1].plot_color = RES_PLOT_color ((SPWOOB_UCLASS)i);
	}

	definition.col_cnt = cnt;
	definition.col_lst = d.coldefs;
}
GuiBattleGraphCls::~GuiBattleGraphCls()
{
	SL_SAFE_FREE (d.coldefs);
}
int
GuiBattleGraphCls::battle_value (SPWAW_BATTLE *battle, int col)
{
	return (battle->snap->OOBp1.core.stats.ustats.cls_cnt[col]);
}


class GuiBattleGraphKal : public GuiBattleGraph
{
public:
	GuiBattleGraphKal();
	~GuiBattleGraphKal();
public:
	int	battle_value	(SPWAW_BATTLE *battle, int col);
};
GuiBattleGraphKal::GuiBattleGraphKal()
{
	unsigned int	cnt = 3;

	/* Initialize */
	memset (&d, 0, sizeof (d));
	d.type = GUI_BATTLE_GRAPH_TYPE_KAL;

	memset (&definition, 0, sizeof (definition));

	name = "Kills and Losses";

	setup (PLOT_LINE, false, AXIS_INDEX);

	SL_SAFE_CALLOC (d.coldefs, cnt, sizeof(MDLPT_COLDEF));

	d.coldefs[0].name = "Date";
	d.coldefs[0].type = MDLPT_DATA_DATE;
	d.coldefs[0].width = BASE_SIZE * 3;

	d.coldefs[1].name = "Kills";
	d.coldefs[1].type = MDLPT_DATA_INT;
	d.coldefs[1].width = BASE_SIZE * 2;
	d.coldefs[1].plot_color = RES_color (RID_RGB_PLAYER_FG);

	d.coldefs[2].name = "Losses";
	d.coldefs[2].type = MDLPT_DATA_INT;
	d.coldefs[2].width = BASE_SIZE * 2;
	d.coldefs[2].plot_color = RES_color (RID_RGB_OPPONENT_FG);

	definition.col_cnt = cnt;
	definition.col_lst = d.coldefs;
}
GuiBattleGraphKal::~GuiBattleGraphKal()
{
	SL_SAFE_FREE (d.coldefs);
}
int
GuiBattleGraphKal::battle_value (SPWAW_BATTLE *battle, int col)
{
	if (col == 0)
		return (battle->tlast->snap->OOBp2.battle.attr.gen.losses);
	else
		return (battle->tlast->snap->OOBp1.battle.attr.gen.losses);
}


/* ------------------------------------------------------------------------ */

GuiBattleGraph::~GuiBattleGraph (void)
{
}

void	
GuiBattleGraph::set_plot_type (PLOT_TYPE type)
{
	if (definition.plot_type != type) {
		setup (type, definition.plot_stacked, definition.axis_type);
	}
}

void	
GuiBattleGraph::set_plot_stacked (bool stacked)
{
	if (definition.plot_stacked != stacked) {
		setup (definition.plot_type, stacked, definition.axis_type);
	}
}

void	
GuiBattleGraph::set_axis_type (AXIS_TYPE type)
{
	if (definition.axis_type != type) {
		setup (definition.plot_type, definition.plot_stacked, type);
	}
}

bool	
GuiBattleGraph::changed (void)
{
	bool	b = d.changed;
	d.changed = false;
	return (b);
}

int	
GuiBattleGraph::battle_value (SPWAW_BATTLE * /*battle*/, int /*col*/)
{
	return (0);
}


void
GuiBattleGraph::setup (PLOT_TYPE plot_type, bool plot_stacked, AXIS_TYPE axis_type)
{
	definition.title	= name;
	definition.plot_type	= plot_type;
	definition.plot_stacked	= plot_stacked;
	definition.axis_type	= axis_type;

	d.changed = true;
}



GuiBattleGraph *
GUI_create_battle_graph (GUI_BATTLE_GRAPH_TYPE type)
{
	GuiBattleGraph	*p = NULL;

	switch (type) {
		case GUI_BATTLE_GRAPH_TYPE_EXP:
			p = new GuiBattleGraphExp();
			break;
		case GUI_BATTLE_GRAPH_TYPE_RNK:
			p = new GuiBattleGraphRnk();
			break;
		case GUI_BATTLE_GRAPH_TYPE_CLS:
			p = new GuiBattleGraphCls();
			break;
		case GUI_BATTLE_GRAPH_TYPE_KAL:
			p = new GuiBattleGraphKal();
			break;
		default:
			break;
	}
	return (p);
}
