# - Find a storyteller installation or build tree.
# The following variables are set if storyteller is found.  If storyteller is not
# found, storyteller_FOUND is set to false.
#  storyteller_FOUND         - Set to true when storyteller is found.
#  storyteller_USE_FILE      - CMake file to use storyteller.
#  storyteller_MAJOR_VERSION - The storyteller major version number.
#  storyteller_MINOR_VERSION - The storyteller minor version number 
#                       (odd non-release).
#  storyteller_BUILD_VERSION - The storyteller patch level 
#                       (meaningless for odd minor).
#  storyteller_INCLUDE_DIRS  - Include directories for storyteller
#  storyteller_LIBRARY_DIRS  - Link directories for storyteller libraries

# The following cache entries must be set by the user to locate storyteller:
#  storyteller_DIR  - The directory containing storytellerConfig.cmake.  
#             This is either the root of the build tree,
#             or the lib directory.  This is the 
#             only cache entry.


# Assume not found.
SET(storyteller_FOUND 0)

# Construct consitent error messages for use below.
SET(storyteller_DIR_DESCRIPTION "directory containing storytellerConfig.cmake.  This is either the root of the build tree, or PREFIX/lib for an installation.")
SET(storyteller_DIR_MESSAGE "storyteller not found.  Set the storyteller_DIR cmake cache entry to the ${storyteller_DIR_DESCRIPTION}")

# Use the Config mode of the find_package() command to find storytellerConfig.
# If this succeeds (possibly because storyteller_DIR is already set), the
# command will have already loaded storytellerConfig.cmake and set storyteller_FOUND.
IF(NOT storyteller_FOUND)
  FIND_PACKAGE(storyteller QUIET NO_MODULE)
ENDIF(NOT storyteller_FOUND)

#-----------------------------------------------------------------------------
IF(NOT storyteller_FOUND)
  # storyteller not found, explain to the user how to specify its location.
  IF(storyteller_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR ${storyteller_DIR_MESSAGE})
  ELSE(storyteller_FIND_REQUIRED)
    IF(NOT storyteller_FIND_QUIETLY)
      MESSAGE(STATUS ${storyteller_DIR_MESSAGE})
    ENDIF(NOT storyteller_FIND_QUIETLY)
  ENDIF(storyteller_FIND_REQUIRED)
ENDIF(NOT storyteller_FOUND)
