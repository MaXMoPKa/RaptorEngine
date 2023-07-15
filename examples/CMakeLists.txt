cmake_minimum_required(VERSION 3.23)

project(raptor_engine_examples)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/third_party/SDL2/cmake/")

if(WIN32)
    find_package(SDL2 REQUIRED)
    set(LIBS raptor_engine glad)
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

    # copy shaders
    file(GLOB SHADERS
            "${type}/${example}/*.vs"
            "${type}/${example}/*.fs")
    
    foreach(SHADER ${SHADERS})
        if(WIN32)
            add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>)
        endif(WIN32)
    endforeach(SHADER)

    # copy dlls
    if (EXISTS ${CMAKE_BINARY_DIR}/examples/Debug/)
        # DO NOTHING
    else ()
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/examples/Debug/)
    endif ()
    if (EXISTS ${CMAKE_BINARY_DIR}/examples/Release/)
        # DO NOTHING
    else ()
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/examples/Release/)
    endif ()

    add_custom_target(SDL2CopyBinaries_examples
        # todo: check if debug and release folder exist
        # debug version
        COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_BINDIR}/SDL2.dll ${CMAKE_BINARY_DIR}/examples/Debug/
        COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_BINDIR}/SDL2.dll ${CMAKE_BINARY_DIR}/examples/Release/

        # Output Message
        COMMENT "Copying SDL2 binaries from '${SDL2_BINDIR}/' to '${CMAKE_BINARY_DIR}/'" VERBATIM
    )
    add_dependencies(${NAME} SDL2CopyBinaries_examples)
endfunction()

foreach(TYPE ${TYPES})
    foreach(EXAMPLE ${${TYPE}})
		create_project_from_sources(${TYPE} ${EXAMPLE})
    endforeach(EXAMPLE)
endforeach(TYPE)

include_directories(${SDL2_INCLUDE_DIRS})