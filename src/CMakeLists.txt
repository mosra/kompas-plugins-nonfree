add_definitions(-Wall -pedantic)

set(KOMPAS_PLUGINS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} ${KOMPAS_PLUGINS_INCLUDE_DIR})
include_directories(${KOMPAS_CORE_INCLUDE_DIR} ${KOMPAS_PLUGINS_INCLUDE_DIR})

if(QT_PLUGINS OR BUILD_TESTS)
    include_directories(${QT_INCLUDE_DIR})
endif()

add_subdirectory(BingMapsRasterModel)
add_subdirectory(CelestialMap1675RasterModel)
add_subdirectory(GoogleMapsRasterModel)
add_subdirectory(KontaminaceCeniaCzRasterModel)
add_subdirectory(MapyCzRasterModel)
add_subdirectory(OviMapsRasterModel)
add_subdirectory(YahooMapsRasterModel)

if(BUILD_TESTS)
    enable_testing()
endif()
