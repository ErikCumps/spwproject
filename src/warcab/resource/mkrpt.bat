@echo off

set BIN=..\..\..\bin

if "x%1"=="x-d" goto DUMP

%BIN%\resrpt warcab.res
goto DONE

:DUMP
mkdir resdump
cd resdump
..\%BIN%\resrpt -d ..\warcab.res
cd ..
goto DONE

:DONE
