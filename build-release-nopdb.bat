@echo off
cd /d "%~dp0"

findstr /x /c:"DESKTOP_APP_NO_PDB:BOOL=ON" out\CMakeCache.txt >nul
if errorlevel 1 (
	cmake -S . -B out -DDESKTOP_APP_NO_PDB=ON
	if errorlevel 1 exit /b 1
)

cmake --build out --config Release --parallel