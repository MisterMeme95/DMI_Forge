# Load the debug and release variables
get_filename_component(_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
file(GLOB DATA_FILES "${_DIR}/tmxlite-*-data.cmake")

foreach(f ${DATA_FILES})
    include(${f})
endforeach()

# Create the targets for all the components
foreach(_COMPONENT ${tmxlite_COMPONENT_NAMES} )
    if(NOT TARGET ${_COMPONENT})
        add_library(${_COMPONENT} INTERFACE IMPORTED)
        message(${tmxlite_MESSAGE_MODE} "Conan: Component target declared '${_COMPONENT}'")
    endif()
endforeach()

if(NOT TARGET tmxlite::tmxlite)
    add_library(tmxlite::tmxlite INTERFACE IMPORTED)
    message(${tmxlite_MESSAGE_MODE} "Conan: Target declared 'tmxlite::tmxlite'")
endif()
# Load the debug and release library finders
get_filename_component(_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
file(GLOB CONFIG_FILES "${_DIR}/tmxlite-Target-*.cmake")

foreach(f ${CONFIG_FILES})
    include(${f})
endforeach()