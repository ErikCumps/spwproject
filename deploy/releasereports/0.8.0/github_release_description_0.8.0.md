Welcome to the SPWaW war cabinet, release 0.8.0.

Copyright (C) 2005-2020 Erik Cumps erik.cumps@gmail.com

License: GPL V2.

# Release information

## IMPORTANT NOTE!

The list of units detected by Warcab version 0.6.1 and older can be different
from the list of units detected by this version of Warcab. This means in fact
that the dossier files created by version 0.6.1 and older are no longer
compatible with and cannot be loaded by this release.

## This releases adds the following new feature:

* Support for multiple game types:   (currently SP:WaW and winSPWW2)
    + date-specific OOB nation info and flags
    + map hex connection and tramlines detection
    + map hex height range expanded and unified
    + unified unit and formation types
    + (SP:WaW) formations can contain core units as well as support units
    + (winSPWW2) month-only battle/turn dates
    + (winSPWW2) Disabled time-based axis for dossier graphs.

* Unit History Tracking:
    The entire history of units in a campaign is now tracked. This includes
    additional units commissioned after the start of the campaign, units
    decomissioned before the end of the campaign and units cross-attached to
    different formations during the campaign.

* Full campaign history preference:
    This preference enables full campaign history, which also shows data for
    decommissioned units on the following Dossier tabs: Overview, Status,
    Progress, Kills, Losses, Roster and History.

* Full, limited and None intel mode:
    Three intel modes are available (full, limited and none) to select the
    amount of information about the opponent forces that should be available.
    + Full: all information is available
    + Limited: limited information is available:
        + The opponent's OOB is available, but the battle status is not.
        + The stratmap shows the uncertainty of the frontline.
        + Only destroyed, abandoned or spotted opponent units are indicated.
    + None: no information is available:
        + Victory hex ownership by the opponent is not available.
        + The opponent's OOB is not available.
        + The opponent's unit history is not available.
        + The stratmap shows the uncertainty of the contested map.
        + Only destroyed, abandoned or spotted opponent units are indicated.

* Additional height color fields for stratmap:
    To liven up the stratmap a bit, additional height color fields are provided:
    + grey: the original greyish colors.
    + topographic: washed out colors, derived from topographic maps
    + terrain: washed out colors, based on the map terrain type

* Campaign battle results:
    The results of a campaign battle (Decisive Victory, Marginal Victory, Drawn
    Battle, Marginal Defeat, Decisive Defeat) are now recorded and reported.
    Note: they are not reported for older dossiers (when battle results were not
    yet recorded).

* Local and global preferences:
    Warcab preferences can now be stored globally (in the registry), or locally
    (in a warcab.ini file).

* XML reporting:
    The spwreport tool now also supports reporting savegame information using
    the XML file format.

## This release adds the following improvements:

* Much improved unit detection for SP:WaW:
    Based on information learned from winSPWW2, the SP:WaW unit detection has
    been much improved, significantly reducing the risk of misdetection.

* Initial support for unit designations
    Although unit names are initialized by the games to reflect the unit
    equipment name, these names are actually unit designations which can change
    during the course of a battle and/or a campaign. Initial support for these
    unit designations has been added.

* Changed unit 'radio' status to 'contact' status:
    The old unit radio status is now reported as a unit contact status (No
    Contact, Radio Contact, Direct Contact)

* Updated loaded unit position recording:
    For the position of a loaded unit, the position of the loading unit is
    recorded.

* Victory hex statistics reporting:
    The occupation status of the victory hexes is now reported for each battle
    turn.

* Only show the force progress tab when relevant:
    The tab is only shown for the player core force during a campaign.

* Only show the support force tab when relevant:
    The tab is not shown when there is no support force present.

* Only report battle losses/force turn status when relevant:
    These are now only reported for the final (score) turns of campaign battles.

* Many small overall improvements for all reports.

* Campaign battle index:
    The campaign battle index tracks the position of the battle in the campaign,
    compared to other battles. It is used to distinguish and sort the different
    battles of a campaign, replacing the date-based mechanism which did no
    longer function with month-only dates.

* Improved savegame unpacked data handling:
    When the size of the savegame section is not fixed (or unknown), buffers are
    auto-allocated to load the uncompressed section data in.

* Some cosmetic improvements.

* Better separation of Debug and Release deliverables:
    This allows deploying both in same target directory.

## This release has the following fixes:

* Fixed issue #7:
    MMAS bad min/max unit reporting

* Dossier/snapshot saving:
    Don't destroy existing file if saving fails.

* Not setting dossier info:
    The dossier info was not saved in the dossier file and was lost after
    loading the dossier file.

* Graph horizontal axis label redraw problem:
    When moving from battle to battle, the orizontal axis labels where not
    regenerated when the number of items on the axis did not change between
    battles.

* Terrain height mapping and water hex detection:
    Some hexes were incorrectly detected to be below 0 meter or to contain
    water.

* Column (re)sorting bug
    Do not (re)sort if no column is selected for sorting.

* ModelHistory data bug
    Fixed bug causing ModelHistory to incorrectly keep using data from previous
    dossier when no data is available for the current dossier. (e.g. when the
    old dossier contained support forces but the new dossier does not)

* Fixed crashes and memory corruptions/leaks

* Updated documentation:
        + added SP:WaW and winSPWW2 OOB file format documentation
        + updated SP:WaW save file sections format documentation
        + added winSPWW2 save file sections format documentation
        + updated existing and added some new demo dossiers

# Installation

To install: extract the zip file in a folder of your choice. A new warcab folder will be created there.

**The application is standalone and can simply be run from inside this folder.**

## Release files

This release contains the following files:

| filename | description |
| --- | --- |
| warcab_0.8.0.zip | The release build. |
| releasereport_warcab_0.8.0.zip.html | An abbreviated FileAlyzer report that can be used to verify the warcab_0.8.0.zip file integrity. |
| warcab_0.8.0_debug.zip | The debug build, featuring debug logging and memory management tracking. |
| releasereport_warcab_0.8.0_debug.zip.html | An abbreviated FileAlyzer report that can be used to verify the warcab_0.8.0_debug.zip file integrity. |

# Content

This installation provides the following tools:

## warcab.exe

  Warcab is a tool to record and track battles in an SPWaW campaign, or to record and track turns in standalone battles from scenarios. It is inspired by Campaign Watcher.

  Warcab tracks information for core units (including reassignments) during a campaign. It also shows and tracks information for core, auxiliary and opponent units during battles and battle turns.

  Its current main missing features are data export and printing.

  Demo warcab campaign and standalone dossiers have been included in the "save" folder of this release. 

## spwreport.exe

  The spwreport tool generates SPWaW savegame reports.

  It can also generate a CSV dump of the SPWaW OOB file content.

## spwdecompose.exe

  The spwdecompose tool extracts the uncompressed section data of a SPWaW savegame, allowing easy inspection and modification.

## spwreconstruct.exe

  The spwreconstruct tool reconstucts a SPWaW savegame from a set of uncompressed data sections (as generated by spwdecompose),
