echo ">>> Start installing rmdecis lib ..."
mkdir build
echo ">>> Installing dependency with Conan ..."
conan install . --install-folder=./build --build=missing -s build_type=Release
echo ">>> Building rmdecis lib ..."
cmake -DCMAKE_BUILD_TYPE=Release -S . -B ./build
cmake --build ./build --target rmdecis -j 8
echo ">>> Installing rmdecis lib to system ..."
sudo cmake --install ./build --config Release
echo ">>> Update ldconfig ..."
sudo ldconfig
echo ">>> Lib rmdecis installed."