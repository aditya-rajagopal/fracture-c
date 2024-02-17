REM Build script for the fracture engine
@ECHO OFF
SetLocal EnableDelayedExpansion

REM Get a list of all the .c files
SET cFileNames=
FOR /R %%f in (*.c) do (
    SET cFileNames=!cFileNames! %%f
)

REM echo "Files: " %cFileNames%
SET assembly=fracture
SET comilerFlags=-g -shared -Wvarargs -Wall -Werror
REM -Wall -Werror
SET includeFlags=-I..\fracture\src -I%VULKAN_SDK%\Include -I..\core\src -I..\platform\src
SET linkerFlags=-luser32 -lvulkan-1 -L%VULKAN_SDK%/Lib -L..\bin\ -lcore.lib -lplatform.lib
SET defines=-D_DEBUG -DFR_EXPORT -D_CRT_SECURE_NO_WARNINGS -D_ENABLE_ASSERTS -DPLATFORM_WINDOWS

ECHO %VULKAN_SDK%

ECHO "Buildin %assembly%..."
clang %cFileNames% %comilerFlags% -o ../bin/%assembly%.dll %defines% %includeFlags% %linkerFlags% -MTd

REM "Writing the compile_flags.txt file"
ECHO "Writing the compile_flags.txt file"
echo %includeFlags% %defines% %comilerFlags% | sed -e "s/-I/-I\\/g" -e "s/ /\n/g" -e "s/-I\\C/-IC/g" -e "s/-I\\..\\/-I..\\/g"> compile_flags.txt
