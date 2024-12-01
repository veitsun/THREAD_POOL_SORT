# select all cpp  in this directory
file(GLOB HEllOLIB_SRC_POOL ${PROJECT_SOURCE_DIR}/src/*.cpp)

# combime both list into hello_src

# set(HELLOLIB_SRC_pool ${HEllOLIB_SRC_POOL})

find_path(INCLUDE_DIR ThreadPool.h HINTS ${CMAKE_SOURCE_DIR}/include)
# 检查是否找到了路径
if(NOT INCLUDE_DIR)
    message(FATAL_ERROR "ThreadPool.h not found!")
endif()
include_directories(${INCLUDE_DIR})
# # 输出调试信息
# message(STATUS "Project source dir: ${PROJECT_SOURCE_DIR}")
# message(STATUS "Source files found: ${HELLOLIB_SRC_POOL}")

# create a  library from the collected source files
add_library(SUNWEI SHARED ${HEllOLIB_SRC_POOL})
target_include_directories(SUNWEI PUBLIC ${INCLUDE_DIR})

