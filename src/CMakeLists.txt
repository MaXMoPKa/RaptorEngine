cmake_minimum_required(VERSION 3.23)

project(raptor_engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/*.hpp
                               ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE SOURCE_CODE_FILES ${PROJECT_SOURCE_DIR}/*.cpp)

set(SOURCES ${HEADER_FILES} ${SOURCE_CODE_FILES})

add_library(${PROJECT_NAME} ${SOURCES})

find_program(IWYU_PATH NAMES include-what-you-use include_what_you_use iwyu REQUIRED)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${IWYU_PATH})

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2) 

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})