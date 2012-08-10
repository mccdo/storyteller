#This script requires these variables be defined:
# TARGET_NAME       = The name of the library or application
# TARGET_VERSION    = The version of the library or application
# TARGET_EXPORT     = The export target tag
# TARGET_CATEGORY   = This can be: Lib, App, Plugin, Test, Example
# PUBLIC_HEADERS    = Headers to be installed

set_target_properties( ${VES_TARGET_NAME} PROPERTIES VERSION ${VES_TARGET_VERSION} )
set_target_properties( ${VES_TARGET_NAME} PROPERTIES SOVERSION ${VES_TARGET_VERSION} )
set_target_properties( ${VES_TARGET_NAME} PROPERTIES PROJECT_LABEL "${VES_TARGET_CATEGORY} ${VES_TARGET_NAME}" )

install(TARGETS ${VES_TARGET_NAME}
        EXPORT ${VES_TARGET_EXPORT}
        RUNTIME DESTINATION ${VES_INSTALL_BINDIR}
        LIBRARY DESTINATION ${VES_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${VES_INSTALL_ARCHIVEDIR})

install(FILES       ${VES_PUBLIC_HEADERS}
        DESTINATION ${VES_INSTALL_INCDIR}/${VES_TARGET_NAME})
