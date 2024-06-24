########## MACROS ###########################################################################
#############################################################################################

# Requires CMake > 3.15
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeDeps' generator only works with CMake >= 3.15")
endif()

if(tmxlite_FIND_QUIETLY)
    set(tmxlite_MESSAGE_MODE VERBOSE)
else()
    set(tmxlite_MESSAGE_MODE STATUS)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmakedeps_macros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/tmxliteTargets.cmake)
include(CMakeFindDependencyMacro)

check_build_type_defined()

foreach(_DEPENDENCY ${tmxlite_FIND_DEPENDENCY_NAMES} )
    # Check that we have not already called a find_package with the transitive dependency
    if(NOT ${_DEPENDENCY}_FOUND)
        find_dependency(${_DEPENDENCY} REQUIRED ${${_DEPENDENCY}_FIND_MODE})
    endif()
endforeach()

set(tmxlite_VERSION_STRING "1.4.4")
set(tmxlite_INCLUDE_DIRS ${tmxlite_INCLUDE_DIRS_DEBUG} )
set(tmxlite_INCLUDE_DIR ${tmxlite_INCLUDE_DIRS_DEBUG} )
set(tmxlite_LIBRARIES ${tmxlite_LIBRARIES_DEBUG} )
set(tmxlite_DEFINITIONS ${tmxlite_DEFINITIONS_DEBUG} )

# Only the first installed configuration is included to avoid the collision
foreach(_BUILD_MODULE ${tmxlite_BUILD_MODULES_PATHS_DEBUG} )
    message(${tmxlite_MESSAGE_MODE} "Conan: Including build module from '${_BUILD_MODULE}'")
    include(${_BUILD_MODULE})
endforeach()


