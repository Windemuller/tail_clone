cmake_minimum_required(VERSION 3.20)
project(tail_clone VERSION 0.1.0)
add_executable(tail_clone inc/tail_clone_config.h.in)

target_sources(tail_clone PRIVATE src/tail_clone.cpp)
include_directories(./inc ${Boost_INCLUDE_DIRS} ${PROJECT_BINARY_DIR})

find_package(Boost COMPONENTS program_options REQUIRED)
target_link_libraries(tail_clone ${Boost_LIBRARIES})

configure_file(inc/tail_clone_config.h.in tail_clone_config.h)

set(target tail_clone)
set(CMAKE_CXX_STANDARD 20)

target_sources(${target} PRIVATE src/tail_clone.cpp)
