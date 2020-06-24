set(SOURCES env.c)
set(HEADERS env.h)

if(${ATX_UPDATE_ENV} STREQUAL "EMU")
  list(APPEND SOURCES env_emu.c)
elseif(${ATX_UPDATE_ENV} STREQUAL "UBOOT")
  list(APPEND SOURCES env_uboot.c)
  list(APPEND LIBS_STATIC libubootenv-static)
  list(APPEND LIBS_SHARED libubootenv-shared)
else()
  message(FATAL_ERROR "Please select bootloader environment!")
endif()

add_library(atx-update-static STATIC ${SOURCES} ${HEADERS})
target_link_libraries(atx-update-static ${LIBS_STATIC})
set_target_properties(atx-update-static PROPERTIES
  PUBLIC_HEADER "${HEADERS}"
  OUTPUT_NAME "atx-update")
install(TARGETS atx-update-static
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include/altronix)
