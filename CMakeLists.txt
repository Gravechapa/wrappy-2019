cmake_minimum_required(VERSION 3.11.4)
set(PROJECT wrappy-2019)
project(${PROJECT})
message(STATUS "Start configuring project ${wrappy-2019}")

set(EXECUTABLE ${PROJECT})
set(SOURCE_ROOT src)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(EXTERNAL_PROJECTS_DIR "cmake_externalprojects")

message(STATUS "Starting collecting files")
aux_source_directory(${SOURCE_ROOT} SOURCES)
file (GLOB_RECURSE SUB_SOURCES "${SOURCE_ROOT}/classes/*.c*")
set(SOURCES ${SOURCES} ${SUB_SOURCES})

file (GLOB_RECURSE HEADERS "${SOURCE_ROOT}/headers/*.h*")

source_group(Classes FILES ${SUB_SOURCES})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${SOURCE_ROOT}/headers)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(FILESYSTEM stdc++fs)
add_executable(${EXECUTABLE} ${SOURCES} ${HEADERS})
if(MSVC)
    unset(FILESYSTEM)
    set_target_properties(${EXECUTABLE} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()
add_dependencies(${EXECUTABLE} wrappy-2019)
target_link_libraries(${EXECUTABLE} ${FILESYSTEM})


#############Copies data files to a binary directory####################################
set (DATA_FILES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources/")
file (GLOB_RECURSE MAPS RELATIVE "${DATA_FILES_DIR}" "${DATA_FILES_DIR}/part-1/*")

foreach(MAP ${MAPS})
    configure_file(${DATA_FILES_DIR}/${MAP}
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${MAP} COPYONLY)
endforeach(MAP)
#########################################################################################
