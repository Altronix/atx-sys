# --------------------------------------------------------------------------
# Lib of mock functions for test
# --------------------------------------------------------------------------
add_library(ubootenv-mock libubootenv.c libubootenv.h)
target_link_libraries(ubootenv-mock ubootenv-static)
target_include_directories(ubootenv-mock PUBLIC .)

function(ubootenv_mock_linker_flags dst)
  string(CONCAT wrap "-Wl,")
  string(CONCAT wrap ${wrap} "--wrap=libuboot_iterator,")
  string(CONCAT wrap ${wrap} "--wrap=libuboot_getname,")
  string(CONCAT wrap ${wrap} "--wrap=libuboot_getvalue")
  set(${dst} ${wrap} PARENT_SCOPE)
endfunction()
