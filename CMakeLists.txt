cmake_minimum_required(VERSION 3.13...3.16)
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
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# Note buildroot sets BUILD_SHARED_LIBS
option(BUILD_SHARED "Whether or not to build the shared object" ON)
option(BUILD_STATIC "Whether or not to build the static archive" ON)
option(BUILD_DEPENDENCIES "Wether or not to build dependencies" ON)

# Target environment
set(ATX_UPDATE_ENV "UBOOT" CACHE STRING "UBOOT or EMU environment support")

# Dependencies
FILE(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include)
include(cmake/import_functions.cmake)
if(BUILD_DEPENDENCIES)
  include(cmake/mongoose.cmake)
  include(cmake/libubootenv.cmake)
  import_library_shared(mongoose mongoose-shared FALSE)
  import_library_static(ubootenv ubootenv-static FALSE)
else()
  find_library(MONGOOSE_LIBRARY_PATH mongoose)
  find_library(UBOOTENV_LIBRARY_PATH ubootenv)
  import_library_shared(mongoose mongoose-shared ${MONGOOSE_LIBRARY_PATH})
  import_library_static(ubootenv ubootenv-static ${UBOOTENV_LIBRARY_PATH})
endif()

# Test?
if(ENABLE_TESTING)
  set(CMAKE_BUILD_TYPE Debug)
  enable_testing()
  include(CTest)
  include(cmake/cmocka.cmake)
  add_subdirectory(test/mocks/libubootenv)
  add_subdirectory(test/env)
endif()

# Modules
add_subdirectory(src)

# Apps
