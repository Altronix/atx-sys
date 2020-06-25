set(MONGOOSE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/mongoose)
set(MONGOOSE_TEST_FILE ${MONGOOSE_SOURCE_DIR}/CMakeLists.txt)

list(APPEND DEFS "-DMG_ENABLE_HTTP_STREAMING_MULTIPART")
list(APPEND DEFS "-DMG_ENABLE_WEBSOCKET")
list(APPEND DEFS "-DMG_ENABLE_CALLBACK_USERDATA")
add_definitions(${DEFS})
add_library(mongoose-shared SHARED 
  ${MONGOOSE_SOURCE_DIR}/mongoose.c 
  ${MONGOOSE_SOURCE_DIR}/mongoose.h)
target_include_directories(mongoose-shared PUBLIC ${MONGOOSE_SOURCE_DIR})
install(TARGETS mongoose-shared
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include)

function(append_mongoose_compiler_flags dst)
  set(definitions ${DEFS})
  set(${dst} ${definitions} PARENT_SCOPE)
endfunction()
