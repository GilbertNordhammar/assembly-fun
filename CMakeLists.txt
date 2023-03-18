cmake_minimum_required(VERSION 3.10)

project(Assembly-Fun)

# Set C++ version and enable MASM
set(CMAKE_CXX_STANDARD 17)
enable_language(ASM_MASM)

add_subdirectory(Examples/Hello_World)