@ECHO OFF

REM BEGIN CONFIGURATION

REM Choose the build mode you want to test.
REM SET RELEASE="Debug"
SET RELEASE=Release

REM Choose the problem size you want to test.
SET PROBLEM_SIZE=128
REM SET PROBLEM_SIZE=256
REM SET PROBLEM_SIZE=512

REM Choose the data set to use for the test.
SET DATASET="\datasets\rabbitct_512.rct"

REM Choose where to store the reconstructed results.
SET OUTPUT="\temp\rabbitct_run-test-win_out.rctd"

REM Choose the module you want to test [default: LolaBunny].
SET MODULE=LolaOpenMP

REM END CONFIGURATION

SET TEST_ROOT=build-test-win

echo Before we get our hands dirty: test if directories and files exist
IF NOT EXIST %DATASET% goto errDatasetMissing
IF NOT EXIST %TEST_ROOT% goto errRunBuildTest

SET RUNNER_BIN="%TEST_ROOT%\RabbitCTRunner\%RELEASE%\RabbitCTRunner.exe"
IF NOT EXIST %RUNNER_BIN% goto errRunBuildTest

SET MODULE_BIN="%TEST_ROOT%\modules\%MODULE%\%RELEASE%\%MODULE%.dll"
IF NOT EXIST %MODULE_BIN% goto errRunBuildTest

SET CMD=%RUNNER_BIN% %MODULE_BIN% %DATASET% %OUTPUT% %PROBLEM_SIZE%

echo Running [%CMD%]
%CMD%


REM No errors so far? Then skip the error messages!
goto :done

:errDatasetMissing
echo.
echo ====================
echo !! ERROR          !!
echo Dataset not found [%DATASET%].
goto :eof

:errRunBuildTest
echo.
echo ====================
echo !! ERROR          !!
echo Build directory [%TEST_ROOT%] or binaries (of RabbitCTRunner or module) not found. Did you run build-test-win before?
goto :eof

:done
echo done.
