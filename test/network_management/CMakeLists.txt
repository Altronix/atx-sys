add_executable(test-network-management-env main.c)
target_link_libraries(test-network-management-env
  atx-update-test
  cmocka)
add_test(NAME test-network-management-env COMMAND test-network-management-env)
install(TARGETS test-network-management-env DESTINATION bin)

