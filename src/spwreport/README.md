#spwreport
This is the SP:WaW and winSPWW2 report tool.

Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>

License: GPL V2.

This is a multi-purpose spwawlib tool that can be used to:

Generate an OOB dump in CSV format (spwoobdump_*.csv files).

    Usage: spwreport oobdump GAME OOB [raw]
    Where: GAME    game type
           OOB     path to oob dir
           raw     specify "raw" for a raw oob data dump

Generate savegame reports:

    Usage: spwreport savereport GAME OOB DIR INDEX BASE [TYPE]
    Where: GAME    game type
           OOB     path to oob dir
           DIR     path to savegame dir
           INDEX   index of savegame to generate report for
           BASE    basename for report files
           TYPE    optional type of report to generate (defaults to ALL reports)

Generate snapshot reports:

    Usage: spwreport snapreport SNAP BASE [TYPE]
    Where: SNAP    filename of snapshot to generate report for
           BASE    basename for report files
           TYPE    optional type of report to generate (defaults to ALL reports)

Supported game types:
    SPWAW       SP:WaW
    WINSPWW2    winSPWW2

Supported report types:
    raw         raw savegame content reports (*_RAW.txt and *_RAWLINK.txt)
    elaborate   an elaborate report (*_complete.txt)
    normal      a normal report (*_short.txt)
    narrative   a normal report in a narrative format (*_narrative.txt)
    table       a normal report in table format (*_table.txt)
    map         a map and unit positions dump (*_positions.dump)

Two reports are generate for elaborate, normal, narrative and table reports:
+ a full report covering the full player1 and player2 forces
+ a core report covering the player1 core force only
