cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(IntegrationGui VERSION 1.0 LANGUAGES CXX)

# Set CMake flag to work with Qt
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Enable C++14
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 11)

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)

# Finding Qt
find_package(Qt5 5.10 REQUIRED COMPONENTS Core Widgets Charts Gui)
message(STATUS "Qt5Widgets version: ${Qt5Widgets_VERSION} " )
message(STATUS "Qt5Core version: ${Qt5Core_VERSION} " )

# Set resources for the application
set(QT_RES ${CMAKE_CURRENT_SOURCE_DIR}/res/res.qrc)

# Targets that we develop here
add_subdirectory(src)