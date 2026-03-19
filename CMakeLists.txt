cmake_minimum_required(VERSION 3.16)
project(Calculator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-checks=*;-format-style=file")
else()
    message(FATAL_ERROR "clang-tidy is not installed, but required for build; please install clang-tidy")
endif()

include(FetchContent)

FetchContent_Declare(
    math_lib
    GIT_REPOSITORY https://github.com/cpprismic/math
    GIT_TAG main
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(math_lib)

add_executable(calculator 
    src/main.cpp
    src/app.cpp
)

target_include_directories(calculator PRIVATE
    src/
)

target_link_libraries(calculator PRIVATE MathLib::MathLib)
