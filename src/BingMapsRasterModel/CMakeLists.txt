corrade_add_plugin(BingMapsRasterModel ${KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR} BingMapsRasterModel.conf BingMapsRasterModel.cpp)

if(WIN32)
    target_link_libraries(BingMapsRasterModel ${KOMPAS_CORE_LIBRARY})
endif()
