
LINK_DIRECTORIES( ${STAGE_LIBRARY_DIRS} )

set_source_files_properties( main.cpp PROPERTIES COMPILE_FLAGS "${FLTK_CFLAGS}" )

#build a shared library
ADD_LIBRARY(ifd MODULE
  main.cpp
  baserobotctrl.cpp
  pathplannerlookup.cpp
  patch.cpp
)

IF (QT_FOUND)
  ADD_DEFINITIONS(-DGUI)
  SET( FASRGUI_LIBRARIES
    ${RAPI_GUI_LIBRARIES}
  )
ENDIF (QT_FOUND)

# delete the "lib" prefix from the plugin libraries
SET_TARGET_PROPERTIES( ifd
  PROPERTIES PREFIX ""
  LINK_FLAGS "${STAGE_LDFLAGS_MORE}"
)

MESSAGE(STATUS "RAPI_LIBRARIES ${RAPI_LIBRARIES}")

#need to link to some other libraries ? just add them here
TARGET_LINK_LIBRARIES( ifd
  ${STAGE_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${RAPI_LIBRARIES}
  ${FASRGUI_LIBRARIES}
  ${QT_LIBRARIES}
  ${ND_LIBRARIES}
)

INSTALL(TARGETS ifd
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
)