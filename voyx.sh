#!/bin/bash

PATH="/opt/homebrew/opt/qt@5/bin:$PATH"

mkdir -p build-release

pushd build-release >/dev/null 2>&1
cmake -DCMAKE_BUILD_TYPE=Release .. || exit $?
cmake --build . || exit $?
popd >/dev/null 2>&1

build-release/voyx $@
