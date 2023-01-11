cmake_minimum_required(VERSION 3.20)
project(tail_clone)
add_executable(tail_clone)

target_sources(tail_clone PRIVATE src/tail_clone.cpp src/arg_parser.cpp)
include_directories(./inc ${Boost_INCLUDE_DIRS})

find_package(Boost COMPONENTS program_options REQUIRED)
target_link_libraries(tail_clone ${Boost_LIBRARIES})

set(target tail_clone)
set(CMAKE_CXX_STANDARD 20)

target_sources(${target} PRIVATE src/tail_clone.cpp)
