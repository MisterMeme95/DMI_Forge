cmake_minimum_required(VERSION 3.21)
project (DMI_FORGE C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wuninitialized")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wuninitialized")

# Executables
add_executable(dmi2sheet src/DMI/DMI2Sheet.c src/DMI/DMIParse.c src/DMI/Icon_State.c src/DMI/DMI_Struct.c src/PixelManip.c src/data_structure.c
        src/list.c
        src/vector.cpp)
add_executable(png_transform src/png_transform.c src/PixelManip.c src/list.c src/data_structure.c src/DMI/DMI_Struct.c src/DMI/DMIParse.c src/DMI/Icon_State.c
        src/vector.cpp)
add_executable(partition_test src/partition_test.c src/PixelManip.c src/list.c src/data_structure.c src/DMI/DMI_Struct.c src/DMI/DMIParse.c src/DMI/Icon_State.c
        src/vector.cpp)
add_executable(tmxtest src/PixelManip.c src/list.c src/data_structure.c src/DMI/DMI_Struct.c src/DMI/DMIParse.c src/DMI/Icon_State.c src/tmx.cpp
        src/vector.cpp)
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
