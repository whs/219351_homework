cmake_minimum_required(VERSION 3.3)
project(WebAppHw2)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(SOURCE_FILES main.c)
add_executable(WebAppHw2 ${SOURCE_FILES})
target_link_libraries(WebAppHw2 Threads::Threads)