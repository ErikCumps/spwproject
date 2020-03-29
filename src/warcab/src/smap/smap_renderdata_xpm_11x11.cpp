/** \file
 * The SPWaW war cabinet - strategic map - render data - 11x11 XPM data.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "smap_renderdata_xpm.h"

#include "xpm_11x11/hexborder_11x11.xpm"
#include "xpm_11x11/hexmask_NONE_11x11.xpm"
#include "xpm_11x11/hexmask_C_11x11.xpm"
#include "xpm_11x11/hexmask_LE_11x11.xpm"
#include "xpm_11x11/hexmask_RE_11x11.xpm"
#include "xpm_11x11/hexmask_RO_11x11.xpm"
#include "xpm_11x11/hexmask_B_11x11.xpm"
#include "xpm_11x11/hexmask_BR_11x11.xpm"
#include "xpm_11x11/hexsel_11x11.xpm"
#include "xpm_11x11/hexselmask_11x11.xpm"
#include "xpm_11x11/hneg_11x11.xpm"
#include "xpm_11x11/h000_11x11.xpm"
#include "xpm_11x11/h005_11x11.xpm"
#include "xpm_11x11/h010_11x11.xpm"
#include "xpm_11x11/h015_11x11.xpm"
#include "xpm_11x11/h020_11x11.xpm"
#include "xpm_11x11/h025_11x11.xpm"
#include "xpm_11x11/h030_11x11.xpm"
#include "xpm_11x11/h035_11x11.xpm"
#include "xpm_11x11/water_11x11.xpm"
#include "xpm_11x11/bridge_EE_11x11.xpm"
#include "xpm_11x11/bridge_SE_11x11.xpm"
#include "xpm_11x11/bridge_SW_11x11.xpm"
#include "xpm_11x11/bridge_WW_11x11.xpm"
#include "xpm_11x11/bridge_NW_11x11.xpm"
#include "xpm_11x11/bridge_NE_11x11.xpm"
#include "xpm_11x11/road1_EE_11x11.xpm"
#include "xpm_11x11/road1_SE_11x11.xpm"
#include "xpm_11x11/road1_SW_11x11.xpm"
#include "xpm_11x11/road1_WW_11x11.xpm"
#include "xpm_11x11/road1_NW_11x11.xpm"
#include "xpm_11x11/road1_NE_11x11.xpm"
#include "xpm_11x11/road2_EE_11x11.xpm"
#include "xpm_11x11/road2_SE_11x11.xpm"
#include "xpm_11x11/road2_SW_11x11.xpm"
#include "xpm_11x11/road2_WW_11x11.xpm"
#include "xpm_11x11/road2_NW_11x11.xpm"
#include "xpm_11x11/road2_NE_11x11.xpm"
#include "xpm_11x11/railr_EE_11x11.xpm"
#include "xpm_11x11/railr_SE_11x11.xpm"
#include "xpm_11x11/railr_SW_11x11.xpm"
#include "xpm_11x11/railr_WW_11x11.xpm"
#include "xpm_11x11/railr_NW_11x11.xpm"
#include "xpm_11x11/railr_NE_11x11.xpm"
#include "xpm_11x11/traml_EE_11x11.xpm"
#include "xpm_11x11/traml_SE_11x11.xpm"
#include "xpm_11x11/traml_SW_11x11.xpm"
#include "xpm_11x11/traml_WW_11x11.xpm"
#include "xpm_11x11/traml_NW_11x11.xpm"
#include "xpm_11x11/traml_NE_11x11.xpm"
#include "xpm_11x11/blue_11x11.xpm"
#include "xpm_11x11/red_11x11.xpm"
#include "xpm_11x11/neutral_11x11.xpm"
#include "xpm_11x11/mask_033_11x11.xpm"
#include "xpm_11x11/bluedot_1_11x11.xpm"
#include "xpm_11x11/bluedot_2_11x11.xpm"
#include "xpm_11x11/bluedot_3_11x11.xpm"
#include "xpm_11x11/reddot_1_11x11.xpm"
#include "xpm_11x11/reddot_2_11x11.xpm"
#include "xpm_11x11/reddot_3_11x11.xpm"
#include "xpm_11x11/vhex_blue_11x11.xpm"
#include "xpm_11x11/vhex_red_11x11.xpm"
#include "xpm_11x11/vhex_neutral_11x11.xpm"
#include "xpm_11x11/frontline_EE_11x11.xpm"
#include "xpm_11x11/frontline_SE_11x11.xpm"
#include "xpm_11x11/frontline_SW_11x11.xpm"
#include "xpm_11x11/frontline_WW_11x11.xpm"
#include "xpm_11x11/frontline_NW_11x11.xpm"
#include "xpm_11x11/frontline_NE_11x11.xpm"

SMAP_RENDERDATA_XPMS xpms_11x11 = {
	HEXBORDER_11X11,
	HEXMASK_C_11X11,
	HEXMASK_LE_11X11,
	HEXMASK_RE_11X11,
	HEXMASK_RO_11X11,
	HEXMASK_B_11X11,
	HEXMASK_BR_11X11,
	HEXSEL_11X11,
	HEXSELMASK_11X11,
	WATER_11X11,
	BRIDGE_EE_11X11,
	BRIDGE_SE_11X11,
	BRIDGE_SW_11X11,
	BRIDGE_WW_11X11,
	BRIDGE_NW_11X11,
	BRIDGE_NE_11X11,
	ROAD1_EE_11X11,
	ROAD1_SE_11X11,
	ROAD1_SW_11X11,
	ROAD1_WW_11X11,
	ROAD1_NW_11X11,
	ROAD1_NE_11X11,
	ROAD2_EE_11X11,
	ROAD2_SE_11X11,
	ROAD2_SW_11X11,
	ROAD2_WW_11X11,
	ROAD2_NW_11X11,
	ROAD2_NE_11X11,
	RAILR_EE_11X11,
	RAILR_SE_11X11,
	RAILR_SW_11X11,
	RAILR_WW_11X11,
	RAILR_NW_11X11,
	RAILR_NE_11X11,
	TRAML_EE_11X11,
	TRAML_SE_11X11,
	TRAML_SW_11X11,
	TRAML_WW_11X11,
	TRAML_NW_11X11,
	TRAML_NE_11X11,
	BLUE_11X11,
	RED_11X11,
	NEUTRAL_11X11,
	MASK_033_11X11,
	BLUEDOT_1_11X11,
	BLUEDOT_2_11X11,
	BLUEDOT_3_11X11,
	BLUEDOT_3_11X11,
	BLUEDOT_3_11X11,
	BLUEDOT_3_11X11,
	BLUEDOT_3_11X11,
	BLUEDOT_3_11X11,
	BLUEDOT_3_11X11,
	BLUEDOT_3_11X11,
	BLUEDOT_3_11X11,
	BLUEDOT_3_11X11,
	BLUEDOT_3_11X11,
	BLUEDOT_3_11X11,
	BLUEDOT_3_11X11,
	BLUEDOT_3_11X11,
	REDDOT_1_11X11,
	REDDOT_2_11X11,
	REDDOT_3_11X11,
	REDDOT_3_11X11,
	REDDOT_3_11X11,
	REDDOT_3_11X11,
	REDDOT_3_11X11,
	REDDOT_3_11X11,
	REDDOT_3_11X11,
	REDDOT_3_11X11,
	REDDOT_3_11X11,
	REDDOT_3_11X11,
	REDDOT_3_11X11,
	REDDOT_3_11X11,
	REDDOT_3_11X11,
	REDDOT_3_11X11,
	VHEX_BLUE_11X11,
	VHEX_RED_11X11,
	VHEX_NEUTRAL_11X11,
	FRONTLINE_EE_11X11,
	FRONTLINE_SE_11X11,
	FRONTLINE_SW_11X11,
	FRONTLINE_WW_11X11,
	FRONTLINE_NW_11X11,
	FRONTLINE_NE_11X11,
};

SMAP_RENDERDATA_HCFXPMS hcfxpms_11x11 = {
	HNEG_11X11,
	H000_11X11,
	H005_11X11,
	H010_11X11,
	H015_11X11,
	H020_11X11,
	H025_11X11,
	H030_11X11,
	H035_11X11,
};