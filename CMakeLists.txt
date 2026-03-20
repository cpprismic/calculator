cmake_minimum_required(VERSION 3.16)
project(Calculator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy")
        message(FATAL_ERROR ".clang-tidy config file is missing from the project root; please add it")
    endif()
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}" "--format-style=file")
    message(STATUS "clang-tidy is enabled for this build")
else()
    message(FATAL_ERROR "clang-tidy is not installed, but required for build; please install clang-tidy")
endif()

find_program(CLANG_FORMAT_EXE NAMES "clang-format")
if(CLANG_FORMAT_EXE)
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.clang-format")
        message(FATAL_ERROR ".clang-format config file is missing from the project root; please add it")
    endif()
    message(STATUS "clang-format is enabled for this build")
else()
    message(FATAL_ERROR "clang-format is not installed, but required for build; please install clang-format")
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

set(FORMAT_STAMP "${CMAKE_CURRENT_BINARY_DIR}/clang_format.stamp")
set(FORMAT_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/app.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/app.hpp"
)

add_custom_command(
    OUTPUT "${FORMAT_STAMP}"
    COMMAND "${CLANG_FORMAT_EXE}" -i ${FORMAT_SOURCES}
    COMMAND ${CMAKE_COMMAND} -E touch "${FORMAT_STAMP}"
    DEPENDS ${FORMAT_SOURCES}
    COMMENT "Running clang-format on calculator sources"
)

add_custom_target(format ALL DEPENDS "${FORMAT_STAMP}")
add_dependencies(calculator format)

target_include_directories(calculator PRIVATE
    src/
)

target_link_libraries(calculator PRIVATE MathLib::MathLib)

install(TARGETS calculator
    RUNTIME DESTINATION bin
)
