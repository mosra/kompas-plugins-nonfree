map2x_add_plugin(MapyCzRasterModel ${MAP2X_PLUGINS_RASTERMODEL_INSTALL_DIR} MapyCzRasterModel.conf MapyCzRasterModel.cpp)

if(BUILD_TESTS)
    enable_testing()
    add_library(MapyCzRasterModelTestLib MapyCzRasterModel.cpp)
    add_subdirectory(Test)
endif()
