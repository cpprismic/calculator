add_executable(calculator_bench
    main.cpp
    ${CMAKE_SOURCE_DIR}/src/app/calculator.cpp
    ${CMAKE_SOURCE_DIR}/src/logger/logger.cpp
    ${CMAKE_SOURCE_DIR}/src/config/config.cpp
    ${CMAKE_SOURCE_DIR}/src/storage/storage.cpp
    ${CMAKE_SOURCE_DIR}/src/storage/cache/cache.cpp
    ${CMAKE_SOURCE_DIR}/src/storage/db/pg_connection.cpp
    ${CMAKE_SOURCE_DIR}/src/storage/db/pg_result.cpp
    ${CMAKE_SOURCE_DIR}/src/storage/db/repository.cpp
)

set_target_properties(calculator_bench PROPERTIES CXX_CLANG_TIDY "")

target_link_libraries(calculator_bench PRIVATE
    MathLib::MathLib
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    PostgreSQL::PostgreSQL
)

target_include_directories(calculator_bench PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)
