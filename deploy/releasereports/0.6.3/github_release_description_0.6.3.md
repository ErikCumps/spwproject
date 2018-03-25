Welcome to the SPWaW war cabinet, release 0.6.3.

Copyright (C) 2005-2017 Erik Cumps erik.cumps@gmail.com

License: GPL V2.

# Release information

## IMPORTANT NOTE!

With the improved unit detection fix for issue #4, the list of units detected by
Warcab version 0.6.1 and older can be different from the list of units detected
by this version of Warcab. This means in fact that the dossier files created by
version 0.6.1 and older are no longer compatible with and cannot be loaded
by this release.

## This release fixes the following issues:

* Fixed issue #6:
    Also deploy the required MSVC runtime DLLs.

# Installation

To install: extract the zip file in a folder of your choice. A new warcab folder will be created there.

**The application is standalone and can simply be run from inside this folder.**

## Release files

This release contains the following files:

| filename | description |
| --- | --- |
| warcab_0.6.3.zip | The release build. |
| releasereport_warcab_0.6.3.zip.html | An abbreviated FileAlyzer report that can be used to verify the warcab_0.6.3.zip file integrity. |
| warcab_0.6.3_debug.zip | The debug build, featuring debug logging and memory management tracking. |
| releasereport_warcab_0.6.3_debug.zip.html | An abbreviated FileAlyzer report that can be used to verify the warcab_0.6.3_debug.zip file integrity. |

# Content

This installation provides the following tools:

## warcab.exe

  Warcab is a tool to record and track battles in an SPWaW campaign. It is inspired by Campaign Watcher.

  Warcab tracks information for core units (including reassignments) during a campaign. It also shows and tracks information for core, auxiliary and opponent units during battles and battle turns.

  Its current main missing features are data export and printing.

  A demo warcab dossier has been included in the "save" folder of this release.

## spwreport.exe

  The spwreport tool generates SPWaW savegame reports.

  It can also generate a CSV dump of the SPWaW OOB file content.

## spwdecompose.exe

  The spwdecompose tool extracts the uncompressed section data of a SPWaW savegame, allowing easy inspection and modification.

## spwreconstruct.exe

  The spwreconstruct tool reconstucts a SPWaW savegame from a set of uncompressed data sections (as generated by spwdecompose),