#!/bin/sh

# BEGIN CONFIGURATION
# This script does not have a configuration
# END CONFIGURATION

TEST_ROOT='build-test-linux'

trigger_error()
{
	echo
	echo ====================
	echo !! ERROR          !!
	echo $1
	exit 1
}

echo Before we get our hands dirty: cleaning up
# remove_directory is stronger than rmdir
cmake -E remove_directory ${TEST_ROOT}

echo Creating temporary build directory
cmake -E make_directory ${TEST_ROOT}

echo Generating project files
echo
cmake -E chdir ${TEST_ROOT} cmake ../../src/

echo
echo ====================
echo = Building         =
echo
CMD="cmake -E chdir ${TEST_ROOT} make"
${CMD}

if [ "$?" -ne "0" ]
then
	trigger_error "Error building binaries."
fi
echo


# No errors so far? Then skip the error messages!
echo done.
exit 0

