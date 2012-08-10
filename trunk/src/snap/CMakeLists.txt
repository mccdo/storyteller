#Lets us address our internal includes as <snap/foo.h>
include_directories(
        ${CMAKE_SOURCE_DIR}/src
    )

ADD_DEFINITIONS(-DSNAP_LIBRARY)

FILE( GLOB srcs "*.cxx" )
#FILE( GLOB hdrs "*.h" )

SET(TARGET_SRC ${srcs})
SET(PUBLIC_HEADERS PresentationMaker.h Exports.h )

set(TARGET_NAME snap)
set(TARGET_VERSION ${storyteller_VERSION})
set(TARGET_EXPORT storyteller-targets)
set(TARGET_CATEGORY Lib)

ADD_LIBRARY(${TARGET_NAME} SHARED ${TARGET_SRC} ${PUBLIC_HEADERS})
TARGET_LINK_LIBRARIES(${TARGET_NAME} ${snap_LIBS} ${POCO_LIBRARIES})

include(VESModuleInstall)
