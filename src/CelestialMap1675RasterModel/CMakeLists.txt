corrade_add_plugin(CelestialMap1675RasterModel ${KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR} CelestialMap1675RasterModel.conf CelestialMap1675RasterModel.cpp)

if(WIN32)
    target_link_libraries(CelestialMap1675RasterModel ${KOMPAS_CORE_LIBRARY} StereographicProjection)
endif()
