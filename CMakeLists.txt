cmake_minimum_required (VERSION 2.8.8)

project( storyteller )

SET( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules" )

set(storyteller_MAJOR_VERSION 0)
set(storyteller_MINOR_VERSION 2)
set(storyteller_SUB_VERSION 0)
set(storyteller_VERSION ${storyteller_MAJOR_VERSION}.${storyteller_MINOR_VERSION}.${storyteller_SUB_VERSION})


find_package(POCO 1.4.1 REQUIRED COMPONENTS PocoFoundation PocoUtil PocoZip)

#option(BUILD_EXAMPLES "Build examples?" ON)

INCLUDE_DIRECTORIES( 
    ${POCO_INCLUDE_DIR}
)

################################################################################
# Platform specific definitions
IF(WIN32)
    IF(MSVC)
        # To enable /MP, parralel build on MSVC
        OPTION(WIN32_USE_MP "Set to OFF to diable /MP." ON)
        MARK_AS_ADVANCED(WIN32_USE_MP)
        IF(WIN32_USE_MP)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        ENDIF(WIN32_USE_MP)

    ENDIF(MSVC)
ENDIF (WIN32)

IF(UNIX AND NOT WIN32 AND NOT APPLE)
  IF(CMAKE_SIZEOF_VOID_P MATCHES "8")
      SET(LIB_POSTFIX "64" CACHE STRING "suffix for 32/64 dir placement")
      MARK_AS_ADVANCED(LIB_POSTFIX)
  ENDIF()
ENDIF()
IF(NOT DEFINED LIB_POSTFIX)
    SET(LIB_POSTFIX "")
ENDIF()

################################################################################
# Create lib directory if required

IF("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
   FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
ENDIF("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")

################################################################################

SET(OUTPUT_LIBDIR ${PROJECT_BINARY_DIR}/lib)
MAKE_DIRECTORY(${OUTPUT_LIBDIR})
SET(LIBRARY_OUTPUT_PATH ${OUTPUT_LIBDIR})


add_subdirectory(src)

