set(LIBUBOOTENV_SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/libubootenv)

if(NOT EXISTS ${LIBUBOOTENV_SOURCE_DIR}/CMakeLists.txt)
  execute_process(COMMAND ${GIT_EXECUTABLE}
                          submodule
                          update
                          --init
                          external/libubootenv
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                  RESULT_VARIABLE GIT_CLONE_LIBUBOOTENV_RESULT)
  message(STATUS "GIT_CLONE_LIBUBOOTENV_RESULT: ${GIT_CLONE_LIBUBOOTENV_RESULT}")
endif()

ExternalProject_Add(libubootenv-project
                    SOURCE_DIR ${LIBUBOOTENV_SOURCE_DIR}
                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                    UPDATE_COMMAND ""
                    BUILD_COMMAND ""
                    LIST_SEPARATOR |
                    CMAKE_ARGS 
		        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
		        -DCMAKE_INSTALL_LIBDIR=<INSTALL_DIR>/lib)

ExternalProject_Get_Property(libubootenv-project install_dir)
set(libubootenv_INCLUDE_DIR ${install_dir}/include)
file(MAKE_DIRECTORY ${install_dir}/include)
set(libubootenv_static_LIBRARY ${install_dir}/lib/libubootenv${CMAKE_STATIC_LIBRARY_SUFFIX})
set(libubootenv_shared_LIBRARY ${install_dir}/lib/libubootenv${CMAKE_SHARED_LIBRARY_SUFFIX})

# libubootenv-static
add_library(libubootenv-static STATIC IMPORTED)
add_dependencies(libubootenv-static libubootenv-project)
set_property(TARGET libubootenv-static PROPERTY IMPORTED_LOCATION ${libubootenv_static_LIBRARY})
set_property(TARGET libubootenv-static PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${libubootenv_INCLUDE_DIR})

# libubootenv-shared
add_library(libubootenv-shared SHARED IMPORTED)
add_dependencies(libubootenv-shared libubootenv-project)
set_property(TARGET libubootenv-shared PROPERTY IMPORTED_LOCATION ${libubootenv_shared_LIBRARY})
set_property(TARGET libubootenv-shared PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${libubootenv_INCLUDE_DIR})
