########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

set(tmxlite_COMPONENT_NAMES "")
list(APPEND tmxlite_FIND_DEPENDENCY_NAMES zstd pugixml ZLIB)
list(REMOVE_DUPLICATES tmxlite_FIND_DEPENDENCY_NAMES)
set(zstd_FIND_MODE "NO_MODULE")
set(pugixml_FIND_MODE "NO_MODULE")
set(ZLIB_FIND_MODE "NO_MODULE")

########### VARIABLES #######################################################################
#############################################################################################
set(tmxlite_PACKAGE_FOLDER_DEBUG "C:/Users/jonat/.conan2/p/b/tmxli4b63b68a2f810/p")
set(tmxlite_BUILD_MODULES_PATHS_DEBUG )


set(tmxlite_INCLUDE_DIRS_DEBUG "${tmxlite_PACKAGE_FOLDER_DEBUG}/include")
set(tmxlite_RES_DIRS_DEBUG )
set(tmxlite_DEFINITIONS_DEBUG "-DTMXLITE_STATIC")
set(tmxlite_SHARED_LINK_FLAGS_DEBUG )
set(tmxlite_EXE_LINK_FLAGS_DEBUG )
set(tmxlite_OBJECTS_DEBUG )
set(tmxlite_COMPILE_DEFINITIONS_DEBUG "TMXLITE_STATIC")
set(tmxlite_COMPILE_OPTIONS_C_DEBUG )
set(tmxlite_COMPILE_OPTIONS_CXX_DEBUG )
set(tmxlite_LIB_DIRS_DEBUG "${tmxlite_PACKAGE_FOLDER_DEBUG}/lib")
set(tmxlite_BIN_DIRS_DEBUG )
set(tmxlite_LIBRARY_TYPE_DEBUG STATIC)
set(tmxlite_IS_HOST_WINDOWS_DEBUG 1)
set(tmxlite_LIBS_DEBUG tmxlite-s-d)
set(tmxlite_SYSTEM_LIBS_DEBUG )
set(tmxlite_FRAMEWORK_DIRS_DEBUG )
set(tmxlite_FRAMEWORKS_DEBUG )
set(tmxlite_BUILD_DIRS_DEBUG )
set(tmxlite_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(tmxlite_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${tmxlite_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${tmxlite_COMPILE_OPTIONS_C_DEBUG}>")
set(tmxlite_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${tmxlite_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${tmxlite_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${tmxlite_EXE_LINK_FLAGS_DEBUG}>")


set(tmxlite_COMPONENTS_DEBUG )