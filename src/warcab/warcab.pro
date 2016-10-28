# The SPWaW war cabinet.
#
# Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
#
# License: GPL V2.

TARGET	= warcab
TEMPLATE= app
LANGUAGE= C++

CONFIG	+= qt thread windows debug_and_release rtti_off
CONFIG	-= flat
CONFIG	+= qwt

UI_DIR			= ./ui
MOC_DIR			= ./moc
PRECOMPILED_HEADER	= stdafx.h

VPATH	= src

HEADERS +=					\
	stdafx.h				\
	build.h					\
	build_options.h				\
	defaults.h				\
	resource/resource.h			\
	common.h				\
	error.h					\
	init.h					\
	warcab.h				\
	dbg/dbg.h				\
	dbg/dbg_private.h			\
	util/util.h				\
	util/util_strbuf.h			\
	util/util_spwdlt.h			\
	util/util_strhash.h			\
	util/util_numhash.h			\
	util/util_seqnum.h			\
	util/util_mdltree_reftrack.h		\
	util/util_valtrack.h			\
	cfg/cfg.h				\
	cfg/cfg_dlg.h				\
	res/res.h				\
	res/res_private.h			\
	res/res_arc.h				\
	res/res_img.h				\
	res/res_imgcol.h			\
	res/res_rgb.h				\
	res/res_rgba.h				\
	res/res_xpm.h				\
	model/model.h				\
	model/model_sanity.h			\
	model/model_sort.h			\
	model/model_filter.h			\
	model/model_savelist.h			\
	model/model_snaplist.h			\
	model/model_dosslist.h			\
	model/model_dossier.h			\
	model/model_oob_data.h			\
	model/model_oob_sort.h			\
	model/model_oob.h			\
	model/model_roster_data.h		\
	model/model_roster_sort.h		\
	model/model_roster.h			\
	model/model_history_data.h		\
	model/model_history.h			\
	model/model_unitlist_data.h		\
	model/model_unitlist.h			\
	model/model_mmas_data.h			\
	model/model_mmas.h			\
	model/model_table.h			\
	plot/plot_qwt.h				\
	plot/plot.h				\
	plot/plot_private.h			\
	plot/plot_background.h			\
	plot/plot_timeline.h			\
	plot/plot_idxline.h			\
	plot/plot_grid.h			\
	plot/plot_legend.h			\
	plot/plot_mark.h			\
	plot/plot_symbol.h			\
	plot/plot_curve.h			\
	plot/plot_bar_item.h			\
	plot/plot_bar.h				\
	plot/plot_mmas.h			\
	plot/plot_table.h			\
	gui/gui_qt.h				\
	gui/gui_state.h				\
	gui/gui.h				\
	gui/gui_errorbox.h			\
	gui/gui_progress.h			\
	gui/gui_private.h			\
	gui/gui_dlg_about.h			\
	gui/gui_dlg_load_savegame.h		\
	gui/gui_dlg_load_snapshot.h		\
	gui/gui_dlg_load_dossier.h		\
	gui/gui_dossier.h			\
	gui/gui_dlg_edit_dossier.h		\
	gui/gui_oob_view.h			\
	gui/gui_oob.h				\
	gui/gui_roster_view.h			\
	gui/gui_roster.h			\
	gui/gui_history_view.h			\
	gui/gui_unitlist_view.h			\
	gui/gui_history.h			\
	gui/gui_mmas_view.h			\
	gui/gui_mmas.h				\
	gui/gui_table_view.h			\
	gui/gui_battle_graph.h			\
	gui/gui_report_kills.h			\
	gui/gui_report_losses.h			\
	gui/gui_report_condition.h		\
	gui/gui_report_progression.h		\
	gui/gui_report_oob.h			\
	gui/gui_report_roster.h			\
	gui/gui_report_history.h		\
	gui/gui_report_mmas.h			\
	gui/gui_reports_dossier_overview.h	\
	gui/gui_reports_dossier_graphs.h	\
	gui/gui_reports_dossier.h		\
	gui/gui_reports_battle_overview.h	\
	gui/gui_reports_battle_force.h		\
	gui/gui_reports_battle_compare.h	\
	gui/gui_reports_battle.h		\
	gui/gui_reports_bturn_overview.h	\
	gui/gui_reports_bturn_force.h		\
	gui/gui_reports_bturn_compare.h		\
	gui/gui_reports_bturn.h			\
	gui/gui_mainreport.h			\
	gui/gui_actions.h			\
	gui/gui_mainmenu.h			\
	gui/gui_toolbar.h			\
	gui/gui_status.h			\
	gui/gui_mainwindow.h

SOURCES +=					\
	build.cpp				\
	resource/resource.c			\
	main.cpp				\
	error.cpp				\
	init.cpp				\
	warcab.cpp				\
	dbg/dbg.cpp				\
	dbg/dbg_console.cpp			\
	util/util.cpp				\
	util/util_strbuf.cpp			\
	util/util_spwdlt.cpp			\
	util/util_strhash.cpp			\
	util/util_numhash.cpp			\
	util/util_seqnum.cpp			\
	util/util_mdltree_reftrack.cpp		\
	cfg/cfg.cpp				\
	cfg/cfg_dlg.cpp				\
	res/res.cpp				\
	res/res_arc.cpp				\
	res/res_img.cpp				\
	res/res_imgcol.cpp			\
	res/res_rgb.cpp				\
	res/res_rgba.cpp			\
	res/res_xpm.cpp				\
	res/res_flags.cpp			\
	res/res_mission.cpp			\
	res/res_gui_color.cpp			\
	res/res_plot_color.cpp			\
	model/model_sanity.cpp			\
	model/model_sort.cpp			\
	model/model_filter.cpp			\
	model/model_savelist.cpp		\
	model/model_snaplist.cpp		\
	model/model_dosslist.cpp		\
	model/model_dossier.cpp			\
	model/model_oob_data.cpp		\
	model/model_oob_sort.cpp		\
	model/model_oob.cpp			\
	model/model_roster_data.cpp		\
	model/model_roster_sort.cpp		\
	model/model_roster.cpp			\
	model/model_history_data.cpp		\
	model/model_history.cpp			\
	model/model_unitlist_data.cpp		\
	model/model_unitlist.cpp		\
	model/model_mmas_data.cpp		\
	model/model_mmas.cpp			\
	model/model_table.cpp			\
	plot/plot_background.cpp		\
	plot/plot_timeline.cpp			\
	plot/plot_idxline.cpp			\
	plot/plot_grid.cpp			\
	plot/plot_legend.cpp			\
	plot/plot_mark.cpp			\
	plot/plot_symbol.cpp			\
	plot/plot_curve.cpp			\
	plot/plot_bar_item.cpp			\
	plot/plot_bar.cpp			\
	plot/plot_mmas.cpp			\
	plot/plot_table.cpp			\
	gui/gui.cpp				\
	gui/gui_errorbox.cpp			\
	gui/gui_progress.cpp			\
	gui/gui_dlg_about.cpp			\
	gui/gui_dlg_load_savegame.cpp		\
	gui/gui_dlg_load_snapshot.cpp		\
	gui/gui_dlg_load_dossier.cpp		\
	gui/gui_dossier.cpp			\
	gui/gui_dlg_edit_dossier.cpp		\
	gui/gui_oob_view.cpp			\
	gui/gui_oob.cpp				\
	gui/gui_roster_view.cpp			\
	gui/gui_roster.cpp			\
	gui/gui_history_view.cpp		\
	gui/gui_unitlist_view.cpp		\
	gui/gui_history.cpp			\
	gui/gui_mmas_view.cpp			\
	gui/gui_mmas.cpp			\
	gui/gui_table_view.cpp			\
	gui/gui_battle_graph.cpp		\
	gui/gui_report_kills.cpp		\
	gui/gui_report_losses.cpp		\
	gui/gui_report_condition.cpp		\
	gui/gui_report_progression.cpp		\
	gui/gui_report_oob.cpp			\
	gui/gui_report_roster.cpp		\
	gui/gui_report_history.cpp		\
	gui/gui_report_mmas.cpp			\
	gui/gui_reports_dossier_overview.cpp	\
	gui/gui_reports_dossier_graphs.cpp	\
	gui/gui_reports_dossier.cpp		\
	gui/gui_reports_battle_overview.cpp	\
	gui/gui_reports_battle_force.cpp	\
	gui/gui_reports_battle_compare.cpp	\
	gui/gui_reports_battle.cpp		\
	gui/gui_reports_bturn_overview.cpp	\
	gui/gui_reports_bturn_force.cpp		\
	gui/gui_reports_bturn_compare.cpp	\
	gui/gui_reports_bturn.cpp		\
	gui/gui_mainreport.cpp			\
	gui/gui_actions.cpp			\
	gui/gui_mainmenu.cpp			\
	gui/gui_toolbar.cpp			\
	gui/gui_status.cpp			\
	gui/gui_mainwindow.cpp

INCLUDEPATH +=					\
	../sys/include				\
	../ad_api/include			\
	../resarchv/include			\
	../SL/include				\
	../spwawlib/include			\
	../debug_memtrace/include		\
	resource				\
	src					\
	.

PREBUILD_COMMAND	= prebuild

!debug_and_release|build_pass {
	CONFIG(debug, debug|release) {
		DEFINES	+= DEBUG _DEBUG DEBUG_MEMTRACE DEBUG_MEMTRACE_CPP
		CONFIG	+= sl_debug warn_on qwt_debug
		LIBS	+=						\
			../ad_api/Debug/ad_api.lib			\
			../resarchv/Debug/resarchv.lib			\
			../SL/Debug/SL.lib				\
			../spwawlib/Debug/spwawlib.lib			\
			../debug_memtrace/Debug/debug_memtrace.lib
	}
	CONFIG(release, debug|release) {
		CONFIG	+= sl_release qwt_release
		LIBS	+=						\
			../ad_api/Release/ad_api.lib			\
			../resarchv/Release/resarchv.lib		\
			../SL/Release/SL.lib				\
			../spwawlib/Release/spwawlib.lib		\
			../debug_memtrace/Release/debug_memtrace.lib
	}
}
