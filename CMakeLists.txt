cmake_minimum_required (VERSION 2.8.8)

project( storyteller )

set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}" )

set(storyteller_MAJOR_VERSION 0)
set(storyteller_MINOR_VERSION 2)
set(storyteller_SUB_VERSION 0)
set(storyteller_VERSION ${storyteller_MAJOR_VERSION}.${storyteller_MINOR_VERSION}.${storyteller_SUB_VERSION})
set(VES_PACKAGE_VERSION ${storyteller_VERSION})

set( POCO_ROOT "Please specify" CACHE PATH "Root directory where POCO is installed" )
find_package(POCO 1.4.1 REQUIRED COMPONENTS PocoFoundation PocoUtil PocoZip)

#option(BUILD_EXAMPLES "Build examples?" ON)

INCLUDE_DIRECTORIES(${POCO_INCLUDE_DIR})

include(VESSetupCompiler)

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

include(VESInstallPDBFiles)

set(VES_EXPORT_LIBRARY_TARGETS storyteller)
set(VES_CONFIGURE_VARS CMAKE_INSTALL_LIBDIR)

#
# Setup the fpc auto detection files
if(WIN32)
    set(storyteller_FPC_LIBRARIES "/libpath:\"\${libdir}\" storyteller.lib")
    set(storyteller_FPC_INCLUDES "/I\"\${includedir}\"")
else(WIN32)
    set(storyteller_FPC_INCLUDES "-I\${includedir}")
    set(storyteller_FPC_LIBRARIES "-L\${libdir} -lstoryteller")
endif(WIN32)

include(VESInstallAutoDiscoveryTools)
