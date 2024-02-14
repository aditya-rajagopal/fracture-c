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
SET includeFlags=-Isrc -I..\fracture\src -I..\base\src
SET linkerFlags=-L../bin/ -lfracture.lib -lbase.lib
SET defines=-D_DEBUG -DFR_IMPORT -D_ENABLE_ASSERTS

ECHO "Buildin %assembly%...."
clang %cFileNames% %comilerFlags% -o ../bin/%assembly%.exe %defines% %includeFlags% %linkerFlags%

REM "Writing the compile_flags.txt file"
ECHO "Writing the compile_flags.txt file"
echo %includeFlags% %defines% %comilerFlags% | sed -e "s/-I/-I\\/g" -e "s/ /\n/g" -e "s/-I\\C/-IC:/g" -e "s/-I\\..\\/-I..\\/g" > compile_flags.txt
