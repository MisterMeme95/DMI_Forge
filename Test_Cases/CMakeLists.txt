cmake_minimum_required(VERSION 3.21)
find_package(PNG REQUIRED)

add_executable(Test_Cases DMI_Verification_Test.c ../DMI_Struct.c ../DMIParse.c ../Icon_State.c)
add_executable(dmi_verify DMI_Verify.c ../DMI_Struct.c ../DMIParse.c ../Icon_State.c)
add_executable(image_convert Image_Converter.c ../DMI_Struct.c ../DMIParse.c ../Icon_State.c ../PixelManip.c)
target_link_libraries(dmi_verify PUBLIC PNG::PNG)
target_link_libraries(Test_Cases PUBLIC PNG::PNG)
target_link_libraries(image_convert PUBLIC PNG::PNG)

