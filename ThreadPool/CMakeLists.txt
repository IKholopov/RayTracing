cmake_minimum_required(VERSION 2.8)
project(ThreadPool)
find_package (Threads)
find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
add_definitions(-std=c++14)
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
aux_source_directory(${PROJECT_SOURCE_DIR} ${PROJECT_NAME}_SRCS)
file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS ${PROJECT_INCLUDE_DIR}/*.h*)
set(${PROJECT_NAME}_SRCS ${${PROJECT_NAME}_SRCS} ${${PROJECT_NAME}_HEADERS})
include_directories("${PROJECT_BINARY_DIR}" ${CMAKE_SOURCE_DIR}/SyncContainer/include)
add_library(ThreadPool ${${PROJECT_NAME}_SRCS})
add_executable(ThreadPoolUnitTest  ${${PROJECT_NAME}_SRCS})
include_directories("${PROJECT_INCLUDE_DIR}")
target_link_libraries(ThreadPool SyncContainer ${CMAKE_THREAD_LIBS_INIT} ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
target_link_libraries(ThreadPoolUnitTest SyncContainer ${CMAKE_THREAD_LIBS_INIT} ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
set_target_properties(ThreadPool PROPERTIES LINKER_LANGUAGE C)
