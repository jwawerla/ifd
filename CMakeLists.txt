
PROJECT(ifd)

cmake_minimum_required(VERSION 2.6)

#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE OFF)

INCLUDE(FindPkgConfig)

#add definitions, compiler switches, etc.
ADD_DEFINITIONS(-g -Wall -O2)

PKG_SEARCH_MODULE( STAGE REQUIRED stage )

IF( STAGE_FOUND )
  MESSAGE( STATUS "STAGE_LIBRARY_DIRS ${STAGE_LIBRARY_DIRS}" )
  MESSAGE( STATUS "STAGE_LIBRARIES ${STAGE_LIBRARIES}" )
  #MESSAGE( STATUS "STAGE_LDFLAGS ${STAGE_LDFLAGS}" )
  MESSAGE( STATUS "STAGEDIR ${STAGE_INCLUDE_DIRS}" )

  # do some work to cope with frameworks on OS X
  STRING(REGEX REPLACE ";" " " STAGE_LDFLAGS_MORE "${STAGE_LDFLAGS_OTHER}")
  MESSAGE( STATUS STAGE_LDFLAGS_MORE " ${STAGE_LDFLAGS_MORE}" )

ELSE( STAGE_FOUND )
  MESSAGE(  "Stage was not found" )
ENDIF( STAGE_FOUND )

PKG_SEARCH_MODULE( RAPI REQUIRED autolab-rapi-stage )

IF (RAPI_FOUND)
  MESSAGE(STATUS "autolab-rapi-stage version ${RAPI_VERSION} detected at ${RAPI_PREFIX}" )
ELSE (RAPI_FOUND)
  MESSAGE(STATUS "autolab-rapi-stage not detected")
ENDIF (RAPI_FOUND)

PKG_SEARCH_MODULE( RAPI_GUI autolab-rapi-gui )

IF (RAPI_GUI_FOUND)
  MESSAGE(STATUS "autolab-rapi-gui version ${RAPI_GUI_VERSION} detected at ${RAPI_GUI_PREFIX}" )
  MESSAGE(STATUS "cflags ${RAPI_GUI_CFLAGS_OTHER}")
ELSE (RAPI_GUI_FOUND)
  MESSAGE(STATUS "autolab-rapi-gui not detected")
ENDIF (RAPI_GUI_FOUND)


FIND_PACKAGE(Qt4 REQUIRED)
IF (QT_FOUND)
  INCLUDE(${QT_USE_FILE})
  ADD_DEFINITIONS( ${QT_DEFINITIONS} )
ELSEIF(QT_FOUND)
  MESSAGE(STATUS "Qt4 not detected")
ENDIF(QT_FOUND)

INCLUDE_DIRECTORIES( ./src
                     ${RAPI_INCLUDE_DIRS}
                     ${RAPI_GUI_INCLUDE_DIRS}
                     ${STAGE_INCLUDE_DIRS}
                     ${QT_INCLUDE_DIR}
)

LINK_DIRECTORIES( ${STAGE_LIBRARY_DIRS}
                  ${RAPI_LIBRARY_DIRS}
                  ${RAPI_GUI_LIBRARY_DIRS}
                  ${QT_LIBRARY_DIR}
)


ADD_SUBDIRECTORY(src)
