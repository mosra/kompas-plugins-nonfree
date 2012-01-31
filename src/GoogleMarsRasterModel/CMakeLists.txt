corrade_add_plugin(GoogleMarsRasterModel ${KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR} GoogleMarsRasterModel.conf GoogleMarsRasterModel.cpp)

if(WIN32)
    target_link_libraries(GoogleMarsRasterModel ${KOMPAS_CORE_LIBRARY})
endif()
