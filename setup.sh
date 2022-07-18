#!/bin/bash
rm -rf builddir

meson setup builddir/debug --buildtype debug
meson setup builddir/arm --cross-file arm-linux-gnueabihf.ini --buildtype release