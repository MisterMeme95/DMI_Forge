#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PNG::png_shared" for configuration ""
set_property(TARGET PNG::png_shared APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(PNG::png_shared PROPERTIES
  IMPORTED_IMPLIB_NOCONFIG "${_IMPORT_PREFIX}/lib/libpng16.dll.a"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/libpng16.dll"
  )

list(APPEND _cmake_import_check_targets PNG::png_shared )
list(APPEND _cmake_import_check_files_for_PNG::png_shared "${_IMPORT_PREFIX}/lib/libpng16.dll.a" "${_IMPORT_PREFIX}/bin/libpng16.dll" )

# Import target "PNG::png_static" for configuration ""
set_property(TARGET PNG::png_static APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(PNG::png_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libpng16.a"
  )

list(APPEND _cmake_import_check_targets PNG::png_static )
list(APPEND _cmake_import_check_files_for_PNG::png_static "${_IMPORT_PREFIX}/lib/libpng16.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
