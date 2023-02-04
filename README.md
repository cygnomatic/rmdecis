# Cygnoides-Decision

北邮鸿雁战队 视觉组 - 决策部分 代码仓库

## Setup & Installation

1. 配置 C++ 环境

    ```bash
   ### Runtime requirements:
   sudo apt install gcc python3 build-essential make cmake
   
   ### Debug requirements:
   # sudo apt install dbg clang-tidy
   ```

2. 安装 C++ 包管理器 Conan
    
    ```bash
   pip3 install conan
   ```

3. 配置 Conan
   ```bash
   # Check gcc version.
   $ gcc -v
   Using built-in specs.
   COLLECT_GCC=gcc
   ...
   gcc version 12.1.0 (Ubuntu 12.1.0-2ubuntu1~22.04)
   
   # Set conan profile.
   $ conan profile update settings.compiler.version=12 default
   $ conan profile update settings.compiler.libcxx=libstdc++11 default
   ```

4. 运行 `install.sh`
   ```bash
   $ sudo sh install.sh
   ```

## File Structure
```
.
├── config # 配置文件
├── data # 测试用数据
├── src 
│   ├── compensator # 补偿器
│   ├── contrib # 第三方库
│   ├── reconstructor # 重建器
│   ├── tracker # 跟踪器
│   ├── typing # 类型定义
│   └── utils # 工具
├── tests # 测试代码
└── utils_contrib # 测试工具

```
## Code Style

- 类名采用 PascalCase
- 函数名采用 camelCase
- 变量名采用 snake_case
- 其他参考 [Google Style Guide](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/)
