cmake_minimum_required(VERSION 3.23)

project(raptor_engine_examples)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)

link_directories(${CMAKE_SOURCE_DIR}/third_party/lib)

if(WIN32)
    set(LIBS raptor_engine)
else()
    set(LIBS )
endif(WIN32)

set(TYPES 
    1.render)

set(1.render
    1.1.colored_triangle)

function(create_project_from_sources type example)
    file(GLOB SOURCE 
            "${type}/${example}/*.hpp"
            "${type}/${example}/*.cpp"
            "${type}/${example}/*.vs"
            "${type}/${example}/*.fs")
    if(example STREQUAL "")
        set(replaced "")
        string(REPLACE "/" "_" replaced ${type})
        set(NAME ${replaced})
    else()
        set(NAME "${type}__${example}")
    endif() 
    add_executable(${NAME} ${SOURCE})
    target_link_libraries(${NAME} ${LIBS})

    if(WIN32)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/examples/${type}")
        set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/examples/${type}/Debug")
    endif(WIN32)

    file(GLOB SHADERS
            "${type}/${example}/*.vs"
            "${type}/${example}/*.fs")
    
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
    foreach(EXAMPLE ${${TYPE}})
		create_project_from_sources(${TYPE} ${EXAMPLE})
    endforeach(EXAMPLE)
endforeach(TYPE)

include_directories(${CMAKE_SOURCE_DIR}/third_party/includes 
                    ${CMAKE_SOURCE_DIR}/src/raptor_engine)