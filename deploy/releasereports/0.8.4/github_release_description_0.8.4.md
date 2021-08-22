Welcome to the SPWaW war cabinet, release 0.8.4.

Copyright (C) 2005-2021 Erik Cumps erik.cumps@gmail.com

License: GPL V2.

# Release information

## IMPORTANT NOTE!

The list of units detected by Warcab version 0.6.1 and older can be different
from the list of units detected by this version of Warcab. This means in fact
that the dossier files created by version 0.6.1 and older are no longer
compatible with and cannot be loaded by this release.

## This releases adds the following new features:

* New feature - winSPMBT support
    War Cabinet now has full support for winSPMBT campaigns and battles:
        + new winSPMBT weapon types and classes
        + new winSPMBT unit types and classes
        + new winSPMBT unit movment classes
        + new winSPMBT EW unit attribute
        + new winSPMBT nations, flags and history

* New feature - Added support for editing the battle location
    It is now possible to edit the location of the battle in the dossier, for
    example to make the dossier more historically accurate.

## This release adds the following improvements:

* Improvement - preferences look and feel
    The preferences dialog box now uses a clearer game config layout and red
    highlighting feedback, to provide better guidance for the correct filling
    out of these configuration items. It is also no longer possible to create a
    new dossier with incomplete or missing game config(s)

* Improvement - sanitized GuiDossier treeview behavior
    The GuiDossier TreeView's behavior on the left side of the application
    window has been sanitized, it is now much less erratic and more in line with
    what is expected of this kind of GUI interface element.

* Improvement - reduced memory usage
    Instead of keeping an individual copy of the battle map for each turn in the
    dossier, War Cabinet now keeps only a single copy of the battle map for each
    battle. (after all, the battle map does not change during the course of the
    battle) For dossiers with many (or all) turns recorded for each battle, this
    results in a significant reduction of the memory usage by the application,
    as well as providings a strong reduction of the size of the dossier fiels

* Improvement - reduced stratmap rendering time
    By reusing the single battle map for each turn, the stratmap can now avoid
    re-rendering the battle map when it is not changed (i.e. when rendering a
    different turn from the same battle)

* Improvement - Improved savegame list filtering
    When adding new savegames, the savegame list in the dialog box filters all
    savegames which have already been added to the dossier, to make it easier to
    select the new savegames from the list. This filtering was not 100% correct,
    sometimes causing it to incorrectly hide savegames which were not yet added
    to the dossier.

* Improvement - Improved dossier Load/Save/SaveAs progress bars
    The dossier Load/Save/SaveAs progress bars now also reflect the
    Loading/Saving of the different battles in the dossier, providing a more
    gradual progression of the progress bar, instead of a single big jump after
    a long waiting time.

## This release has the following fixes:

* Fix - fixed crash after deletion of last battle or turn
    Dossier corruption due to forgotten reset of tfirst/tlast when deleting last
    battle/turn.

* Fix - fixed dossier corruption after deletion of any battle
    Dossier corruption due to incomplete UHT cleaning.

* Fix - Fixed unit readiness calculation for explosives
    For units without headcount (like explosives) the unit readiness calculation
    was incorrect.

* Fix - Fixed incorrect formation detection for winSPWW2 and winSPMBT
    
* Fix - Fixed crash highlighting decommissioned units in the unit history

* Fix - fixed some memory leaks

# Installation

To install: extract the zip file in a folder of your choice. A new warcab folder will be created there.

**The application is standalone and can simply be run from inside this folder.**

## Release files

This release contains the following files:

| filename | description |
| --- | --- |
| warcab_0.8.4.zip | The release build. |
| releasereport_warcab_0.8.4.zip.html | An abbreviated FileAlyzer report that can be used to verify the warcab_0.8.4.zip file integrity. |
| warcab_0.8.4_debug.zip | The debug build, featuring debug logging and memory management tracking. |
| releasereport_warcab_0.8.4_debug.zip.html | An abbreviated FileAlyzer report that can be used to verify the warcab_0.8.4_debug.zip file integrity. |

# Content

This installation provides the following tools:

## warcab.exe

  Warcab is a tool to record and track battles in SP:Waw, winSPWW2 and winSPMBT campaigns, or to record and track turns in standalone battles from scenarios. It is inspired by Campaign Watcher.

  Warcab tracks information for core units (including reassignments) during a campaign. It also shows and tracks information for core, auxilliary and opponent units during battles and battle turns.

  Its current main missing features are data export and printing.

  Demo warcab campaign and standalone dossiers have been included in the "save" folder of this release.

## spwreport.exe

  The spwreport tool is a multi-purpose spwawlib tool that can be used to:

  Generate OOB dumps in CSV format for SP:WaW, winSPWW2 and winSPMBT OOB data files.

  Generate reports for SP:WaW, winSPWW2 and winSPMBT savegames.

  Generate reports for spwawlib snapshot files.

## spwdecompose.exe

  The spwdecompose tool extracts and decompresses all section data of SP:WaW, winSPWW2 and winSPMBT savegames, allowing easy inspection and modification.

## spwreconstruct.exe

  The spwreconstruct tool reconstructs SP:WaW, winSPWW2 and winSPMBT savegames from a set of uncompressed data sections (as generated by spwdecompose). 
