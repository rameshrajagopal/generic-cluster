#!/bin/bash
#find $1 -name "*_test.out" | xargs -I% sh -c %

CWD=$1
BUILD_DIR=$CWD/build/
ROOT_DIR=$CWD
export ROOT_DIR

cd ${BUILD_DIR}
ctest -VV --timeout 120
make_exit_code=$?
cd -

exit $make_exit_code
