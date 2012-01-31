configure_file(${CMAKE_CURRENT_SOURCE_DIR}/testConfigure.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/testConfigure.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

corrade_add_test(MapyCzRasterModelTest MapyCzRasterModelTest.h MapyCzRasterModelTest.cpp KompasCore MapyCzRasterModelTestLib)
target_link_libraries(MapyCzRasterModelTest ${CORRADE_UTILITY_LIBRARY} ${CORRADE_PLUGINMANAGER_LIBRARY})
