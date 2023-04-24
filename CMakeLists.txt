cmake_minimum_required(VERSION 3.10)

project(Assembly-Fun)

# Set C++ version and enable MASM
set(CMAKE_CXX_STANDARD 17)
enable_language(ASM_MASM)

# SDK variables
set(SDK_SDL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sdk/SDL)

# Examples
add_subdirectory(Examples/Hello_World)
add_subdirectory(Examples/Snake)