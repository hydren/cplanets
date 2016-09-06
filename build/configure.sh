#!/bin/sh

cd ../src_libs/SDL_widgets
echo "Running configure script for SDL_widgets at $PWD ..."
./configure
cd ../../build

OLD="BUILD_PATH"
INCLUDE_PATH=${PWD%/*}
DPATH=${PWD}

echo "Ready to touch .mk files within the current folder (and subfolders, recursively)."
echo "This will replace $OLD tokens with $INCLUDE_PATH" 

find $DPATH -name '*.mk' -type f -exec echo "Patched " {} \;
find $DPATH -name '*.mk' -type f -exec sed -i "s,$OLD,$INCLUDE_PATH,g" {} \;
