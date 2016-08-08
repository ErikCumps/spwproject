@echo off

set RPT=doc_report.txt
set ERR=doc_errors.txt

echo Preparing...
if exist %RPT% del %RPT%
touch %RPT%
if exist %ERR% del %ERR%
touch %ERR%

echo Generating HTML documentation...
doxygen 1>>%RPT% 2>>%ERR%
if errorlevel 1 goto ERR_DOXYGEN

echo Generating windows CHM documentation...
cd html
..\hhc index.hhp 1>>..\%RPT% 2>>..\%ERR%
cd ..

echo Done!
goto DONE

:ERR_DOXYGEN
echo *** Error: failed to run doxygen! ***
goto DONE

:DONE
rem Done!
