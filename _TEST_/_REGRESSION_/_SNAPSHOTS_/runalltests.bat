@echo off

set RUNTEST_NO_PAUSE=1

echo ^+-------------------------------^+
echo ^| Performing regression testing ^|
echo ^+-------------------------------^+
echo.

for /D %%d in (set*) do (
	echo Running regression test in %%d...
	pushd %%d >nul 2>nul
	call runtest.bat
	diffstat DIFF_BASE.diff >DIFFSTAT.txt
	popd >nul 2>nul
)
echo.

echo ^+--------------^+
echo ^| DIFF reports ^|
echo ^+--------------^+
echo.

for /D %%d in (set*) do (
	find /C "---" %%d\DIFF_BASE.diff
	type %%d\DIFFSTAT.txt
)
echo.

pause
