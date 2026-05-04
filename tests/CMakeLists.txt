add_executable(calculator_tests
    app/parser_test.cpp
    app/calculator_test.cpp
    storage/cache_test.cpp
    ${CMAKE_SOURCE_DIR}/src/app/parser.cpp
    ${CMAKE_SOURCE_DIR}/src/app/calculator.cpp
    ${CMAKE_SOURCE_DIR}/src/logger/logger.cpp
    ${CMAKE_SOURCE_DIR}/src/storage/cache/cache.cpp
)

set_target_properties(calculator_tests PROPERTIES CXX_CLANG_TIDY "")

target_link_libraries(calculator_tests PRIVATE
    GTest::gtest_main
    MathLib::MathLib
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)

target_include_directories(calculator_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_include_directories(calculator_tests SYSTEM PRIVATE
    ${spdlog_SOURCE_DIR}/include
    ${math_lib_SOURCE_DIR}/include
)

include(GoogleTest)
gtest_discover_tests(calculator_tests)

find_program(VALGRIND_EXE NAMES valgrind)
if(VALGRIND_EXE)
    add_test(
        NAME ValgrindMemcheck
        COMMAND "${VALGRIND_EXE}"
            --leak-check=full
            --show-leak-kinds=definite,indirect
            --error-exitcode=1
            --quiet
            $<TARGET_FILE:calculator_tests>
    )
endif()
