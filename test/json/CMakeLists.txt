add_executable(test-json main.c)
target_link_libraries(test-json atxsys-test cmocka)
add_test(NAME test-json COMMAND test-json)
install(TARGETS test-json DESTINATION bin)
