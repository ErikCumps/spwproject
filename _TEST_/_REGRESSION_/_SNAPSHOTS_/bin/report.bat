@echo off

rem
rem The spwawlib snapshot regression test:
rem this is the spwawlib snapshot reporting tool
rem

if "x%1" == "x" goto USAGE

mkdir _RPT_		1>nul 2>nul

set REPORT=REPORT_%1

set SPWAWLIB_DISABLE_LOGTIME=1

echo Generating reports for %REPORT%...

..\..\..\bin\spwreport.exe snapreport _SNAP_\snapshot_%1.snap %REPORT%
move %REPORT%*.txt _RPT_\		1>nul 2>nul
move %REPORT%*.xml _RPT_\		1>nul 2>nul
move %REPORT%*.dump _RPT_\		1>nul 2>nul
move SPWAWLIB.log _RPT_\SPWAWLIB_%1.log	1>nul 2>nul
del *.log				1>nul 2>nul

goto DONE

:USAGE
echo Usage: report.bat idx
echo Where: idx   snapshot index number to generate report for
goto DONE

:DONE
rem done
