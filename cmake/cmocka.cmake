# cmocka

set(CMOCKA_SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/cmocka)

if(NOT EXISTS ${CMOCKA_SOURCE_DIR}/CMakeLists.txt)
	execute_process(
		COMMAND ${GIT_EXECUTABLE} submodule update --init external/cmocka
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		RESULT_VARIABLE GIT_CLONE_CMOCKA_RESULT)
	message(STATUS "GIT_CLONE_CMOCKA_RESULT: ${GIT_CLONE_CMOCKA_RESULT}")
endif()

ExternalProject_Add(cmocka-project
	SOURCE_DIR ${CMOCKA_SOURCE_DIR}
	INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
	UPDATE_COMMAND ""
	LIST_SEPARATOR |
	CMAKE_ARGS 
		-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
		-DCMAKE_INSTALL_LIBDIR=<INSTALL_DIR>/lib
	)

ExternalProject_Get_Property(cmocka-project install_dir)
set(cmocka_LIBRARY ${install_dir}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}cmocka${CMAKE_SHARED_LIBRARY_SUFFIX})
set(cmocka_INCLUDE_DIR ${install_dir}/include)

add_library(cmocka SHARED IMPORTED)
set_property(TARGET cmocka PROPERTY IMPORTED_LOCATION ${cmocka_LIBRARY})
set_property(TARGET cmocka PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${cmocka_INCLUDE_DIR})
add_dependencies(cmocka cmocka-project)
