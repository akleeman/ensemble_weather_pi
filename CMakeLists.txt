##---------------------------------------------------------------------------
## Author:      Alex Kleeman (template borrowed from Sean D'Epagnier)
## Copyright:
## License:     GPLv3+
##---------------------------------------------------------------------------

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)
cmake_policy(SET CMP0012 NEW)
#SET(CMAKE_VERBOSE_MAKEFILE ON)

PROJECT(ensemble_weather_pi)
SET(PACKAGE_NAME ensemble_weather_pi)
SET(VERBOSE_NAME Ensemble_Weather)
SET(TITLE_NAME ENSEMBLE_WEATHER)
SET(CPACK_PACKAGE_CONTACT "Alex Kleeman")

SET(BUNDLE_DATA TRUE)

INCLUDE( VERSION.cmake )

SET(PLUGIN_VERSION "${PLUGIN_VERSION_MAJOR}.${PLUGIN_VERSION_MINOR}.${PLUGIN_VERSION_PATCH}" )

INCLUDE("cmake/PluginConfigure.cmake")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS -std=gnu++11)

SET(SRC_ENSEMBLE_WEATHER
            src/ensemble_weather_pi.cpp
            src/WeatherRouting.cpp
            src/ConfigurationDialog.cpp
            src/SettingsDialog.cpp
            src/PlotDialog.cpp
            src/BoatDialog.cpp
            src/StatisticsDialog.cpp
            src/ReportDialog.cpp
            src/FilterRoutesDialog.cpp
            src/ConfigurationBatchDialog.cpp
            src/EditPolarDialog.cpp
            src/AboutDialog.cpp
            src/Polar.cpp
            src/Boat.cpp
            src/RouteMap.cpp
            src/RouteMapOverlay.cpp
            src/Utilities.cpp
            src/LineBufferOverlay.cpp
            src/PolygonRegion.cpp
            src/WeatherRoutingUI.cpp
            src/icons.cpp
            src/wrdc.cpp
            src/TexFont.cpp
            src/zuFile.cpp
            src/georef.c
            src/GribRecord.cpp
            # New Files
            src/VectorUtilities.cpp
            src/GriddedData.cpp
)

IF(WIN32)
SET(SRC_JSON
            src/wxJSON/jsonval.cpp
            src/wxJSON/jsonreader.cpp
            src/wxJSON/jsonwriter.cpp)
ENDIF(WIN32)

SET(SRC_LIBTESS2
  src/libtess2/bucketalloc.c
  src/libtess2/dict.c
  src/libtess2/geom.c
  src/libtess2/mesh.c
  src/libtess2/priorityq.c
  src/libtess2/sweep.c
  src/libtess2/tess.c)

IF(UNIX)
 INCLUDE("cmake/FindTinyXML.cmake")
 FIND_PACKAGE(TinyXML QUIET)
ENDIF(UNIX)

SET(SRC ${SRC_ENSEMBLE_WEATHER} ${SRC_JSON} ${SRC_LIBTESS2})

IF(TINYXML_FOUND)
 message (STATUS "Building with system tinyxml")
 INCLUDE_DIRECTORIES(${TINYXML_INCLUDE_DIR})
 ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC})
 TARGET_LINK_LIBRARIES(${PACKAGE_NAME} ${TINYXML_LIBRARIES})
ELSE(TINYXML_FOUND)
 message (STATUS "Building with embedded tinyxml")
 INCLUDE_DIRECTORIES(src/tinyxml/)
 SET(SRC_LTINYXML
            src/tinyxml/tinyxml.cpp
            src/tinyxml/tinyxmlparser.cpp
            src/tinyxml/tinyxmlerror.cpp
 )
 ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC} ${SRC_LTINYXML})
ENDIF(TINYXML_FOUND)
ADD_DEFINITIONS(-DTIXML_USE_STL)

IF(NOT UNIX)
    SET(SRC_BZIP
            src/bzip2/bzlib.c
            src/bzip2/blocksort.c
            src/bzip2/compress.c
            src/bzip2/crctable.c
            src/bzip2/decompress.c
            src/bzip2/huffman.c
            src/bzip2/randtable.c
    )
    ADD_LIBRARY(LIB_BZIP STATIC ${SRC_BZIP})
    TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} LIB_BZIP )

    INCLUDE_DIRECTORIES(${PLUGIN_SOURCE_DIR}/src/zlib-1.2.3)
    INCLUDE_DIRECTORIES(${PLUGIN_SOURCE_DIR}/src/bzip2)
ENDIF(NOT UNIX)

#IF(NOT UNIX)
#    INCLUDE_DIRECTORIES(src/zlib-1.2.3)
#    INCLUDE_DIRECTORIES(src/bzip2)
#ENDIF(NOT UNIX)

IF(MINGW)
    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} "-lwxzlib-2.8")
ENDIF(MINGW)

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")

add_subdirectory(tests)
