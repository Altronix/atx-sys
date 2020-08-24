set(SOURCES
  atxsys.c
  http.c
  http_route_about.c
  http_route_env.c
  http_route_network.c
  http_route_kill.c
  http_route_update.c
  json.c
  network_management.c
  sys.c)
set(HEADERS
  http.h
  http_route_about.h
  http_route_env.h
  http_route_network.h
  http_route_kill.h
  http_route_update.h
  jsmn.h
  json.h
  network_management.h
  sys.h
  env.h
  klist.h
  khash.h
  containers.h)
set(HEADERS_PUBLIC ../include/altronix/atxsys.h log.h)

set(LIBS mongoose-shared ${CMAKE_THREAD_LIBS_INIT} m z)
list(APPEND definitions "-DMG_ENABLE_HTTP_STREAMING_MULTIPART")
list(APPEND definitions "-DMG_ENABLE_WEBSOCKET")
list(APPEND definitions "-DMG_ENABLE_CALLBACK_USERDATA")
append_log_level_compiler_flags(definitions)

set(CONFIG_FILE "" CACHE STRING "User configuration file path")
if(NOT "${CONFIG_FILE}" STREQUAL "")
  list(APPEND definitions "-DATX_SYS_CONFIG_FILE=\"${CONFIG_FILE}\"")
endif()

if(${BUILD_EMU})
  list(APPEND SOURCES env_emu.c)
  list(APPEND SOURCES updater_emu.c)
  list(APPEND definitions "-DEMU=TRUE")
else()
  list(APPEND SOURCES env_uboot.c)
  list(APPEND SOURCES updater_swupdate.c)
  list(APPEND LIBS ubootenv-shared)
  list(APPEND LIBS swupdate-static)
  list(APPEND definitions "-DEMU=FALSE")
endif()

add_library(atxsys-test STATIC ${SOURCES} ${HEADERS})
target_link_libraries(atxsys-test ${LIBS})
target_include_directories(atxsys-test PUBLIC ./)
target_include_directories(atxsys-test PUBLIC ../include)
target_compile_definitions(atxsys-test PUBLIC ${definitions})

add_library(atxsys-static STATIC ${SOURCES} ${HEADERS})
target_link_libraries(atxsys-static ${LIBS})
target_include_directories(atxsys-static PUBLIC ../include)
target_compile_definitions(atxsys-static PUBLIC ${definitions})
set_target_properties(atxsys-static PROPERTIES
  PUBLIC_HEADER "${HEADERS_PUBLIC}"
  OUTPUT_NAME "atxsys")
install(TARGETS atxsys-static
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include/altronix)
