# select all cpp  in this directory
# 
file(GLOB HEllOLIB_SRC_POOL ${PROJECT_SOURCE_DIR}/src/*.cpp)

# combime both list into hello_src

# set(HELLOLIB_SRC_pool ${HEllOLIB_SRC_POOL})

# # 输出调试信息
# message(STATUS "Project source dir: ${PROJECT_SOURCE_DIR}")
# message(STATUS "Source files found: ${HELLOLIB_SRC_POOL}")

# create a  library from the collected source files
add_library(SUNWEI SHARED ${HEllOLIB_SRC_POOL})

