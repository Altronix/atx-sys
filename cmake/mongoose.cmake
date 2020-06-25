set(MONGOOSE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/mongoose)
set(MONGOOSE_TEST_FILE ${MONGOOSE_SOURCE_DIR}/CMakeLists.txt)

ExternalProject_Add(mongoose-project
  SOURCE_DIR ${MONGOOSE_SOURCE_DIR}
  INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND COMMAND 
		  ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS} -fpic -c ${MONGOOSE_SOURCE_DIR}/mongoose.c 
		  -DMG_ENABLE_HTTP_STREAMING_MULTIPART
		  -DMG_ENABLE_WEBSOCKET
		  -DMG_ENABLE_CALLBACK_USERDATA
		COMMAND 
		  ${CMAKE_COMMAND} -E copy ${MONGOOSE_SOURCE_DIR}/mongoose.h ${CMAKE_INSTALL_PREFIX}/include/mongoose.h
  INSTALL_COMMAND ${CMAKE_C_COMPILER} -shared -o ${CMAKE_INSTALL_PREFIX}/lib/libmongoose.so mongoose.o)

function(append_mongoose_compiler_flags dst)
  list(APPEND definitions "-DMG_ENABLE_HTTP_STREAMING_MULTIPART")
  list(APPEND definitions "-DMG_ENABLE_WEBSOCKET")
  list(APPEND definitions "-DMG_ENABLE_CALLBACK_USERDATA")
  set(${dst} ${definitions} PARENT_SCOPE)
endfunction()
