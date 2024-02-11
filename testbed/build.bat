REM Build script for the testbed
@ECHO OFF
SetLocal EnableDelayedExpansion

REM Get a list of all the .c files
SET cFileNames=
FOR /R %%f in (*.c) do (
    SET cFileNames=!cFileNames! %%f
)

REM echo "Files: " %cFileNames%
SET assembly=testbed
SET comilerFlags=-g
REM -Wall -Werror
SET includeFlags=-Isrc -I../fracture/src
SET linkerFlags=-L../bin/ -lfracture.lib
SET defines=-D_DEBUG -DFR_IMPORT


ECHO "Buildin %assembly%...."
clang %cFileNames% %comilerFlags% -o ../bin/%assembly%.exe %defines% %includeFlags% %linkerFlags%
