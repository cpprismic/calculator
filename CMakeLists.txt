cmake_minimum_required(VERSION 3.16)
project(Calculator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type (Debug or Release)" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

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
    UPDATE_DISCONNECTED TRUE
)
FetchContent_MakeAvailable(math_lib)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.11.3
    GIT_SHALLOW TRUE
    UPDATE_DISCONNECTED TRUE
)
set(JSON_BuildTests OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG        v1.16.0
    GIT_SHALLOW    TRUE
    UPDATE_DISCONNECTED TRUE
)
set(SPDLOG_BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(spdlog)
set_target_properties(spdlog PROPERTIES CXX_CLANG_TIDY "")

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG        v1.17.0
    GIT_SHALLOW    TRUE
    UPDATE_DISCONNECTED TRUE
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
set_target_properties(gtest PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gtest_main PROPERTIES CXX_CLANG_TIDY "")

add_executable(calculator 
    src/main.cpp
    src/app/parser.cpp
    src/app/calculator.cpp
    src/logger/logger.cpp
)

set(FORMAT_STAMP "${CMAKE_CURRENT_BINARY_DIR}/clang_format.stamp")
set(FORMAT_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/app/task.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/app/exceptions.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/app/parser.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/app/parser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/app/calculator.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/app/calculator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/logger/logger.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/logger/logger.cpp"
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

target_link_libraries(calculator PRIVATE 
    MathLib::MathLib
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)

install(TARGETS calculator
    RUNTIME DESTINATION bin
)

enable_testing()
add_subdirectory(tests)
