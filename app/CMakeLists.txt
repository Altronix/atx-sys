add_executable(atx-update main.c)
target_link_libraries(atx-update atx-update-static mongoose-shared ubootenv-shared)
install(TARGETS atx-update RUNTIME DESTINATION bin)
