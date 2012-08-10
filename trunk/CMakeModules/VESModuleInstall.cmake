#This script requires these variables be defined:
# TARGET_NAME       = The name of the library or application
# TARGET_VERSION    = The version of the library or application
# TARGET_EXPORT     = The export target tag
# TARGET_CATEGORY   = This can be: Lib, App, Plugin, Test, Example
# PUBLIC_HEADERS    = Headers to be installed

set_target_properties( ${TARGET_NAME} PROPERTIES VERSION ${TARGET_VERSION} )
set_target_properties( ${TARGET_NAME} PROPERTIES SOVERSION ${TARGET_VERSION} )
set_target_properties( ${TARGET_NAME} PROPERTIES PROJECT_LABEL "${TARGET_CATEGORY} ${TARGET_NAME}" )

install(TARGETS ${TARGET_NAME}
        EXPORT ${TARGET_EXPORT}
        RUNTIME DESTINATION ${VES_INSTALL_BINDIR}
        LIBRARY DESTINATION ${VES_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${VES_INSTALL_ARCHIVEDIR})

install(FILES       ${PUBLIC_HEADERS}
        DESTINATION ${VES_INSTALL_INCDIR}/${TARGET_NAME})
