cmake_minimum_required(VERSION 3.25)
project(THREAD_POOL_SORT CXX)

# Build type
set(CMAKE_BUILD_TYPE "Debug" )
Set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -DDEBUG")
Set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

include_directories(${INCLUDE_DIR})
add_subdirectory(src)

add_executable(main main.cpp)
target_include_directories(main PUBLIC ${INCLUDE_DIR})
target_link_libraries(main PRIVATE SUNWEI pthread)