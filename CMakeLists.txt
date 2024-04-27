cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME Prom)

project(${PROJECT_NAME})

file(GLOB SOURCES "src/*.cpp")

add_library(Prom SHARED ${SOURCES})

# set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(${PROJECT_NAME} PRIVATE -std=c++17)