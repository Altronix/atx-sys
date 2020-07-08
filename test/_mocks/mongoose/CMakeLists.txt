# --------------------------------------------------------------------------
# Lib of mock functions for test
# --------------------------------------------------------------------------
add_library(mongoose-mock 
  mongoose_mock.c
  parse_http.c
  parse_http.h)

target_link_libraries(mongoose-mock
  atx-update-test
  http-parser
  mongoose-shared
  stdc++)

target_include_directories(mongoose-mock PUBLIC .)

function(mongoose_mock_linker_flags dst)
  string(CONCAT wrap_flags "-Wl,")
  string(CONCAT wrap_flags ${wrap_flags} "--wrap=mg_mgr_init,")
  string(CONCAT wrap_flags ${wrap_flags} "--wrap=mg_mgr_free,")
  string(CONCAT wrap_flags ${wrap_flags} "--wrap=mg_set_protocol_http_websocket,")
  string(CONCAT wrap_flags ${wrap_flags} "--wrap=mg_mgr_poll,")
  string(CONCAT wrap_flags ${wrap_flags} "--wrap=mg_printf,")
  string(CONCAT wrap_flags ${wrap_flags} "--wrap=mg_send_websocket_frame,")
  string(CONCAT wrap_flags ${wrap_flags} "--wrap=mg_vprintf,")
  string(CONCAT wrap_flags ${wrap_flags} "--wrap=mg_printf_http_chunk,")
  string(CONCAT wrap_flags ${wrap_flags} "--wrap=mg_printf_html_escape,")
  string(CONCAT wrap_flags ${wrap_flags} "--wrap=mg_bind")
  set(${dst} ${wrap_flags} PARENT_SCOPE)
endfunction()

mongoose_mock_linker_flags(mongoose_flags)
# mock_utils_linker_flags(utils_flags)
set(wrap_flags "${mongoose_flags} ${utils_flags}")

add_executable(test-mongoose-mock main.c)
set_target_properties(test-mongoose-mock PROPERTIES LINK_FLAGS ${wrap_flags})
target_link_libraries(test-mongoose-mock
                      mongoose-mock
                      cmocka)
add_test(NAME test-mongoose-mock COMMAND test-mongoose-mock)
install(TARGETS test-mongoose-mock DESTINATION bin)

