function(import_library_shared ARG_LIB ARG_ID ARG_PATH)
  set(VAR_INC "${CMAKE_INSTALL_PREFIX}/include")
  if(ARG_PATH)
    get_filename_component(LIB_DIR ${ARG_PATH} DIRECTORY)
    list(APPEND VAR_INC "${LIB_DIR}/../include")
    set(VAR_LOC "${ARG_PATH}")
  else()
    set(VAR_LIB "${CMAKE_SHARED_LIBRARY_PREFIX}${ARG_LIB}${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(VAR_LOC "${CMAKE_INSTALL_PREFIX}/lib/${VAR_LIB}")
  endif()
  add_library(${ARG_ID} SHARED IMPORTED)
  set_target_properties(${ARG_ID} PROPERTIES
	  IMPORTED_LOCATION ${VAR_LOC}
	  INTERFACE_INCLUDE_DIRECTORIES "${VAR_INC}")
endfunction()

function(import_library_static ARG_LIB ARG_ID ARG_PATH)
  set(VAR_INC "${CMAKE_INSTALL_PREFIX}/include")
  if(ARG_PATH)
    get_filename_component(LIB_DIR ${ARG_PATH} DIRECTORY)
    list(APPEND VAR_INC "${LIB_DIR}/../include")
    set(VAR_LOC "${ARG_PATH}")
  else()
    set(VAR_LIB "${CMAKE_STATIC_LIBRARY_PREFIX}${ARG_LIB}${CMAKE_STATIC_LIBRARY_SUFFIX}")
    set(VAR_LOC "${CMAKE_INSTALL_PREFIX}/lib/${VAR_LIB}")
  endif()
  add_library(${ARG_ID} STATIC IMPORTED)
  set_target_properties(${ARG_ID} PROPERTIES
	  IMPORTED_LOCATION ${VAR_LOC}
	  INTERFACE_INCLUDE_DIRECTORIES "${VAR_INC}")
endfunction()
