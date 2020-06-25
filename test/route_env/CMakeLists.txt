add_executable(test-route-env main.c)
target_link_libraries(test-route-env
  atx-update-test
  mongoose-mock
  cmocka)
mongoose_mock_linker_flags(mongoose_link_flags)
set(wrap_flags "${mongoose_link_flags}")
set_target_properties(test-route-env PROPERTIES LINK_FLAGS ${wrap_flags})
add_test(NAME test-route-env COMMAND test-route-env)
install(TARGETS test-route-env DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
