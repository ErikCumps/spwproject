@echo off

rem
rem The spwawlib savegame regression test:
rem this tool updates the regression test baseline data
rem

echo Performing _BASE_ cleanup...
del /Q _BASE_\*.*		1>nul 2>nul

echo Performing _RPT_ copy...
xcopy /Y _RPT_\*.* _BASE_	1>nul 2>nul

:DONE
rem done
