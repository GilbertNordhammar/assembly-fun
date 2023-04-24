cmake_minimum_required(VERSION 3.10)

project(HelloWorld)

# Set recurring variables
set(TARGET_NAME "HelloWorld")
set(SRC_DIR_RELATIVE "src")
set(SRC_DIR_ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR_RELATIVE})

# Gather project files
file(GLOB_RECURSE ASM_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.asm")
file(GLOB_RECURSE CPP_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE H_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.h")
list(APPEND SRC_FILES ${ASM_FILES} ${CPP_FILES} ${H_FILES})

# Add include directories and files to be compiled in executable
include_directories(${SRC_DIR_ABSOLUTE})
add_executable(${TARGET_NAME} ${SRC_FILES})

# Set shared config
SET (COMPILE_OPTIONS_ALL )
SET (COMPILE_DEFINITIONS_ALL )
target_compile_options(${TARGET_NAME} PRIVATE ${COMPILE_OPTIONS_ALL})
target_compile_definitions(${TARGET_NAME} PRIVATE ${COMPILE_DEFINITIONS_ALL})

# Set config for Debug build target
set(COMPILE_OPTIONS_DEBUG /Zi)
set(COMPILE_DEFINITIONS_DEBUG DEBUG_BUILD)
target_compile_options(${TARGET_NAME} PRIVATE $<$<CONFIG:Debug>:${COMPILE_OPTIONS_DEBUG}>)
target_compile_definitions(${TARGET_NAME} PRIVATE $<$<CONFIG:Debug>:${COMPILE_DEFINITIONS_DEBUG}>)

# Set config for Release build target
set(COMPILE_OPTIONS_RELEASE -O2)
set(COMPILE_DEFINITIONS_RELEASE RELEASE_BUILD)
target_compile_options(${TARGET_NAME} PRIVATE $<$<CONFIG:Release>:${COMPILE_OPTIONS_RELEASE}>)
target_compile_definitions(${TARGET_NAME} PRIVATE $<$<CONFIG:Release>:${COMPILE_DEFINITIONS_RELEASE}>)

# Setup for maintaining physical folder structure in project
foreach(SINGLE_SRC_FILE ${SRC_FILES})
    get_filename_component(PARENT_DIR ${SINGLE_SRC_FILE} DIRECTORY)
    string(REPLACE "/" "\\" SRC_GROUP ${PARENT_DIR})
    source_group(${SRC_GROUP} FILES ${SINGLE_SRC_FILE})
endforeach()