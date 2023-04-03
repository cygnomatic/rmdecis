#!/bin/bash

ARCH=$(uname -m)

echo ">>> Start installing rmdecis lib ..."
mkdir -p build

echo ">>> Building rmdecis lib ..."
if [ "${ARCH}" == "x86_64" ]; then
  echo ">> Detected arch: amd64."
  cmake -DCMAKE_BUILD_TYPE=Release -S . -B ./build -G Ninja
elif [ "${ARCH}" == "aarch64" ]; then
  echo ">> Detected arch: aarch64."
  cmake -DCMAKE_BUILD_TYPE=Release -DRMDECIS_DEPLOYMENT=ON -S . -B ./build -G Ninja
else
  echo "(${ARCH}) not supported."
fi

cmake --build ./build --target rmdecis --parallel $(($(nproc) - 1))
echo ">>> Installing rmdecis lib to system ..."
sudo cmake --install ./build --config Release
echo ">>> Update ldconfig ..."
sudo ldconfig
echo ">>> Lib rmdecis installed."