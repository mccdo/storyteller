#
# Create and install all of the auto find tools
# =============================================

# Add all targets to the build-tree export set
export(TARGETS ${VES_EXPORT_LIBRARY_TARGETS}
  FILE "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/${CMAKE_PROJECT_NAME}LibraryDepends.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE ${CMAKE_PROJECT_NAME})

# Create a storytellerBuildTreeSettings.cmake file for the use from the build tree
configure_file(${PROJECT_SOURCE_DIR}/CMakeModules/packageBuildTreeSettings.cmake.in
    "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/${CMAKE_PROJECT_NAME}BuildTreeSettings.cmake" @ONLY)
configure_file(${PROJECT_SOURCE_DIR}/CMakeModules/packageConfigVersion.cmake.in
    "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake" @ONLY)
configure_file(${PROJECT_SOURCE_DIR}/CMakeModules/Usepackage.cmake.in
    "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/Use${CMAKE_PROJECT_NAME}.cmake" @ONLY)

include(CMakePackageConfigHelpers)
# Create the storytellerConfig.cmake and storytellerConfigVersion files
configure_package_config_file( 
    ${PROJECT_SOURCE_DIR}/CMakeModules/packageConfig.cmake.in 
        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/${CMAKE_PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PATH_VARS ${VES_CONFIGURE_VARS})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/${CMAKE_PROJECT_NAME}Config.cmake 
              ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/Use${CMAKE_PROJECT_NAME}.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT development)

# Install the CMake find script for storyteller
configure_file(${PROJECT_SOURCE_DIR}/CMakeModules/Findpackage.cmake.in
    "${PROJECT_BINARY_DIR}/Find${CMAKE_PROJECT_NAME}.cmake" @ONLY)
install(FILES "${PROJECT_BINARY_DIR}/Find${CMAKE_PROJECT_NAME}.cmake"
    DESTINATION "share/${CMAKE_PROJECT_NAME}/extra"
    COMPONENT development)
    #RENAME Find${CMAKE_PROJECT_NAME}.cmake)

# Install the export set for use with the install-tree
install(EXPORT ${CMAKE_PROJECT_NAME}-targets DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development)

configure_file(extra/${CMAKE_PROJECT_NAME}.fpc.in
  "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/flagpoll/${CMAKE_PROJECT_NAME}.fpc" @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/flagpoll/${CMAKE_PROJECT_NAME}.fpc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/flagpoll )