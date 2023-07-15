cmake_minimum_required(VERSION 3.23)

project(raptor_engine)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/third_party/SDL2/cmake/")

add_subdirectory(src)
add_subdirectory(third_party)

set(TESTS ON CACHE BOOL "" ${REWRITE_FORCE})
set(EXAMPLES ON CACHE BOOL "" ${REWRITE_FORCE})

if(${TESTS})
    enable_testing()
    add_subdirectory(tests)
endif()
if(${EXAMPLES})
    add_subdirectory(examples)
endif()
