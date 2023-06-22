#!/bin/bash
set -ex


test-one-build () {
    CFG="$1"
    shift
    BUILDDIR="build-$CFG"

    rm -rf -- "$BUILDDIR"
    mkdir -- "$BUILDDIR"
    pushd -- "$BUILDDIR"

    # Run unit tests.
    cmake "$@" -DCMAKE_BUILD_TYPE="$CFG" ..
    make -j8
    make test

    # Run unit tests with valgrind.
    cmake "$@" -DCMAKE_BUILD_TYPE="$CFG" -DENABLE_ASAN=0 ..
    make -j8 unit-test
    ctest -T memcheck

    popd
}

test-one-build Debug "$@"
test-one-build Release "$@"

echo "All tests passed."

