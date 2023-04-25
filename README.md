# rmdecis

[![State-of-the-art Shitcode](https://img.shields.io/static/v1?label=State-of-the-art&message=Shitcode&color=7B5804)](https://github.com/trekhleb/state-of-the-art-shitcode) [![CircleCI](https://dl.circleci.com/status-badge/img/gh/CygnoidesV/rmdecis/tree/main.svg?style=shield&circle-token=8d7d1a6746becf36606404fa61bdba53afb9b66d)](https://dl.circleci.com/status-badge/redirect/gh/CygnoidesV/rmdecis/tree/main)

北京邮电大学鸿雁战队 RoboMaster 自瞄预测功能库

> **Warning**  
> 本功能库并未经过测试，请勿用于实际场景。

> 作为一支第一年参加 RMUL、还未能参加 RMUC 的新队伍，由于种种前置原因，非常遗憾我们的自瞄今年没能在赛场实装。虽然这套功能库既不完善，也没有技术创新，甚至可能实际上场时效果不符合预期，但我们任然决定把这套方案开源 —— 为了能够方便地与各位交流，更为了倒逼自己把代码写的规范。  
> @catslashbin, 2023.04.25

## Features

输入视觉识别部分的输出的装甲板角点，解算云台目标角度。  

功能：
- 单目相机 PnP 解算
- 相机、云台、世界坐标系变换
- SORT 跟踪器跟踪装甲板
- 卡尔曼滤波预测
- 弹道解算补偿

特点：
- Modern C++ 封装，简单易用
- 不依赖 ROS 运行，支持跨平台开发

## Installation

### Ubuntu

1. 配置 C++ 环境

    ```bash
   ### Only tested on ubuntu 2004
   sudo apt install gcc build-essential make cmake libspdlog-dev libceres-dev libyaml-cpp-dev libeigen3-dev libopencv-dev libfmt-dev ninja-build
   ```

2. 安装 kissfft

   - Ubuntu 20.04 需要手动编译 kissfft

      ```
      git clone https://github.com/mborgerding/kissfft.git
      cd kissfft
      mkdir build && cd build
      cmake -DKISSFFT_STATIC=1 -DKISSFFT_TEST=OFF ..
      make all
      sudo make install
      ```

   - Ubuntu 22.04 可使用 apt 安装

      ```
      sudo install libkissfft-dev
      ```

3. 克隆此仓库
   ```
   git clone https://github.com/CygnoidesV/rmdecis.git
   cd rmdecis
   ```

4. 运行 `build.sh`
   ```bash
   bash build.bash
   ```

5. （可选）安装库
   ```bash
   sudo bash install.bash
   ```

### Windows

TODO

## Usage

> **Note**  
> 示例请参考 `./example/`

1. 安装本项目

2. 配置 `CMakeLists.txt`

   ```
   #### Link rmdecis
   find_package(PkgConfig REQUIRED)
   pkg_check_modules(RMDECIS REQUIRED rmdecis)
   link_libraries(${RMDECIS_LIBRARIES})
   include_directories(${RMDECIS_INCLUDE_DIRS})
   ```

3. 编写配置文件（参考 `./example/config/config.yml`）

4. 调用 `rmdecis`
   ```C++
   #include <rmdecis/basic_aiming.h>
   #include <iostream>

   using namespace cv;

   ArmorFrameInput getDetectionResult() {
      /* Construct DetectArmorInfo */
      auto corners = RectCorners2f({100, 200}, {100, 300}, {200, 200}, {200, 300});
      auto armor = DetectArmorInfo(STANDARD_3, corners, 1.0);

      /* Construct DetectArmorsFrame */
      int seq_idx = 0; // Frame seq index
      Time time = {43453, 122}; // Frame capture time
      std::vector<DetectArmorInfo> armor_info = {armor}; // Detected armor list

      /* Construct RobotState */
      auto robot_state = RobotState(0.f, 0.f, 15000, true);

      return {seq_idx, time, robot_state, armor_info};
   }

   int main() {
      /* Load config */
      Config cfg("../config/config.yml");
      BasicAiming basic_aiming(cfg);

      /* Predict */
      EulerAngles angles = basic_aiming.update(getDetectionResult());

      /* Print result (should be "nan nan") */
      std::cout << angles.pitch << " " << angles.yaw << std::endl;
   }
   ```

## Project Structure

TODO

### File Structure

```
.
├── 3rdparty                        # 第三方库
├── build.bash                      # 安装脚本
├── CMakeLists.txt   
├── example                         # 库使用示例
│   ├── CMakeLists.txt  
│   ├── config                      # 配置文件
│   │   ├── camera_coeffs.yml       # 相机标定参数
│   │   └── config.yml              # 配置文件示例
│   └── test_rmdecis.cpp   
├── include                         # 库公开接口
├── install.bash                    # 安装脚本    
├── README.md           
├── rmdecis.pc.in
├── src           
│   ├── compensator                 # 补偿器：弹道补偿
│   ├── predictor                   # 预测器：三角函数拟合
│   ├── reconstructor               # 重建器：PnP 解算、坐标系变换
│   ├── rmdecis                     # pimpl 接口
│   ├── rmdecis_impl                # pimpl 实现
│   ├── tracker                     # 跟踪器：SORT实现、卡尔曼滤波
│   ├── typing                      # 通用类型实现
│   └── utils                       # 工具
├── tests                           # 测试样例
├── uninstall.sh                    # 卸载脚本
└── utils_contrib                   # 测试工具
```

## Development

TODO

### Code Style

项目主体参考 [Google Style Guide: C++](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/)，有如下更改：


1. 函数名采用 camelCase
2. 常变量名采用 k_snake_case

## Acknowledgement

感谢以下项目：

- [chenjunnn/rm_auto_aim](https://github.com/chenjunnn/rm_auto_aim)
- [robomaster-oss/rmoss_core](https://github.com/robomaster-oss/rmoss_core)
- [Birdiebot/bubble](https://github.com/Birdiebot/bubble)
- [Harry-hhj/CVRM2021-sjtu](https://github.com/Harry-hhj/CVRM2021-sjtu)
- [munkres-cpp](https://github.com/saebyn/munkres-cpp)

