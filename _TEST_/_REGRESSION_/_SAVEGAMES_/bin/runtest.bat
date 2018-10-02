@echo off

rem
rem The spwawlib savegames regression test tool
rem

echo ### Preparing testrun...
rmdir /S /Q _RPT_ 1>nul 2>nul
echo.

echo ### Generating new reports...
call report_all.bat
echo.

echo ### Comparing reports...
diff -U3 -r _BASE_ _RPT_ >DIFF_BASE.diff
diff -U3 -r _RPT_  _SNP_ >DIFF_SNAP.diff
echo.

if "x%RUNTEST_NO_PAUSE%" == "x" goto DOPAUSE 
goto DONE

:DOPAUSE
pause

:DONE
