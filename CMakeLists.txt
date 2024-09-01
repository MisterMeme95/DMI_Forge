cmake_minimum_required(VERSION 3.21)
project (DMI_FORGE C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wuninitialized")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wuninitialized")

#add_executable(DMI_Forge main.c DMI_Struct.c DMIParse.c Icon_State.c)
add_executable(dmi2sheet DMI2Sheet.c  DMIParse.c Icon_State.c DMI_Struct.c PixelManip.c data_structure.c)
add_executable(png_transform png_transform.c PixelManip.c data_structure.c DMI_Struct.c DMIParse.c Icon_State.c)
add_executable(partition_test partition_test.c PixelManip.c data_structure.c DMI_Struct.c DMIParse.c Icon_State.c)
#add_executable(aespritefix Aesprite_Fixer.c DMIParse.c Icon_State.c DMI_Struct.c)
add_executable(tmxtest PixelManip.c data_structure.c DMI_Struct.c DMIParse.c Icon_State.c tmx.cpp data_structure.c)

#llol
add_subdirectory(Test_Cases)
find_package(tmxlite)

# Then, link your executable or library with the corresponding package targets:

find_package(PNG REQUIRED)

target_link_libraries(tmxtest tmxlite::tmxlite PNG::PNG)

#target_link_libraries(DMI_Forge PUBLIC PNG::PNG)
target_link_libraries(dmi2sheet PUBLIC PNG::PNG)
#target_link_libraries(aespritefix PUBLIC PNG::PNG)
target_link_libraries(png_transform PUBLIC PNG::PNG)
target_link_libraries(partition_test PUBLIC PNG::PNG)