kompas_add_plugin(GoogleMapsRasterModel ${KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR} GoogleMapsRasterModel.conf GoogleMapsRasterModel.cpp)

if(WIN32)
    target_link_libraries(GoogleMapsRasterModel ${KOMPAS_CORE_LIBRARIES})
endif()

if(BUILD_TESTS)
    enable_testing()
    add_library(GoogleMapsRasterModelTestLib GoogleMapsRasterModel.cpp)
    add_subdirectory(Test)
endif()
