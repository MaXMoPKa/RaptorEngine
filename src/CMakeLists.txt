cmake_minimum_required(VERSION 3.23)

project(raptor_engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTOR ${CMAKE_BINARY_DIR}/bin)
endif(WIN32)

link_directories(${CMAKE_SOURCE_DIR}/third_party/lib)

find_package(SDL2 REQUIRED)
message(STATUS "Found SDL2 in ${SLD2_INCLUDE_DIR}")

if(WIN32)
    set(LIBS SDL2)
else()
    set(LIBS )
endif(WIN32)

configure_file(${CMAKE_SOURCE_DIR}/configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/src/configuration) 

add_library(GLAD STATIC "${CMAKE_SOURCE_DIR}/src/third_party/glad/glad.c")
set(LIBS ${LIBS} GLAD)

add_library(STB_IMAGE STATIC "${CMAKE_SOURCE_DIR}/src/third_party/stb_image/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)

file(GLOB_RECURSE SOURCE "${CMAKE_SOURCE_DIR}/src/raptor_engine/*.h"
                         "${CMAKE_SOURCE_DIR}/src/raptor_engine/*.hpp"
                         "${CMAKE_SOURCE_DIR}/src/raptor_engine/*.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCE})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})

file(GLOB DLLS "${CMAKE_SOURCE_DIR}/third_party/dlls/*.dll")
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${DLLS} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif(WIN32)

include_directories(${CMAKE_SOURCE_DIR}/third_party/includes 
                    ${CMAKE_SOURCE_DIR}/src/raptor_engine)

if(${TESTS})
     target_compile_definitions(${PROJECT_NAME} PUBLIC TESTS=1)
endif()