cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(IntegrationGui VERSION 1.0 LANGUAGES CXX)

# Set CMake flag to work with Qt
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt 6 requires C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)

# Finding Qt
find_package(Qt6 6.8 REQUIRED COMPONENTS Core Widgets Charts Gui Svg)
message(STATUS "Qt6 version: ${Qt6_VERSION} " )

# Set resources for the application
set(QT_RES ${CMAKE_CURRENT_SOURCE_DIR}/res/res.qrc)

# Targets that we develop here
add_subdirectory(src)