
# AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/*.cpp )
# AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/*.cu HELLOlib_src)

# add_library(CMatrixData STATIC $(HELLOlib_src))
# add_library(CGemmWithC STATIC CGemmWithC.cpp)

# add_library(cuda_gemm)


# add


# Collect all .cpp and .cu files in the src directory
file(GLOB HELLOlib_src_cpp ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB HELLOlib_src_cu ${PROJECT_SOURCE_DIR}/src/*.cu)

# Combine both lists into HELLOlib_src
set(HELLOlib_src ${HELLOlib_src_cpp} ${HELLOlib_src_cu})

# Create a static library from the collected source files
add_library(CMatrixData STATIC ${HELLOlib_src})

add_executable(main main.cpp)

target_link_libraries(main PRIVATE CMatrixData)
# target_link_libraries(main PRIVATE CGemmWithC)