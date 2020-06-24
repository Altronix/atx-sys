# --------------------------------------------------------------------------
# Lib of mock functions for test
# --------------------------------------------------------------------------
add_library(mock-libubootenv libubootenv.c libubootenv.h)
target_link_libraries(mock-libubootenv libubootenv-static)
target_include_directories(mock-libubootenv PUBLIC .)

function(mock_libubootenv_linker_flags dst)
  string(CONCAT wrap "-Wl,")
  string(CONCAT wrap ${wrap} "--wrap=libuboot_iterator,")
  string(CONCAT wrap ${wrap} "--wrap=libuboot_getname,")
  string(CONCAT wrap ${wrap} "--wrap=libuboot_getvalue")
  set(${dst} ${wrap} PARENT_SCOPE)
endfunction()
