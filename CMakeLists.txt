cmake_minimum_required(VERSION 3.1)

project(redis_benchmark VERSION 0.0)

set(SOURCES
    src/redis_find.cpp
    src/main.cpp
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} 
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
    PRIVATE
        ${PROJECT_BINARY_DIR}/src
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(redis++ REQUIRED)

target_link_libraries(${PROJECT_NAME} redis++)