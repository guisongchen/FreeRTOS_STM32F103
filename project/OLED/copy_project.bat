@echo off

echo remove dummy project files

set "currPath=%~dp0"

if exist %currPath%DebugConfig (
rd /s/q DebugConfig
) else (
echo DebugConfig already removed
)

if exist %currPath%Listings (
rd /s/q Listings
) else (
echo Listings already removed
)

if exist %currPath%Objects (
rd /s/q Objects
) else (
echo Objects already removed
)

echo remove done!

echo rename project

for %%i in ("%cd%") do set "project_name=%%~ni"

ren *.uvguix.CCC %project_name%.uvguix.CCC
ren *.uvoptx %project_name%.uvoptx
ren *.uvprojx %project_name%.uvprojx

echo rename done!

echo.

dir %cd%

pause