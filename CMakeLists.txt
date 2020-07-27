cmake_minimum_required(VERSION 3.13)
project(btcmon)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(btcmon main.cpp distr.cpp distr.h management.cpp management.h)

message(${CONAN_LIBS})

target_link_libraries(btcmon ${CONAN_LIBS})