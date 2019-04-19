@echo off

rem
rem The spwawlib savegame regression test:
rem this is the spwawlib savegame reporting tool
rem

if "x%1" == "x" goto USAGE

mkdir _RPT_		1>nul 2>nul
mkdir _SNP_		1>nul 2>nul
del SPWAWLIB.log	1>nul 2>nul

set REPORT=REPORT_%1

set SPWAWLIB_DISABLE_LOGTIME=1

echo Generating reports for %REPORT%...

..\..\..\bin\spwreport.exe savereport SPWAW _OOB_ _SAVE_ %1 %REPORT%
move %REPORT%*.txt _RPT_\		1>nul 2>nul
move %REPORT%*.dump _RPT_\		1>nul 2>nul
move SPWAWLIB.log _RPT_\SPWAWLIB_%1.log	1>nul 2>nul
del *.log				1>nul 2>nul

..\..\..\bin\spwreport.exe snapreport snapshot_%1.snap %REPORT%
del %REPORT%_RAW.txt			1>nul 2>nul
move %REPORT%*.txt _SNP_\		1>nul 2>nul
move %REPORT%*.dump _SNP_\		1>nul 2>nul
rem move SPWAWLIB.log _SNP_\SPWAWLIB_%1.log	1>nul 2>nul
del *.log				1>nul 2>nul

del *.snap				1>nul 2>nul

goto DONE

:USAGE
echo Usage: report.bat idx
echo Where: idx   savegame index to generate report for
goto DONE

:DONE
rem done
