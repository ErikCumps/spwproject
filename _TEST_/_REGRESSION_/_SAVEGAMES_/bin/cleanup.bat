@echo off

rem
rem The spwawlib savegame regression test:
rem this tool cleans up the regression test data
rem

echo Performing _RPT_ and _SNP_ cleanup...
del /Q _RPT_\*.*		1>nul 2>nul
del /Q _SNP_\*.*		1>nul 2>nul

echo Performing DIFF data cleanup...
del /Q DIFF			1>nul 2>nul
del /Q DIFF_*			1>nul 2>nul
del /Q DIFFSTAT.txt		1>nul 2>nul
del /Q DIFFSTAT_*.txt		1>nul 2>nul

:DONE
rem done
