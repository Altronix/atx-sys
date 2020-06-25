function(import_library_shared lib id path)
  set(VAR_LIB ${CMAKE_SHARED_LIBRARY_PREFIX}${lib}${CMAKE_SHARED_LIBRARY_SUFFIX})
  if(${path})
    set(VAR_LOC "${path}")
  else()
    set(VAR_LOC "${CMAKE_INSTALL_PREFIX}/lib/${VAR_LIB}")
  endif()
  add_library(${id} SHARED IMPORTED)
  set_target_properties(${id} PROPERTIES
	  IMPORTED_LOCATION ${VAR_LOC}
	  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/include")
endfunction()

function(import_library_static lib id path)
  set(VAR_LIB ${CMAKE_STATIC_LIBRARY_PREFIX}${lib}${CMAKE_STATIC_LIBRARY_SUFFIX})
  if(${path})
    set(VAR_LOC "${path}")
  else()
    set(VAR_LOC "${CMAKE_INSTALL_PREFIX}/lib/${VAR_LIB}")
  endif()
  add_library(${id} STATIC IMPORTED)
  set_target_properties(${id} PROPERTIES
	  IMPORTED_LOCATION ${VAR_LOC}
	  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/include")
endfunction()
