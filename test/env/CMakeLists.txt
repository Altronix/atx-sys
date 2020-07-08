add_executable(test-env main.c)
target_link_libraries(test-env 
  atx-update-test 
  ubootenv-shared
  ubootenv-mock
  cmocka)
ubootenv_mock_linker_flags(ubootenv_link_flags)
set(wrap_flags "${ubootenv_link_flags}")
set_target_properties(test-env PROPERTIES LINK_FLAGS ${wrap_flags})
add_test(NAME test-env COMMAND test-env)
install(TARGETS test-env DESTINATION bin)
