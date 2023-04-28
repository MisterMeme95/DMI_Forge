cmake_minimum_required(VERSION 3.21)
project(DMI_FORGE C)

set(CMAKE_C_STANDARD 23)

add_library(pretend_obj OBJECT Object_C.c)
add_executable(Preten_Exe $<TARGET_OBJECTS:pretend_obj>)

add_subdirectory(zlib)

add_subdirectory(png)
add_executable(DMI_Forge main.c)
target_link_libraries(DMI_Forge PUBLIC png)

target_include_directories(DMI_Forge PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/png"
        )