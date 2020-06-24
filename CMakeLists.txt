cmake_minimum_required(VERSION 3.9.3...3.16)
project(atx-update)

find_package(Git)
if(NOT Git_FOUND)
  message(FATAL_ERROR "Git not found! Git is required to install submodules")
endif()

# dependency install
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
find_package(Threads)

# Compiler stuff
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE on)

# Note buildroot sets BUILD_SHARED_LIBS
option(BUILD_SHARED "Whether or not to build the shared object"  ON)
option(BUILD_STATIC "Whether or not to build the static archive" ON)

# Target environment
set(ATX_UPDATE_ENV "EMU" CACHE STRING "UBOOT or EMU environment support")

# Dependencies
if(${ATX_UPDATE_ENV} STREQUAL "UBOOT")
  include(cmake/libubootenv.cmake)
endif()

# Test?
if(ENABLE_TESTING)
endif()

# Modules

# Apps
