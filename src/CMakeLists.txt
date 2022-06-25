cmake_minimum_required(VERSION 3.22)

add_executable(DZ31 main.cpp task1.cpp task2.cpp)

target_include_directories(DZ31 PUBLIC ${CMAKE_SOURCE_DIR}/include)