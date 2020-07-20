add_executable(atxsys main.c)
target_link_libraries(atxsys atxsys-static)
install(TARGETS atxsys RUNTIME DESTINATION bin)
