cmake_minimum_required(VERSION 3.14)

project(PrometheusClient
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Library sources
file(GLOB LIB_SOURCES "src/*.cpp")

add_library(prom SHARED ${LIB_SOURCES})
target_include_directories(prom PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Also create static library
add_library(prom_static STATIC ${LIB_SOURCES})
target_include_directories(prom_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Testing
option(BUILD_TESTS "Build the tests" ON)

if(BUILD_TESTS)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    # Test executable
    file(GLOB TEST_SOURCES "tests/*.cpp")
    add_executable(prom_tests ${TEST_SOURCES})

    target_link_libraries(prom_tests
        PRIVATE
            prom_static
            GTest::gtest
            GTest::gtest_main
    )

    target_include_directories(prom_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

    include(GoogleTest)
    gtest_discover_tests(prom_tests)
endif()
