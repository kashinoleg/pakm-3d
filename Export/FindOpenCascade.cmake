#
# Find the OpenCascade includes and library
#
# This module defines
# OPENCASCADE_INCLUDE_DIR, where to find headers
# OPENCASCADE_LIBRARY_DIR, where to find the OpenCascade library.
# OPENCASCADE_DEBUG_LIBRARY_DIR, where to find the OpenCascade library in debug mode.
# OPENCASCADE_FOUND, If false, do not try to use OpenCascade.

FIND_PATH(OPENCASCADE_INCLUDE_DIR gp.hxx
  /usr/local/include
  /usr/include
)


SET(OPENCASCADE_LIBRARIES_NAME
	TKBin
  TKBinL
  TKBinTObj
  TKBinXCAF
  TKBO
  TKBool
  TKBRep
  TKCAF
  TKCDF
  TKDCAF
  TKDraw
  TKernel
  TKFeat
  TKFillet
  TKG2d
  TKG3d
  TKGeomAlgo
  TKGeomBase
  TKHLR
  TKIGES
  TKLCAF
  TKMath
  TKMesh
  TKMeshVS
  TKOffset
  TKOpenGl
  TKPrim
  TKQADraw
  TKService
  TKShHealing
  TKStd
  TKStdL
  TKSTEP
  TKSTEP209
  TKSTEPAttr
  TKSTEPBase
  TKSTL
  TKTObj
  TKTObjDRAW
  TKTopAlgo
  TKTopTest
  TKV3d
  TKVCAF
  TKViewerTest
  TKVRML
  TKXCAF
  TKXDEDRAW
  TKXDEIGES
  TKXDESTEP
  TKXMesh
  TKXml
  TKXmlL
  TKXmlTObj
  TKXmlXCAF
  TKXSBase
  TKXSDRAW
)

FIND_PATH(OPENCASCADE_DIR ${OPENCASCADE_LIBRARIES_NAME}
  lib
  bin
)

IF(OPENCASCADE_INCLUDE_DIR)
  IF(OPENCASCADE_DIR)
    foreach (name ${OPENCASCADE_LIBRARIES_NAME})
      SET(OPENCASCADE_STATIC_LIB
        ${OPENCASCADE_STATIC_LIB}
        ${OPENCASCADE_DIR}/lib/${name}.lib
      )
      SET(OPENCASCADE_SHARED_LIB
        ${OPENCASCADE_SHARED_LIB}
        ${OPENCASCADE_DIR}/bin/${name}.dll
      )
    endforeach()
    SET(OPENCASCADE_FOUND "YES")
  ENDIF()
ENDIF(OPENCASCADE_INCLUDE_DIR)
