@echo off
cd /d "%~dp0"

powershell -NoProfile -Command "$p='out\Telegram\Telegram.vcxproj'; $x=[IO.File]::ReadAllText($p); $x=[regex]::Replace($x,'(<ItemDefinitionGroup Condition=""''\$\(Configuration\)\|\$\(Platform\)''==''Release\|x64''"">[\s\S]*?<GenerateDebugInformation>)[^<]*','$1false',1); [IO.File]::WriteAllText($p,$x)"

if errorlevel 1 exit /b 1

cmake --build out --config Release --parallel