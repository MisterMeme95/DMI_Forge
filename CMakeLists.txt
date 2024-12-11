cmake_minimum_required(VERSION 3.21)
project (DMI_FORGE C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wuninitialized")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wuninitialized")

# Executables
add_executable(dmi2sheet DMI2Sheet.c  DMIParse.c Icon_State.c DMI_Struct.c PixelManip.c data_structure.c
        list.c
        vector.cpp)
add_executable(png_transform png_transform.c PixelManip.c list.c data_structure.c DMI_Struct.c DMIParse.c Icon_State.c
        vector.cpp)
add_executable(partition_test partition_test.c PixelManip.c list.c data_structure.c DMI_Struct.c DMIParse.c Icon_State.c
        vector.cpp)
add_executable(tmxtest PixelManip.c list.c data_structure.c DMI_Struct.c DMIParse.c Icon_State.c tmx.cpp
        vector.cpp)
add_executable(ai_practice ai_practice.c)
# Find and Link Libraries
find_package(tmxlite REQUIRED)
find_package(PNG REQUIRED)

add_subdirectory(Test_Cases)
add_subdirectory(src/GUI)  # Include the Gui/Cmake directory



target_link_libraries(tmxtest tmxlite::tmxlite PNG::PNG)
target_link_libraries(dmi2sheet PUBLIC PNG::PNG)
target_link_libraries(ai_practice PUBLIC PNG::PNG)
target_link_libraries(png_transform PUBLIC PNG::PNG)
target_link_libraries(partition_test PUBLIC PNG::PNG)
