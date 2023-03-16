echo ">>> Start installing rmdecis lib ..."
mkdir -p build
# echo ">>> Installing dependency with Conan ..."
# sudo conan install . --install-folder=./build --build=missing -s build_type=Release -c tools.system.package_manager:mode=install
echo ">>> Building rmdecis lib ..."
cmake -DCMAKE_BUILD_TYPE=Release -S . -B ./build -G Ninja
cmake --build ./build --target rmdecis --parallel $(($(nproc) - 1))
echo ">>> rmdecis build finished"