#Lets us address our internal includes as <storyteller/foo.h>
include_directories(${CMAKE_SOURCE_DIR}/src)

ADD_DEFINITIONS(-DSTORYTELLER_LIBRARY)

FILE( GLOB TARGET_SRC "*.cxx" )
FILE( GLOB hdrs "*.h" )

set(VES_PRIVATE_HEADERS ${hdrs})
set(VES_PUBLIC_HEADERS PresentationMaker.h Exports.h )

set(VES_TARGET_NAME storyteller)
set(VES_TARGET_VERSION ${storyteller_VERSION})
set(VES_TARGET_EXPORT ${CMAKE_PROJECT_NAME}-targets)
set(VES_TARGET_CATEGORY Lib)

ADD_LIBRARY(${VES_TARGET_NAME} SHARED ${TARGET_SRC} ${VES_PUBLIC_HEADERS})
TARGET_LINK_LIBRARIES(${VES_TARGET_NAME} ${POCO_LIBRARIES})

include(VESModuleInstall)
