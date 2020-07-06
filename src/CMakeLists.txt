set(SOURCES
  atxupdate.c
  http.c
  http_route_env.c
  sys.c)
set(HEADERS
  http.h
  http_route_env.h
  sys.h
  env.h
  klist.h
  khash.h
  containers.h)
set(HEADERS_PUBLIC ../include/altronix/atxupdate.h log.h)

set(LIBS mongoose-shared m z)
list(APPEND definitions "-DMG_ENABLE_HTTP_STREAMING_MULTIPART")
list(APPEND definitions "-DMG_ENABLE_WEBSOCKET")
list(APPEND definitions "-DMG_ENABLE_CALLBACK_USERDATA")
append_log_level_compiler_flags(definitions)

if(${BUILD_EMU})
  list(APPEND SOURCES env_emu.c)
  list(APPEND definitions "-DEMU=TRUE")
else()
  list(APPEND SOURCES env_uboot.c)
  list(APPEND LIBS ubootenv-shared)
  list(APPEND definitions "-DEMU=FALSE")
endif()

add_library(atx-update-test STATIC ${SOURCES} ${HEADERS})
target_link_libraries(atx-update-test ${LIBS})
target_include_directories(atx-update-test PUBLIC ./)
target_include_directories(atx-update-test PUBLIC ../include)
target_compile_definitions(atx-update-test PUBLIC ${definitions})

add_library(atx-update-static STATIC ${SOURCES} ${HEADERS})
target_link_libraries(atx-update-static ${LIBS})
target_include_directories(atx-update-static PUBLIC ../include)
target_compile_definitions(atx-update-static PUBLIC ${definitions})
set_target_properties(atx-update-static PROPERTIES
  PUBLIC_HEADER "${HEADERS_PUBLIC}"
  OUTPUT_NAME "atx-update")
install(TARGETS atx-update-static
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include/altronix)
