@echo off

set RUNTEST_NO_PAUSE=1

echo ^+----------------------------^+
echo ^| Starting regresion testing ^|
echo ^+----------------------------^+
echo.

for /D %%d in (set*) do (
	echo Performing regression test in %%d...
	pushd %%d >nul 2>nul
	call runtest.bat
	diffstat DIFF >DIFFSTAT.txt
	popd >nul 2>nul
)
echo.

echo ^+--------------^+
echo ^| DIFF reports ^|
echo ^+--------------^+
echo.

for /D %%d in (set*) do (
	find /C "---" %%d\DIFF
	type %%d\DIFFSTAT.txt
)
echo.

pause
