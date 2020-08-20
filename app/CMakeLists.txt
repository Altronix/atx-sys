add_executable(atx-sys main.c)
target_link_libraries(atx-sys atxsys-static)
install(TARGETS atx-sys RUNTIME DESTINATION bin)
