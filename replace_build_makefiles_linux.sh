#!/bin/bash
echo "Erasing content of build folder (except configure.sh)..."
mv build/configure.sh .
rm build/* -rf
mv configure.sh build/
echo "Performing make clean in linux-release folder..."
cd linux-release
make clean
cd ..
echo "Copying makefiles from linux-release folder to build folder..."
cp linux-release/* -r build/
echo "Replacing current folder with generic BUILD_PATH token..."
./touch_makefiles.sh ./build/ ${PWD} BUILD_PATH
