cmake_minimum_required(VERSION 3.23)

project(raptor_engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(examples)
add_subdirectory(src)

set(TESTS ON CACHE BOOL "" ${REWRITE_FORCE})

if(${TESTS})
    enable_testing()
    add_subdirectory(tests)
endif()


