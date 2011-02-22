#!/bin/sh

# BEGIN CONFIGURATION

# Choose the problem size you want to test.
PROBLEM_SIZE=128
# PROBLEM_SIZE=256
# PROBLEM_SIZE=512

# Choose the data set to use for the test.
DATASET="${HOME}/datasets/rabbitct_512.rct"

# Choose where to store the reconstructed results.
OUTPUT="/tmp/rabbitct_run-test-linux_out.rctd"

# Choose the module you want to test [default: LolaBunny].
MODULE=LolaOpenMP

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

echo Before we get our hands dirty: test if directories and files exist
if [ ! -e ${DATASET} ]
then
	trigger_error "Data set not found [${DATASET}]."
fi
if [ ! -d ${TEST_ROOT} ]
then
	trigger_error "Build directory [${TEST_ROOT}] or binaries (of RabbitCTRunner or module) not found. Did you run build-test-win before?"
fi

RUNNER_BIN="${TEST_ROOT}/RabbitCTRunner/RabbitCTRunner"
if [ ! -e ${RUNNER_BIN} ]
then
	trigger_error "Build directory [${TEST_ROOT}] or binaries (of RabbitCTRunner or module) not found. Did you run build-test-win before?"
fi

MODULE_BIN="${TEST_ROOT}/modules/${MODULE}/lib${MODULE}.so"
if [ ! -e ${MODULE_BIN} ]
then
	trigger_error "Build directory [${TEST_ROOT}] or binaries (of RabbitCTRunner or module) not found. Did you run build-test-win before?"
fi

CMD="${RUNNER_BIN} ${MODULE_BIN} ${DATASET} ${OUTPUT} ${PROBLEM_SIZE}"

echo "Running [${CMD}]"
${CMD}

# No errors so far? Then skip the error messages!
echo done.
exit 0

