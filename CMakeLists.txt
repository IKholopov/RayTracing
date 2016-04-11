cmake_minimum_required(VERSION 2.8)
project(RayTracing)

add_definitions(-std=c++14)

add_subdirectory(SyncContainer)
add_subdirectory(ThreadPool)
add_subdirectory(RayTracer)
add_subdirectory(Gl_View)
add_subdirectory(UnitTests)

