cmake_minimum_required(VERSION 3.10)

project(C0_Compiler VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

# 查找源文件
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp" "include/*/*.h" "include/*/*.hpp")

# 添加检查确保找到了源文件
if(NOT SOURCES)
    message(FATAL_ERROR "No source files found in src/ directory. Please ensure you have .cpp files in the src folder.")
endif()

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)