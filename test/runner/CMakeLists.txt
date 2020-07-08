file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)
add_custom_command(OUTPUT ${CMAKE_INSTALL_PREFIX}/bin/runner.sh
  COMMAND ${CMAKE_COMMAND} -E
      copy ${CMAKE_SOURCE_DIR}/test/runner/runner.sh ${CMAKE_INSTALL_PREFIX}/bin/)
add_custom_target(test-runner ALL DEPENDS ${CMAKE_INSTALL_PREFIX}/bin/runner.sh)
