# A simple tool to convert an image file to XPM.
#
# Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
#
# License: GPL v2
 
TARGET	= mkxpm
TEMPLATE= app
LANGUAGE= C++

CONFIG	+= qt console debug_and_release rtti_off

VPATH		= src
HEADERS		+= stdafx.h main.h

SOURCES		+= main.cpp
		   
INCLUDEPATH	+= src

UI_DIR			= ./ui
MOC_DIR			= ./moc
PRECOMPILED_HEADER	= stdafx.h

!debug_and_release|build_pass {
	CONFIG(debug, debug|release) {
		DEFINES	+= DEBUG _DEBUG
		CONFIG	+= sl_debug warn_on
	}
	CONFIG(release, debug|release) {
		CONFIG	+= sl_release
	}
}
