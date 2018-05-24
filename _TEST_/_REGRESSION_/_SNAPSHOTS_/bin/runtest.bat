@echo off

rem
rem The spwawlib snapshot regression test tool
rem

echo ### Preparing testrun...
rmdir /S /Q _RPT_ 1>nul 2>nul
echo.

echo ### Generating new reports...
call report_all.bat
echo.

echo ### Comparing reports...
diff -U3 -r _BASE_ _RPT_ >DIFF
echo.

if "x%RUNTEST_NO_PAUSE%" == "x" goto DOPAUSE 
goto DONE

:DOPAUSE
pause

:DONE
