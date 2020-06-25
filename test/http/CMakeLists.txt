# --------------------------------------------------------------------------
# Build test-http
# --------------------------------------------------------------------------
add_executable(test-http main.c)
target_link_libraries(test-http
                      atx-update-test
                      mongoose-mock
		      cmocka)

mongoose_mock_linker_flags(mongoose_link_flags)
# mock_sqlite_linker_flags(sqlite_link_flags)
# mock_utils_linker_flags(utils_flags)
set(wrap_flags "${mongoose_link_flags}")
set_target_properties(test-http PROPERTIES LINK_FLAGS ${wrap_flags})
add_test(NAME test-http COMMAND test-http)
install(TARGETS test-http DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
