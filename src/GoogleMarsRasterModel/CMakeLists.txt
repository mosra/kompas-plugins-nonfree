corrade_add_plugin(GoogleMarsRasterModel ${KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR} GoogleMarsRasterModel.conf GoogleMarsRasterModel.cpp)

if(WIN32)
    target_link_libraries(GoogleMarsRasterModel ${KOMPAS_CORE_LIBRARY})
endif()

if(BUILD_TESTS)
    enable_testing()
    add_library(GoogleMarsRasterModelTestLib GoogleMarsRasterModel.cpp)
    target_link_libraries(GoogleMarsRasterModelTestLib ${CORRADE_UTILITY_LIBRARY} ${CORRADE_PLUGINMANAGER_LIBRARY})
    add_subdirectory(Test)
endif()
