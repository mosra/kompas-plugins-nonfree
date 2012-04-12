corrade_add_plugin(GoogleMoonRasterModel ${KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR} GoogleMoonRasterModel.conf GoogleMoonRasterModel.cpp)

if(WIN32)
    target_link_libraries(GoogleMoonRasterModel ${KOMPAS_CORE_LIBRARY})
endif()

if(BUILD_TESTS)
    enable_testing()
    add_library(GoogleMoonRasterModelTestLib GoogleMoonRasterModel.cpp)
    target_link_libraries(GoogleMoonRasterModelTestLib ${CORRADE_UTILITY_LIBRARY} ${CORRADE_PLUGINMANAGER_LIBRARY})
    add_subdirectory(Test)
endif()
