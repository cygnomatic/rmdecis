#!/bin/bash

ARCH=$(uname -m)

echo ">>> Start installing rmdecis lib ..."
mkdir -p build

if [ "${ARCH}" == "x86_64" ]; then
  echo ">>> Detected arch: amd64. Disable RMDECIS_DEPLOYMENT"
  cmake -DCMAKE_BUILD_TYPE=Release -S . -B ./build -G Ninja
elif [ "${ARCH}" == "aarch64" ]; then
  echo ">>> Detected arch: aarch64. Enable RMDECIS_DEPLOYMENT."
  cmake -DCMAKE_BUILD_TYPE=Release -DRMDECIS_DEPLOYMENT=ON -S . -B ./build -G Ninja
else
  echo "(${ARCH}) not supported. Fallback to amd64."
  cmake -DCMAKE_BUILD_TYPE=Release -S . -B ./build -G Ninja
fi

echo ">>> Building rmdecis lib ..."
cmake --build ./build --target rmdecis --parallel $(($(nproc) - 1))