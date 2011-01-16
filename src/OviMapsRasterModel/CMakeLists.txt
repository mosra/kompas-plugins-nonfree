kompas_add_plugin(OviMapsRasterModel ${KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR} OviMapsRasterModel.conf OviMapsRasterModel.cpp)

if(WIN32)
    target_link_libraries(OviMapsRasterModel ${KOMPAS_CORE_LIBRARY})
endif()
