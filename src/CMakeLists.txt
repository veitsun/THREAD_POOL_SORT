add_library(CMatrixData STATIC CMatrixData.cpp)
add_library(CGemmWithC STATIC CGemmWithC.cpp)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE CMatrixData)
target_link_libraries(main PRIVATE CGemmWithC)