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
		        -DCMAKE_INSTALL_LIBDIR=<INSTALL_DIR>/lib
                        -DBUILD_DOC:BOOL=FALSE)
