cmake_minimum_required(VERSION 3.21)

add_executable(Test_Cases Test_Cases/DMI_Verification_Test.c)
target_include_directories(Test_Cases PUBLIC "${PROJECT_SOURCE_DIR}"
        "${PROJECT_SOURCE_DIR}/png")
target_link_libraries(Test_Cases PUBLIC png)

add_subdirectory(png)