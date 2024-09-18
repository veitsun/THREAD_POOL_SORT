add_library(func_c_girl STATIC func_c_girl.cpp)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE func_c_girl)