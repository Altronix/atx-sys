add_executable(test-route-network main.c)
target_link_libraries(test-route-network
  atxsys-test
  mongoose-mock
  cmocka)
mongoose_mock_linker_flags(mongoose_link_flags)
set(wrap_flags "${mongoose_link_flags}")
set_target_properties(test-route-network PROPERTIES LINK_FLAGS ${wrap_flags})
add_test(NAME test-route-network COMMAND test-route-network)
install(TARGETS test-route-network DESTINATION bin)
