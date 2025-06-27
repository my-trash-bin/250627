#!/bin/sh

set -e
cd "$(dirname "$0")"

mkdir -p builddir/out
cmake -B builddir/p250627 -DCMAKE_INSTALL_PREFIX=builddir/out
cmake --build builddir/p250627 --config Debug
cmake --install builddir/p250627 --config Debug

builddir/out/bin/p250627
