REM Build script for the fracture engine
@ECHO OFF
SetLocal EnableDelayedExpansion

REM Get a list of all the .c files
SET cFileNames=
FOR /R %%f in (*.c) do (
    SET cFileNames=!cFileNames! %%f
)

REM echo "Files: " %cFileNames%
SET assembly=platform
SET compilerFlags=-g
REM -Wall -Werror
SET includeFlags=-I..\platform\src -I..\fracture\src\
SET linkerFlags=
SET defines=-D_DEBUG -D_CRT_SECURE_NO_WARNINGS -DPLATFORM_WINDOWS

ECHO "Buildin %assembly%..."
REM Compile C files to object file
FOR %%f in (%cFileNames%) do (
    clang -c %compilerFlags% %%f %defines% %includeFlags% -o ../obj/%%~nf.obj
)
clang -fuse-ld=llvm-lib -o ..\bin\%assembly%.lib ../obj/*.obj %linkerFlags%


REM "Writing the compile_flags.txt file"
ECHO "Writing the compile_flags.txt file"
echo %includeFlags% %defines% %compilerFlags% | sed -e "s/-I/-I\\/g" -e "s/ /\n/g" -e "s/-I\\C/-IC/g" -e "s/-I\\..\\/-I..\\/g"> compile_flags.txt