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
	popd >nul 2>nul
	
)

echo ^+--------------^+
echo ^| DIFF reports ^|
echo ^+--------------^+
echo.

for /D %%d in (set*) do (
	find /C "---" %%d\DIFF
)

pause
