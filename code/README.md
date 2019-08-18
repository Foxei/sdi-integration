# Installation

The following requirements must be met before compiling: 
- Qt 5.8+
- CMake 3.12+
- C++ Compiler with Cxx11 support

 The software is not bound to the operating system and runs on all common platforms.
 
## Compilation

The standard cmake building process is used to build the source code.
 
```bash
# Creating a build folder
mkdir build
cd build
# Running CMake
cmake ..
cmake --build . -j <number_of_cores>
# Installation (optional)
cmake --build . --target install
```