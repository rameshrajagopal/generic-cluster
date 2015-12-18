#!/bin/bash
CWD=$1
BUILD_DIR=$CWD/build/

if [ "$2" != "--noclean" ]; then
    printf "Cleaning the build directory...\n"
    rm -rf ${BUILD_DIR}/*
fi

printf "Running cmake...\n"

cmake -H${CWD} -B${BUILD_DIR}
cd ${BUILD_DIR}
printf "Running make all...\n"
make all
make_exit_code=$?
cd -
printf "Done.\n"
if [[ $make_exit_code != 0 ]]; then
    exit $make_exit_code
fi
./run_tests.sh .
