#spwreport
This is the Steel Panthers World at War report tool.

Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>

License: GPL V2.

This tool generates SPWaW savegame reports (and a snapshot of the savegame).

Usage: spwreport OOB DIR INDEX BASE [TYPE]
Where: OOB     path to oob dir
       DIR     path to savegame dir
       INDEX   index of savegame to generate report for
       BASE    basename for report files
       TYPE    optional type of report to generate (defaultd to ALL reports)

Supported report types:
    raw         raw savegame content reports
    elaborate   an elaborate report
    normal      a normal report
    narrative   a normal report in a narrative format
    table       a normal report in table format
    map         a map and unit positions dump

For complete, short, narrative and table reports a set of 2 reports are
generated: a full report covering the full player1 and player2 forces and
a core report covering the player1 core force only.

This tool can also generate an OOB dump in CSV format (spwoobdump_*.csv files).

Usage: spwreport dump OOB
Where: OOB      path to oob dir

