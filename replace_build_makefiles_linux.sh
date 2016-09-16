#!/bin/bash
echo "Erasing content of build folder (except configure.sh)..."
mv build/configure.sh .
rm build/* -rf
mv configure.sh build/
echo "Performing make clean in linux-gcc-release folder..."
cd linux-gcc-release
make clean
cd ..
echo "Copying makefiles from linux-gcc-release folder to build folder..."
cp linux-gcc-release/* -r build/
echo "Replacing current folder with generic BUILD_PATH token..."
./touch_makefiles.sh ./build/ ${PWD} BUILD_PATH
