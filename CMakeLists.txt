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
set(LOG_LEVEL "DEBUG" CACHE STRING "TRACE DEBUG INFO WARN ERROR FATAL or NONE")
if(${LOG_LEVEL} STREQUAL "TRACE")
  set(LOG_LEVEL_INT 6)
elseif(${LOG_LEVEL} STREQUAL "DEBUG")
  set(LOG_LEVEL_INT 5)
elseif(${LOG_LEVEL} STREQUAL "INFO")
  set(LOG_LEVEL_INT 4)
elseif(${LOG_LEVEL} STREQUAL "WARN")
  set(LOG_LEVEL_INT 3)
elseif(${LOG_LEVEL} STREQUAL "ERROR")
  set(LOG_LEVEL_INT 2)
elseif(${LOG_LEVEL} STREQUAL "FATAL")
  set(LOG_LEVEL_INT 1)
else()
  set(LOG_LEVEL_INT 0)
endif()
function(append_log_level_compiler_flags dst)
  MESSAGE(STATUS "LOG_LEVEL set ${LOG_LEVEL}")
  list(APPEND definitions "-DLOG_LEVEL=${LOG_LEVEL_INT}")
  set(${dst} ${definitions} PARENT_SCOPE)
endfunction()

# Note buildroot sets BUILD_SHARED_LIBS
option(BUILD_SHARED "Whether or not to build the shared object" ON)
option(BUILD_STATIC "Whether or not to build the static archive" ON)
option(BUILD_DEPENDENCIES "Wether or not to build dependencies" ON)

# Target environment
set(ATX_UPDATE_ENV "UBOOT" CACHE STRING "UBOOT or EMU environment support")
if(${ATX_UPDATE_ENV} STREQUAL "UBOOT")
  set(ATX_UPDATE_ENV_INT 0)
elseif(${ATX_UPDATE_ENV} STREQUAL "EMU")
  set(ATX_UPDATE_ENV_INT 1)
else()
endif()

# Dependencies
FILE(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include)
include(cmake/import_functions.cmake)
if(BUILD_DEPENDENCIES)
  include(cmake/mongoose.cmake)
  include(cmake/ubootenv.cmake)
  import_library_static(ubootenv ubootenv-shared FALSE)
else()
  find_library(MONGOOSE_LIBRARY_PATH mongoose)
  find_library(UBOOTENV_LIBRARY_PATH ubootenv)
  import_library_shared(mongoose mongoose-shared ${MONGOOSE_LIBRARY_PATH})
  import_library_shared(ubootenv ubootenv-shared ${UBOOTENV_LIBRARY_PATH})
  message("mongoose: ${MONGOOSE_LIBRARY_PATH}")
  message("ubootenv: ${UBOOTENV_LIBRARY_PATH}")
endif()

# Test?
if(ENABLE_TESTING)
  set(CMAKE_BUILD_TYPE Debug)
  enable_testing()
  include(CTest)
  include(cmake/cmocka.cmake)
  include(cmake/http-parser.cmake)
  add_subdirectory(test/_mocks/ubootenv)
  add_subdirectory(test/_mocks/mongoose)
  add_subdirectory(test/env)
  add_subdirectory(test/http)
  add_subdirectory(test/route_env)
endif()
set(CMAKE_BUILD_TYPE Debug)

# Modules
add_subdirectory(src)

# Apps
add_subdirectory(app)
