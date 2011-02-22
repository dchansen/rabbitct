@ECHO OFF

devenv.com /build > nul

if errorlevel 1 goto havenodevenv
goto havedevenv
:havenodevenv
echo Error: devenv.com not found.
echo        This script has to be run from the Visual Studio Command Prompt!
goto :eof
:havedevenv

REM BEGIN CONFIGURATION

REM Choose your generator wisely.

REM GENERATOR="Visual Studio 8 2005"
REM GENERATOR="Visual Studio 8 2005 Win64"
REM GENERATOR="Visual Studio 9 2008"
SET GENERATOR="Visual Studio 9 2008 Win64"

REM END CONFIGURATION

SET TEST_ROOT=build-test-win

echo Before we get our hands dirty: cleaning up
REM remove_directory is stronger than rmdir
cmake -E remove_directory %TEST_ROOT%

echo Creating temporary build directory
cmake -E make_directory %TEST_ROOT%

echo Generating project files
echo.
cmake -E chdir %TEST_ROOT% cmake -G %GENERATOR% ..\..\src\

REM goto skipdebugmode
echo.
echo ====================
echo = Building DEBUG   =
SET RELEASE_MODE=Debug
echo.
REM cmake --build %TEST_ROOT%\%RELEASE_MODE%
cmake -E chdir %TEST_ROOT% devenv.com RabbitCT.sln /build %RELEASE_MODE% /project ALL_BUILD
if errorlevel 1 goto errBuildDebug
echo.
:skipdebugmode

REM goto skipreleasemode
echo.
echo ====================
echo = Building RELEASE =
SET RELEASE_MODE=Release
echo.
REM cmake --build %TEST_ROOT%\%RELEASE_MODE%
cmake -E chdir %TEST_ROOT% devenv.com RabbitCT.sln /build %RELEASE_MODE% /project ALL_BUILD
if errorlevel 1 goto errBuildRelease
echo.
:skipreleasemode

REM No errors so far? Then skip the error messages!
goto :done

:errBuildDebug
echo.
echo ====================
echo !! ERROR          !!
echo Error building Debug mode binaries.
echo If you got an x86/x64 conflict make sure you ran the script using the correct (x86/x64) version of the VS Command Prompt.
goto :eof

:errBuildRelease
echo.
echo ====================
echo !! ERROR          !!
echo Error building Release mode binaries.
echo If you got an x86/x64 conflict make sure you ran the script using the correct (x86/x64) version of the VS Command Prompt.
goto :eof

:done
echo done.
