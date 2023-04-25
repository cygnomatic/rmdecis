#!/bin/bash

echo ">>> Start installing rmdecis lib ..."
mkdir -p build

UBUNTU_VER=$(lsb_release -r | awk '{print $2}')
if [ "$UBUNTU_VER" == "20.04" ]; then
  echo ">>> Detected Ubuntu 20.04. Enable RMDECIS_DEPLOYMENT."
  cmake -DCMAKE_BUILD_TYPE=Release -DRMDECIS_DEPLOYMENT=ON -S . -B ./build -G Ninja
elif [ "$UBUNTU_VER" == "22.04" ]; then
  echo ">>> Detected Ubuntu 22.04. Disable RMDECIS_DEPLOYMENT"
  cmake -DCMAKE_BUILD_TYPE=Release -S . -B ./build -G Ninja
else
  echo "Unsupported distribution. Please install manually."
  exit 0
fi

echo ">>> Building rmdecis lib ..."
cmake --build ./build --target rmdecis --parallel $(($(nproc) - 1))
echo ">>> Lib rmdecis built."