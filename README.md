# rmdecis

## Build & install

1. 配置 C++ 环境

    ```bash
   ### Only tested on ubuntu 2004
   ### Build requirements:
   sudo apt install gcc build-essential make cmake libspdlog-dev libceres-dev libyaml-cpp-dev libeigen3-dev libopencv-dev libfmt-dev ninja-build
   ```

2. 安装 kissfft
   ```
   mkdir -p ~/Projects
   cd ~/Projects
   git clone https://github.com/mborgerding/kissfft.git
   cd kissfft
   mkdir build && cd build
   cmake -DKISSFFT_STATIC=1 -DKISSFFT_TEST=OFF ..
   make all
   sudo make install
   ```

3. 克隆此仓库
   ```
   mkdir -p ~/Projects
   cd ~/Projects
   git clone https://github.com/CygnoidesV/Cygnoides-Decision.git
   cd Cygnoides-Decision
   git checkout [在这里填入版本]
   ```

2. 运行 `build.sh`
   ```bash
   sh build.bash
   ```

3. （可选）安装库
   ```bash
   sudo bash install.bash
   ```

## File Structure

TODO

## Code Style

- 类名采用 PascalCase
- 函数名采用 camelCase
- 变量名采用 snake_case
- 其他参考 [Google Style Guide](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/)
