#corrade_add_plugin(OviMapsRasterModel ${KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR} OviMapsRasterModel.conf OviMapsRasterModel.cpp)

if(WIN32)
    #target_link_libraries(OviMapsRasterModel ${KOMPAS_CORE_LIBRARY})
endif()

if(BUILD_TESTS)
    enable_testing()
    add_library(OviMapsRasterModelTestLib OviMapsRasterModel.cpp)
    target_link_libraries(OviMapsRasterModelTestLib ${CORRADE_UTILITY_LIBRARY} ${CORRADE_PLUGINMANAGER_LIBRARY})
    add_subdirectory(Test)
endif()
