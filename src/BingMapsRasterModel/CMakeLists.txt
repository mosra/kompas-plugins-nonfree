corrade_add_plugin(BingMapsRasterModel ${KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR} BingMapsRasterModel.conf BingMapsRasterModel.cpp)

if(WIN32)
    target_link_libraries(BingMapsRasterModel ${KOMPAS_CORE_LIBRARY})
endif()

if(BUILD_TESTS)
    enable_testing()
    add_library(BingMapsRasterModelTestLib BingMapsRasterModel.cpp)
    target_link_libraries(BingMapsRasterModelTestLib ${CORRADE_UTILITY_LIBRARY} ${CORRADE_PLUGINMANAGER_LIBRARY})
    add_subdirectory(Test)
endif()
