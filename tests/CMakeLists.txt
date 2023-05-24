cmake_minimum_required(VERSION 3.23)

project(raptor_engine_tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/third_party/SDL2/cmake/")

file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/*.hpp
                               ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE SOURCE_CODE_FILES ${PROJECT_SOURCE_DIR}/*.cpp)

set(SOURCES ${HEADER_FILES} ${SOURCE_CODE_FILES})

add_executable(${PROJECT_NAME} ${SOURCES})

if(UNIX)
    find_program(IWYU_PATH NAMES include-what-you-use include_what_you_use iwyu REQUIRED)
    set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${IWYU_PATH})

    target_link_libraries(${PROJECT_NAME} PRIVATE gtest)
    target_link_libraries(${PROJECT_NAME} PRIVATE gmock)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)
elseif(WIN32)
    find_package(SDL2 REQUIRED)
    target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
    
    if (EXISTS ${CMAKE_BINARY_DIR}/tests/Debug/)
        # DO NOTHING
    else ()
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/tests/Debug/)
    endif ()
    if (EXISTS ${CMAKE_BINARY_DIR}/tests/Release/)
        # DO NOTHING
    else ()
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/tests/Release/)
    endif ()

    add_custom_target(SDL2CopyBinaries
        # todo: check if debug and release folder exist
        # debug version
        COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_BINDIR}/SDL2.dll ${CMAKE_BINARY_DIR}/tests/Debug/
        COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_BINDIR}/SDL2.dll ${CMAKE_BINARY_DIR}/tests/Release/

        # Output Message
        COMMENT "Copying SDL2 binaries from '${SDL2_BINDIR}/' to '${CMAKE_BINARY_DIR}/'" VERBATIM
    )
    add_dependencies(${PROJECT_NAME} SDL2CopyBinaries)

    include(FetchContent)
    FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        b796f7d44681514f58a683a3a71ff17c94edb0c1 # v1.13.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    target_link_libraries(${PROJECT_NAME} PRIVATE gtest_main gtest gmock gmock_main)
    add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC raptor_engine)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})