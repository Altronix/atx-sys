set(MONGOOSE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/mongoose)
set(MONGOOSE_TEST_FILE ${MONGOOSE_SOURCE_DIR}/CMakeLists.txt)

add_library(mongoose-shared SHARED 
  ${MONGOOSE_SOURCE_DIR}/mongoose.c 
  ${MONGOOSE_SOURCE_DIR}/mongoose.h)
target_include_directories(mongoose-shared PUBLIC ${MONGOOSE_SOURCE_DIR})
install(TARGETS mongoose-shared
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include)

function(append_mongoose_compiler_flags dst)
  list(APPEND definitions "-DMG_ENABLE_HTTP_STREAMING_MULTIPART")
  list(APPEND definitions "-DMG_ENABLE_WEBSOCKET")
  list(APPEND definitions "-DMG_ENABLE_CALLBACK_USERDATA")
  set(${dst} ${definitions} PARENT_SCOPE)
endfunction()
