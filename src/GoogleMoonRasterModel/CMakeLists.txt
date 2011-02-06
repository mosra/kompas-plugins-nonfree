kompas_add_plugin(GoogleMoonRasterModel ${KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR} GoogleMoonRasterModel.conf GoogleMoonRasterModel.cpp)

if(WIN32)
    target_link_libraries(GoogleMoonRasterModel ${KOMPAS_CORE_LIBRARY})
endif()
