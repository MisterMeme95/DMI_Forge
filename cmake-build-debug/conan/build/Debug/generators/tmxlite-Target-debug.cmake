# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(tmxlite_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(tmxlite_FRAMEWORKS_FOUND_DEBUG "${tmxlite_FRAMEWORKS_DEBUG}" "${tmxlite_FRAMEWORK_DIRS_DEBUG}")

set(tmxlite_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET tmxlite_DEPS_TARGET)
    add_library(tmxlite_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET tmxlite_DEPS_TARGET
             PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${tmxlite_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${tmxlite_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:zstd::libzstd_static;pugixml::pugixml;ZLIB::ZLIB>
             APPEND)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### tmxlite_DEPS_TARGET to all of them
conan_package_library_targets("${tmxlite_LIBS_DEBUG}"    # libraries
                              "${tmxlite_LIB_DIRS_DEBUG}" # package_libdir
                              "${tmxlite_BIN_DIRS_DEBUG}" # package_bindir
                              "${tmxlite_LIBRARY_TYPE_DEBUG}"
                              "${tmxlite_IS_HOST_WINDOWS_DEBUG}"
                              tmxlite_DEPS_TARGET
                              tmxlite_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "tmxlite"    # package_name
                              "${tmxlite_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${tmxlite_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## GLOBAL TARGET PROPERTIES Debug ########################################
    set_property(TARGET tmxlite::tmxlite
                 PROPERTY INTERFACE_LINK_LIBRARIES
                 $<$<CONFIG:Debug>:${tmxlite_OBJECTS_DEBUG}>
                 $<$<CONFIG:Debug>:${tmxlite_LIBRARIES_TARGETS}>
                 APPEND)

    if("${tmxlite_LIBS_DEBUG}" STREQUAL "")
        # If the package is not declaring any "cpp_info.libs" the package deps, system libs,
        # frameworks etc are not linked to the imported targets and we need to do it to the
        # global target
        set_property(TARGET tmxlite::tmxlite
                     PROPERTY INTERFACE_LINK_LIBRARIES
                     tmxlite_DEPS_TARGET
                     APPEND)
    endif()

    set_property(TARGET tmxlite::tmxlite
                 PROPERTY INTERFACE_LINK_OPTIONS
                 $<$<CONFIG:Debug>:${tmxlite_LINKER_FLAGS_DEBUG}> APPEND)
    set_property(TARGET tmxlite::tmxlite
                 PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                 $<$<CONFIG:Debug>:${tmxlite_INCLUDE_DIRS_DEBUG}> APPEND)
    # Necessary to find LINK shared libraries in Linux
    set_property(TARGET tmxlite::tmxlite
                 PROPERTY INTERFACE_LINK_DIRECTORIES
                 $<$<CONFIG:Debug>:${tmxlite_LIB_DIRS_DEBUG}> APPEND)
    set_property(TARGET tmxlite::tmxlite
                 PROPERTY INTERFACE_COMPILE_DEFINITIONS
                 $<$<CONFIG:Debug>:${tmxlite_COMPILE_DEFINITIONS_DEBUG}> APPEND)
    set_property(TARGET tmxlite::tmxlite
                 PROPERTY INTERFACE_COMPILE_OPTIONS
                 $<$<CONFIG:Debug>:${tmxlite_COMPILE_OPTIONS_DEBUG}> APPEND)

########## For the modules (FindXXX)
set(tmxlite_LIBRARIES_DEBUG tmxlite::tmxlite)
