cmake_minimum_required(VERSION 3.23)

project(raptor_engine_tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories($ENV{GMOCK_HOME}/include $ENV{GMOCK_HOME}/gtest/include)
link_directories($ENV{GMOCK_HOME}/mybuild $ENV{GMOCK_HOME}/gtest/mybuild)
add_definitions(-std=c++0x)
set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -Wall")

file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/*.hpp
                               ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE SOURCE_CODE_FILES ${PROJECT_SOURCE_DIR}/*.cpp)

set(SOURCES ${HEADER_FILES} ${SOURCE_CODE_FILES})

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} gmock)
target_link_libraries(${PROJECT_NAME} gtest)
target_link_libraries(${PROJECT_NAME} raptor_engine)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
