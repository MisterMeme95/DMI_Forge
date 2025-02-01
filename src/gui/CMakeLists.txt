cmake_minimum_required(VERSION 3.5)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(PNG REQUIRED)
set(CMAKE_AUTOMOC ON)

# Create an executable for the test subdirectory
add_executable(test main.cpp ../dmi/DMI_Struct.c ../image_manipulation/PixelManip.c ../data_structures/data_structure.c ../data_structures/list.c ../data_structures/vector.cpp
        ../dmi/DMIParse.c ../dmi/Icon_State.c IconLabel.cpp Docker.cpp)

# Add a post-build step to deploy required DLLs (optional)
if (WIN32)
    set(WINDEPLOYQT_PATH "C:/Qt/6.8.0/mingw_64/bin/windeployqt.exe")
    add_custom_command(TARGET test POST_BUILD
            COMMAND "${WINDEPLOYQT_PATH}" --no-compiler-runtime --dir "$<TARGET_FILE_DIR:test>" "$<TARGET_FILE:test>"
            COMMENT "Running windeployqt to deploy required DLLs"
    )
endif()




target_link_libraries(test Qt6::Widgets PNG::PNG)
