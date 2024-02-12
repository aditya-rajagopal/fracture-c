REM Build script for the fracture engine
@ECHO OFF
SetLocal EnableDelayedExpansion

REM Get a list of all the .c files
SET cFileNames=
FOR /R %%f in (*.c) do (
    SET cFileNames=!cFileNames! %%f
)

REM echo "Files: " %cFileNames%
SET assembly=base
SET compilerFlags=-g
REM -Wall -Werror
SET includeFlags=-I..\base\src
SET linkerFlags=
SET defines=-D_DEBUG -D_CRT_SECURE_NO_WARNINGS

ECHO "Buildin %assembly%..."
REM Compile C files to object files
clang -c %compilerFlags% %defines% %includeFlags% %cFileNames% -o ../bin/%assembly%.obj
lib ../bin/%assembly%.obj


REM "Writing the compile_flags.txt file"
ECHO "Writing the compile_flags.txt file"
echo %includeFlags% %defines% %compilerFlags% | sed -e "s/-I/-I\\/g" -e "s/ /\n/g" -e "s/-I\\C/-IC/g" -e "s/-I\\..\\/-I..\\/g"> compile_flags.txt