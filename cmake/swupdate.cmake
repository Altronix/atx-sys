set(SWUPDATE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/swupdate)

if(NOT EXISTS ${SWUPDATE_SOURCE_DIR}/Makefile)
  execute_process(COMMAND ${GIT_EXECUTABLE}
                          submodule
                          update
                          --init
                          external/swupdate
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		  RESULT_VARIABLE GIT_CLONE_SWUPDATE_RESULT)
	  message(STATUS "GIT_CLONE_SWUPDATE_RESULT: ${GIT_CLONE_SWUPDATE_RESULT}")
endif()

ExternalProject_Add(swupdate-project
	            SOURCE_DIR ${SWUPDATE_SOURCE_DIR}
                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                    CONFIGURE_COMMAND COMMAND make allnoconfig
                    UPDATE_COMMAND ""
                    BUILD_COMMAND COMMAND make 
                      DESTDIR=${CMAKE_INSTALL_PREFIX}
                      BINDIR=bin
                      LIBDIR=lib
                      INCLUDEDIR=include
                      CONFIG_EXTRA_CFLAGS=-I${CMAKE_INSTALL_PREFIX}/include
                      CONFIG_EXTRA_LDFLAGS=-L${CMAKE_INSTALL_PREFIX}/lib
                    INSTALL_COMMAND COMMAND make install
                      DESTDIR=${CMAKE_INSTALL_PREFIX}
                      BINDIR=bin
                      LIBDIR=lib
                      INCLUDEDIR=include
                      CONFIG_EXTRA_CFLAGS=-I${CMAKE_INSTALL_PREFIX}/include
                      CONFIG_EXTRA_LDFLAGS=-L${CMAKE_INSTALL_PREFIX}/lib
                    BUILD_IN_SOURCE TRUE)
add_dependencies(swupdate-project libubootenv-project)
