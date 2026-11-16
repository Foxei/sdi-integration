# Cross-compile for macOS from Linux with clang + lld.
# Pass -DCMAKE_OSX_ARCHITECTURES=<arm64|x86_64> (one architecture per build).
set(CMAKE_SYSTEM_NAME Darwin)
set(CMAKE_SYSTEM_VERSION 22.0) # Darwin 22 = macOS 13

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER_TARGET ${CMAKE_OSX_ARCHITECTURES}-apple-macos13.0)
set(CMAKE_CXX_COMPILER_TARGET ${CMAKE_OSX_ARCHITECTURES}-apple-macos13.0)
set(CMAKE_EXE_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_AR llvm-ar CACHE FILEPATH "")
set(CMAKE_RANLIB llvm-ranlib CACHE FILEPATH "")
set(CMAKE_INSTALL_NAME_TOOL llvm-install-name-tool CACHE FILEPATH "")
# llvm-ranlib does not understand Apple's ranlib flags
set(CMAKE_C_ARCHIVE_FINISH "<CMAKE_RANLIB> <TARGET>")
set(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> <TARGET>")

set(CMAKE_OSX_SYSROOT /opt/macos-sdk)
set(CMAKE_OSX_DEPLOYMENT_TARGET 13.0)

set(CMAKE_FIND_ROOT_PATH ${QT_TARGET_PATH} ${CMAKE_OSX_SYSROOT})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Qt would otherwise call xcrun/xcodebuild, which do not exist on Linux
set(QT_NO_APPLE_SDK_AND_XCODE_CHECK ON)
set(QT_NO_APPLE_SDK_MAX_VERSION_CHECK ON)

# The host rcc supports zstd, the target QtCore may not
set(CMAKE_AUTORCC_OPTIONS --no-zstd)
