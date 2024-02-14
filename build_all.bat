@ECHO OFF
REM Build Everything

ECHO "Building everything...."

PUSHD base
CALL build.bat
POPD
IF %ERRORLEVEL% NEQ 0 GOTO :error

PUSHD fracture
CALL build.bat
POPD
IF %ERRORLEVEL% NEQ 0 GOTO :error 

PUSHD testbed
CALL build.bat
POPD
IF %ERRORLEVEL% NEQ 0 GOTO :error

ECHO "Done!"
GOTO :end

:error
ECHO Error: %ERRORLEVEL%
ECHO "Build failed!"
:end