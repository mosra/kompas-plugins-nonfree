kompas_add_plugin(KontaminaceCeniaCzRasterModel ${KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR} KontaminaceCeniaCzRasterModel.conf KontaminaceCeniaCzRasterModel.cpp)

if(WIN32)
    target_link_libraries(KontaminaceCeniaCzRasterModel ${KOMPAS_CORE_LIBRARY})
endif()
