# - Convenience include for using wxWidgets library
# Finds if wxWidgets is installed 
# and set the appropriate libs, incdirs, flags etc. 
# INCLUDE_DIRECTORIES, LINK_DIRECTORIES and ADD_DEFINITIONS
# are called.
#
# USAGE
#  SET( wxWidgets_USE_LIBS  gl xml xrc ) # optionally: more than wx std libs
#  FIND_PACKAGE(wxWidgets REQUIRED)
#  INCLUDE( ${xWidgets_USE_FILE} )
#  ... add your targets here, e.g. ADD_EXECUTABLE/ ADD_LIBRARY ...
#  TARGET_LINK_LIBRARIERS( <yourWxDependantTarget>  ${wxWidgets_LIBRARIES})
# 
# DEPRECATED
#  LINK_LIBRARIES is not called in favor of adding dependencies per target.
#
# AUTHOR
#  Jan Woetzel <jw -at- mip.informatik.uni-kiel.de>


MSG("wxWidgets_FOUND=${wxWidgets_FOUND}")
IF(wxWidgets_FOUND)
  IF(wxWidgets_INCLUDE_DIRS)
    IF(wxWidgets_INCLUDE_DIRS_NO_SYSTEM)
      INCLUDE_DIRECTORIES(${wxWidgets_INCLUDE_DIRS})
    ELSE(wxWidgets_INCLUDE_DIRS_NO_SYSTEM)
      INCLUDE_DIRECTORIES(SYSTEM ${wxWidgets_INCLUDE_DIRS})
    ENDIF(wxWidgets_INCLUDE_DIRS_NO_SYSTEM)
    MSG("wxWidgets_INCLUDE_DIRS=${wxWidgets_INCLUDE_DIRS}")
  ENDIF(wxWidgets_INCLUDE_DIRS)

  IF(wxWidgets_LIBRARY_DIRS)
    LINK_DIRECTORIES(${wxWidgets_LIBRARY_DIRS})
    MSG("wxWidgets_LIBRARY_DIRS=${wxWidgets_LIBRARY_DIRS}")
  ENDIF(wxWidgets_LIBRARY_DIRS)

  IF(wxWidgets_DEFINITIONS)
    ADD_DEFINITIONS( ${wxWidgets_DEFINITIONS} )
    MSG("wxWidgets_DEFINITIONS=${wxWidgets_DEFINITIONS}")
  ENDIF(wxWidgets_DEFINITIONS)  

  IF(wxWidgets_CXX_FLAGS)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${wxWidgets_CXX_FLAGS}")
    MSG("wxWidgets_CXX_FLAGS=${wxWidgets_CXX_FLAGS}")
  ENDIF(wxWidgets_CXX_FLAGS) 
ELSE(wxWidgets_FOUND)
  MESSAGE("wxWidgets requested but not found.")
ENDIF(wxWidgets_FOUND)
