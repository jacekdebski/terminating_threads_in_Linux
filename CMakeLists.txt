cmake_minimum_required(VERSION 3.19)
project(terminating_threads_in_linux)

set(CMAKE_CXX_STANDARD 14)

add_executable(terminating_threads_in_linux main.cpp src/threads.cpp include/threads.h)
target_compile_options(terminating_threads_in_linux PUBLIC -Wall -Wextra -Wpedantic -pthread)