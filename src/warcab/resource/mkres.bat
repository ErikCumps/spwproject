@echo off

set BIN=..\..\..\bin

if not exist warcab.res goto DOIT
if not exist %BIN%\isfilenewer.exe goto DOIT
%BIN%\isfilenewer resource.cfg resource.c
if errorlevel 2 goto ERROR
if errorlevel 1 goto DOIT
goto DONE

:DOIT
if not exist resource.c goto SKIPBACKUP
rename resource.c resource.c.old
rename resource.h resource.h.old
:SKIPBACKUP
%BIN%\buildres -ol warcab.res -oh resource.h -oc resource.c resource.cfg
if errorlevel 1 goto ERROR
goto CLEANUP

:ERROR
echo *** An error occurred!
echo.
if exist resource.c.old goto RESTOREBACKUP
goto DONE

:RESTOREBACKUP
del resource.c resource.h		>nul 2>nul
rename resource.c.old resource.c	>nul 2>nul
rename resource.h.old resource.h	>nul 2>nul
goto DONE

:CLEANUP
del resource.c.old			>nul 2>nul
del resource.h.old			>nul 2>nul
goto DONE

:DONE
