#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "para::log" for configuration "Release"
set_property(TARGET para::log APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(para::log PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libpara_log.so.1.0.0"
  IMPORTED_SONAME_RELEASE "libpara_log.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS para::log )
list(APPEND _IMPORT_CHECK_FILES_FOR_para::log "${_IMPORT_PREFIX}/lib/libpara_log.so.1.0.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
