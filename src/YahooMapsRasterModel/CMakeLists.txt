corrade_add_plugin(YahooMapsRasterModel ${KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR} YahooMapsRasterModel.conf YahooMapsRasterModel.cpp)

if(WIN32)
    target_link_libraries(YahooMapsRasterModel ${KOMPAS_CORE_LIBRARY})
endif()
