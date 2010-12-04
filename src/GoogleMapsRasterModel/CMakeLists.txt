kompas_add_plugin(GoogleMapsRasterModel ${KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR} GoogleMapsRasterModel.conf GoogleMapsRasterModel.cpp)

if(BUILD_TESTS)
    enable_testing()
    add_library(GoogleMapsRasterModelTestLib GoogleMapsRasterModel.cpp)
    add_subdirectory(Test)
endif()
