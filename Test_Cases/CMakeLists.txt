cmake_minimum_required(VERSION 3.21)

add_executable(Test_Cases ./DMI_Verification_Test.c)
add_executable(dmi_verify ./DMI_Verify.c)
target_include_directories(Test_Cases PUBLIC "${PROJECT_SOURCE_DIR}"
        "${PROJECT_SOURCE_DIR}/png")

target_include_directories(dmi_verify PUBLIC "${PROJECT_SOURCE_DIR}"
        "${PROJECT_SOURCE_DIR}/png")

target_link_libraries(dmi_verify PUBLIC png)
target_link_libraries(Test_Cases PUBLIC png)

add_subdirectory(png)