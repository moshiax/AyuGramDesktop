@echo OFF

setlocal enabledelayedexpansion
set "FullScriptPath=%~dp0"

set "PATH=%PATH:C:\msys64\mingw64\bin;=%"
set "PATH=%PATH:C:\msys64\mingw32\bin;=%"
set "PATH=%PATH:C:\msys64;=%"

python "%FullScriptPath%configure.py" %*
if %errorlevel% neq 0 goto error

exit /b

:error
echo FAILED
exit /b 1
