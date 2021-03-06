add_library(mock-file 
  mock_file.c 
  mock_file.h 
  klib/khash.h 
  klib/klist.h 
  containers.h)
target_link_libraries(mock-file cmocka stdc++)
target_include_directories(mock-file PUBLIC .)

function (append_mock_file_linker_flags dst)
  string(CONCAT wrap_flags "-Wl,")
  string(CONCAT wrap_flags ${wrap_flags} "--wrap=fopen,")
  string(CONCAT wrap_flags ${wrap_flags} "--wrap=fclose,")
  string(CONCAT wrap_flags ${wrap_flags} "--wrap=fread,")
  string(CONCAT wrap_flags ${wrap_flags} "--wrap=fwrite,")
  string(CONCAT wrap_flags ${wrap_flags} "--wrap=vfprintf,")
  string(CONCAT wrap_flags ${wrap_flags} "--wrap=fprintf,")
  string(CONCAT wrap_flags ${wrap_flags} "--wrap=ioctl,")
  string(CONCAT wrap_flags ${wrap_flags} "--wrap=fileno")
  set(${dst} ${wrap_flags} PARENT_SCOPE)
endfunction()
append_mock_file_linker_flags(wrap_flags)

add_executable(test-mock-file main.c)
set_target_properties(test-mock-file PROPERTIES LINK_FLAGS ${wrap_flags})
target_link_libraries(test-mock-file mock-file cmocka)
add_test(NAME test-mock-file COMMAND test-mock-file)
install(TARGETS test-mock-file DESTINATION bin)
