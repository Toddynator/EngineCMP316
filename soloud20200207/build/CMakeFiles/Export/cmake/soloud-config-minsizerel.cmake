#----------------------------------------------------------------
# Generated CMake target import file for configuration "MinSizeRel".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "soloud" for configuration "MinSizeRel"
set_property(TARGET soloud APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
set_target_properties(soloud PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_MINSIZEREL "C;CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_MINSIZEREL "C:/Users/toddy/Downloads/SDL2-2.32.10/lib/x64/SDL2.lib"
  IMPORTED_LOCATION_MINSIZEREL "${_IMPORT_PREFIX}/lib/soloud.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS soloud )
list(APPEND _IMPORT_CHECK_FILES_FOR_soloud "${_IMPORT_PREFIX}/lib/soloud.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
