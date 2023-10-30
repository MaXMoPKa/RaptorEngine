cmake_minimum_required(VERSION 3.23)

project(raptor_engine_tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)

link_directories(${CMAKE_SOURCE_DIR}/third_party/lib)

if(WIN32)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        b796f7d44681514f58a683a3a71ff17c94edb0c1 # v1.13.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
endif(WIN32)

if(WIN32)
    set(LIBS raptor_engine gtest_main gtest gmock gmock_main)
else()
    set(LIBS )
endif(WIN32)

set(TYPES 
    render)

set(render
    shader_manager
    shader_program
    geometry_manager)

function(create_project_from_sources type test)
    file(GLOB SOURCE main.cpp
                     "${type}/${test}/*.hpp"
                     "${type}/${test}/*.cpp"
                     "${type}/${test}/*.vs"
                     "${type}/${test}/*.fs")

    if(test STREQUAL "")
        set(replaced "")
        string(REPLACE "/" "_" replaced ${type})
        set(NAME ${replaced})
    else()
        set(NAME "${type}__${test}")
    endif() 

    add_executable(${NAME} ${SOURCE})
    target_link_libraries(${NAME} PUBLIC ${LIBS})

    if(WIN32)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/tests/${type}")
        set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/tests/${type}/Debug")
    endif(WIN32)

    file(GLOB SHADERS
            "${type}/${test}/*.vs"
            "${type}/${test}/*.fs")

    file(GLOB DLLS "${CMAKE_SOURCE_DIR}/third_party/dlls/*.dll")
    foreach(SHADER ${SHADERS})
        if(WIN32)
            add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>)
            add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${DLLS} $<TARGET_FILE_DIR:${NAME}>)
        endif(WIN32)
    endforeach(SHADER)

    if(MSVC)
        configure_file(${CMAKE_SOURCE_DIR}/configuration/visualstudio.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.vcxproj.user @ONLY)
    endif(MSVC)
endfunction()

foreach(TYPE ${TYPES})
    foreach(TEST ${${TYPE}})
		create_project_from_sources(${TYPE} ${TEST})
    endforeach(TEST)
endforeach(TYPE)

include_directories(${CMAKE_SOURCE_DIR}/third_party/includes 
                    ${CMAKE_SOURCE_DIR}/src/raptor_engine)