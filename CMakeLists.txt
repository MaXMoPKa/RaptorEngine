cmake_minimum_required(VERSION 3.23)

project(raptor_engine)

add_subdirectory(src)
add_subdirectory(third_party)

set(TESTS    ON CACHE BOOL "" ${REWRITE_FORCE})

if(${TESTS})
    add_subdirectory(tests)
endif()
