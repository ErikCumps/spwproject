/** \file
 * The SPWaW war cabinet - strategic map - render data - 11x11 XPM data.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "smap_renderdata_xpm.h"

#include "xpm_21x21/hexborder_21x21.xpm"
#include "xpm_21x21/hexmask_NONE_21x21.xpm"
#include "xpm_21x21/hexmask_C_21x21.xpm"
#include "xpm_21x21/hexmask_LE_21x21.xpm"
#include "xpm_21x21/hexmask_RE_21x21.xpm"
#include "xpm_21x21/hexmask_RO_21x21.xpm"
#include "xpm_21x21/hexmask_B_21x21.xpm"
#include "xpm_21x21/hexmask_BR_21x21.xpm"
#include "xpm_21x21/hexsel_21x21.xpm"
#include "xpm_21x21/hexselmask_21x21.xpm"
#include "xpm_21x21/water_21x21.xpm"
#include "xpm_21x21/new_bridge_EE_21x21.xpm"
#include "xpm_21x21/new_bridge_SE_21x21.xpm"
#include "xpm_21x21/new_bridge_SW_21x21.xpm"
#include "xpm_21x21/new_bridge_WW_21x21.xpm"
#include "xpm_21x21/new_bridge_NW_21x21.xpm"
#include "xpm_21x21/new_bridge_NE_21x21.xpm"
#include "xpm_21x21/new_road1_EE_21x21.xpm"
#include "xpm_21x21/new_road1_SE_21x21.xpm"
#include "xpm_21x21/new_road1_SW_21x21.xpm"
#include "xpm_21x21/new_road1_WW_21x21.xpm"
#include "xpm_21x21/new_road1_NW_21x21.xpm"
#include "xpm_21x21/new_road1_NE_21x21.xpm"
#include "xpm_21x21/road2_EE_21x21.xpm"
#include "xpm_21x21/road2_SE_21x21.xpm"
#include "xpm_21x21/road2_SW_21x21.xpm"
#include "xpm_21x21/road2_WW_21x21.xpm"
#include "xpm_21x21/road2_NW_21x21.xpm"
#include "xpm_21x21/road2_NE_21x21.xpm"
#include "xpm_21x21/new_railr_EE_21x21.xpm"
#include "xpm_21x21/new_railr_SE_21x21.xpm"
#include "xpm_21x21/new_railr_SW_21x21.xpm"
#include "xpm_21x21/new_railr_WW_21x21.xpm"
#include "xpm_21x21/new_railr_NW_21x21.xpm"
#include "xpm_21x21/new_railr_NE_21x21.xpm"
#include "xpm_21x21/traml_EE_21x21.xpm"
#include "xpm_21x21/traml_SE_21x21.xpm"
#include "xpm_21x21/traml_SW_21x21.xpm"
#include "xpm_21x21/traml_WW_21x21.xpm"
#include "xpm_21x21/traml_NW_21x21.xpm"
#include "xpm_21x21/traml_NE_21x21.xpm"
#include "xpm_21x21/blue_21x21.xpm"
#include "xpm_21x21/red_21x21.xpm"
#include "xpm_21x21/neutral_21x21.xpm"
#include "xpm_21x21/contested_21x21.xpm"
#include "xpm_21x21/mask_033_21x21.xpm"
#include "xpm_21x21/bluedot_01_21x21.xpm"
#include "xpm_21x21/bluedot_02_21x21.xpm"
#include "xpm_21x21/bluedot_03_21x21.xpm"
#include "xpm_21x21/bluedot_04_21x21.xpm"
#include "xpm_21x21/bluedot_05_21x21.xpm"
#include "xpm_21x21/bluedot_06_21x21.xpm"
#include "xpm_21x21/bluedot_07_21x21.xpm"
#include "xpm_21x21/bluedot_08_21x21.xpm"
#include "xpm_21x21/bluedot_09_21x21.xpm"
#include "xpm_21x21/bluedot_10_21x21.xpm"
#include "xpm_21x21/bluedot_11_21x21.xpm"
#include "xpm_21x21/bluedot_12_21x21.xpm"
#include "xpm_21x21/bluedot_13_21x21.xpm"
#include "xpm_21x21/bluedot_14_21x21.xpm"
#include "xpm_21x21/bluedot_15_21x21.xpm"
#include "xpm_21x21/bluedot_16_21x21.xpm"
#include "xpm_21x21/bluesplat_21x21.xpm"
#include "xpm_21x21/reddot_01_21x21.xpm"
#include "xpm_21x21/reddot_02_21x21.xpm"
#include "xpm_21x21/reddot_03_21x21.xpm"
#include "xpm_21x21/reddot_04_21x21.xpm"
#include "xpm_21x21/reddot_05_21x21.xpm"
#include "xpm_21x21/reddot_06_21x21.xpm"
#include "xpm_21x21/reddot_07_21x21.xpm"
#include "xpm_21x21/reddot_08_21x21.xpm"
#include "xpm_21x21/reddot_09_21x21.xpm"
#include "xpm_21x21/reddot_10_21x21.xpm"
#include "xpm_21x21/reddot_11_21x21.xpm"
#include "xpm_21x21/reddot_12_21x21.xpm"
#include "xpm_21x21/reddot_13_21x21.xpm"
#include "xpm_21x21/reddot_14_21x21.xpm"
#include "xpm_21x21/reddot_15_21x21.xpm"
#include "xpm_21x21/reddot_16_21x21.xpm"
#include "xpm_21x21/redsplat_21x21.xpm"
#include "xpm_21x21/vhex_blue_21x21.xpm"
#include "xpm_21x21/vhex_red_21x21.xpm"
#include "xpm_21x21/vhex_neutral_21x21.xpm"
#include "xpm_21x21/vhex_contested_21x21.xpm"
#include "xpm_21x21/frontline_EE_21x21.xpm"
#include "xpm_21x21/frontline_SE_21x21.xpm"
#include "xpm_21x21/frontline_SW_21x21.xpm"
#include "xpm_21x21/frontline_WW_21x21.xpm"
#include "xpm_21x21/frontline_NW_21x21.xpm"
#include "xpm_21x21/frontline_NE_21x21.xpm"
#include "xpm_21x21/frontline_dotted_EE_21x21.xpm"
#include "xpm_21x21/frontline_dotted_SE_21x21.xpm"
#include "xpm_21x21/frontline_dotted_SW_21x21.xpm"
#include "xpm_21x21/frontline_dotted_WW_21x21.xpm"
#include "xpm_21x21/frontline_dotted_NW_21x21.xpm"
#include "xpm_21x21/frontline_dotted_NE_21x21.xpm"

SMAP_RENDERDATA_XPMS xpms_21x21 = {
	HEXBORDER_21X21,
	HEXMASK_C_21X21,
	HEXMASK_LE_21X21,
	HEXMASK_RE_21X21,
	HEXMASK_RO_21X21,
	HEXMASK_B_21X21,
	HEXMASK_BR_21X21,
	HEXSEL_21X21,
	HEXSELMASK_21X21,
	WATER_21X21,
	BRIDGE_EE_21X21,
	BRIDGE_SE_21X21,
	BRIDGE_SW_21X21,
	BRIDGE_WW_21X21,
	BRIDGE_NW_21X21,
	BRIDGE_NE_21X21,
	ROAD1_EE_21X21,
	ROAD1_SE_21X21,
	ROAD1_SW_21X21,
	ROAD1_WW_21X21,
	ROAD1_NW_21X21,
	ROAD1_NE_21X21,
	ROAD2_EE_21X21,
	ROAD2_SE_21X21,
	ROAD2_SW_21X21,
	ROAD2_WW_21X21,
	ROAD2_NW_21X21,
	ROAD2_NE_21X21,
	RAILR_EE_21X21,
	RAILR_SE_21X21,
	RAILR_SW_21X21,
	RAILR_WW_21X21,
	RAILR_NW_21X21,
	RAILR_NE_21X21,
	TRAML_EE_21X21,
	TRAML_SE_21X21,
	TRAML_SW_21X21,
	TRAML_WW_21X21,
	TRAML_NW_21X21,
	TRAML_NE_21X21,
	BLUE_21X21,
	RED_21X21,
	NEUTRAL_21X21,
	CONTESTED_21X21,
	MASK_033_21X21,
	BLUEDOT_1_21X21,
	BLUEDOT_2_21X21,
	BLUEDOT_3_21X21,
	BLUEDOT_4_21X21,
	BLUEDOT_5_21X21,
	BLUEDOT_6_21X21,
	BLUEDOT_7_21X21,
	BLUEDOT_8_21X21,
	BLUEDOT_9_21X21,
	BLUEDOT_10_21X21,
	BLUEDOT_11_21X21,
	BLUEDOT_12_21X21,
	BLUEDOT_13_21X21,
	BLUEDOT_14_21X21,
	BLUEDOT_15_21X21,
	BLUEDOT_16_21X21,
	BLUESPLAT_21X21,
	REDDOT_1_21X21,
	REDDOT_2_21X21,
	REDDOT_3_21X21,
	REDDOT_4_21X21,
	REDDOT_5_21X21,
	REDDOT_6_21X21,
	REDDOT_7_21X21,
	REDDOT_8_21X21,
	REDDOT_9_21X21,
	REDDOT_10_21X21,
	REDDOT_11_21X21,
	REDDOT_12_21X21,
	REDDOT_13_21X21,
	REDDOT_14_21X21,
	REDDOT_15_21X21,
	REDDOT_16_21X21,
	REDSPLAT_21X21,
	VHEX_BLUE_21X21,
	VHEX_RED_21X21,
	VHEX_NEUTRAL_21X21,
	VHEX_CONTESTED_21X21,
	FRONTLINE_EE_21X21,
	FRONTLINE_SE_21X21,
	FRONTLINE_SW_21X21,
	FRONTLINE_WW_21X21,
	FRONTLINE_NW_21X21,
	FRONTLINE_NE_21X21,
	FRONTLINE_DOTTED_EE_21X21,
	FRONTLINE_DOTTED_SE_21X21,
	FRONTLINE_DOTTED_SW_21X21,
	FRONTLINE_DOTTED_WW_21X21,
	FRONTLINE_DOTTED_NW_21X21,
	FRONTLINE_DOTTED_NE_21X21,
};
