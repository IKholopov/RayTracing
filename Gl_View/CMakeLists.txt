aux_source_directory(./src SRC_LIST)
set(LINK_FLAGS " -lGL -lGLEW -lglut")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINK_FLAGS}")
add_executable(${PROJECT_NAME} ${SRC_LIST})
