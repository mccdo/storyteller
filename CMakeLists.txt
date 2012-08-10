cmake_minimum_required (VERSION 2.8.8)

project( storyteller )

set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}" )

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

include(VESBinAndLibDirs)

add_subdirectory(src)

#
# Doxygen
#find_package( Doxygen )
#if( DOXYGEN_FOUND )
#    set( HAVE_DOT "NO" )
#    if( DOXYGEN_DOT_PATH )
#        set( HAVE_DOT "YES" )
#    endif()
#    
#    configure_file( ${PROJECT_SOURCE_DIR}/doc/doxyfile.cmake
#        ${PROJECT_BINARY_DIR}/doc/doxyfile
#    )
#    add_custom_target( Documentation ${DOXYGEN_EXECUTABLE}
#        ${PROJECT_BINARY_DIR}/doc/doxyfile
#    )
#    
#    add_subdirectory( doc/images )
#endif()

# On windows provide the user with the pdb files 
# for debugging if they are present
if(MSVC)
    install(
        DIRECTORY ${PROJECT_BINARY_DIR}/lib/\${CMAKE_INSTALL_CONFIG_NAME}/
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        USE_SOURCE_PERMISSIONS
        COMPONENT Development
        FILES_MATCHING PATTERN "*.pdb"
    )
endif(MSVC)

# Install the CMake find script for storyteller
install( FILES "extra/Findstoryteller.cmake"
    DESTINATION "share/${CMAKE_PROJECT_NAME}/extra"
    COMPONENT Development
)

#
# Create and install all of the auto find tools
# =============================================

# Add all targets to the build-tree export set
export(TARGETS snap
  FILE "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/storytellerLibraryDepends.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE storyteller)

# Create a storytellerBuildTreeSettings.cmake file for the use from the build tree
configure_file(storytellerBuildTreeSettings.cmake.in
  "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/storytellerBuildTreeSettings.cmake" @ONLY)

#configure_file(storytellerConfig.cmake.in
#  "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/storytellerConfig.cmake" @ONLY)
configure_file(storytellerConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/storytellerConfigVersion.cmake" @ONLY)
configure_file(Usestoryteller.cmake.in
  "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/Usestoryteller.cmake" @ONLY)

# Install the export set for use with the install-tree
install(EXPORT storyteller-targets DESTINATION ${CMAKE_INSTALL_LIBDIR})

include( CMakePackageConfigHelpers )
# Create the storytellerConfig.cmake and storytellerConfigVersion files
file(RELATIVE_PATH CONF_REL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
   "${CMAKE_INSTALL_PREFIX}/include")

configure_package_config_file( 
    storytellerConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/storytellerConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PATH_VARS CONF_REL_INCLUDE_DIR CMAKE_INSTALL_LIBDIR)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/storytellerConfig.cmake 
              ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/storytellerConfigVersion.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/Usestoryteller.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR} )

#
# Setup the fpc auto detection files
if(WIN32)
    set(storyteller_FPC_LIBRARIES "/libpath:\"\${libdir}\" snap.lib")
    set(storyteller_FPC_INCLUDES "/I\"\${includedir}\"")
else(WIN32)
    set(storyteller_FPC_INCLUDES "-I\${includedir}")
    set(storyteller_FPC_LIBRARIES "-L\${libdir} -lsnap")
endif(WIN32)

configure_file(extra/storyteller.fpc.in
  "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/flagpoll/storyteller.fpc" @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/flagpoll/storyteller.fpc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/flagpoll )