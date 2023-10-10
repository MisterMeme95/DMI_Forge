cmake_minimum_required(VERSION 3.21)
project (DMI_FORGE C CXX)

set(CMAKE_C_STANDARD 23)

add_executable(DMI_Forge main.c DMI_Struct.c DMIParse.c Icon_State.c)
add_executable(dmi2sheet DMI2Sheet.c  DMIParse.c Icon_State.c DMI_Struct.c
)

find_package(PNG REQUIRED)
target_link_libraries(DMI_Forge PUBLIC PNG::PNG)
target_link_libraries(dmi2sheet PUBLIC PNG::PNG)