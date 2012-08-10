#Lets us address our internal includes as <snap/foo.h>
include_directories(${CMAKE_SOURCE_DIR}/src)

ADD_DEFINITIONS(-DSNAP_LIBRARY)

FILE( GLOB TARGET_SRC "*.cxx" )
#FILE( GLOB hdrs "*.h" )

SET(VES_PUBLIC_HEADERS PresentationMaker.h Exports.h )

set(VES_TARGET_NAME snap)
set(VES_TARGET_VERSION ${storyteller_VERSION})
set(VES_TARGET_EXPORT storyteller-targets)
set(VES_TARGET_CATEGORY Lib)

ADD_LIBRARY(${VES_TARGET_NAME} SHARED ${TARGET_SRC} ${VES_PUBLIC_HEADERS})
TARGET_LINK_LIBRARIES(${VES_TARGET_NAME} ${snap_LIBS} ${POCO_LIBRARIES})

include(VESModuleInstall)
