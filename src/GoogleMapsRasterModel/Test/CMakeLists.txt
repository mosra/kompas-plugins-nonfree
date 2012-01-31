configure_file(${CMAKE_CURRENT_SOURCE_DIR}/testConfigure.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/testConfigure.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# TODO: CMake complains here about wrong order of DEBUG / RELEASE keywords
corrade_add_test(GoogleMapsRasterModelTest GoogleMapsRasterModelTest.h GoogleMapsRasterModelTest.cpp ${QT_QTNETWORK_LIBRARY} KompasCore GoogleMapsRasterModelTestLib)
target_link_libraries(GoogleMapsRasterModelTest ${CORRADE_UTILITY_LIBRARY} ${CORRADE_PLUGINMANAGER_LIBRARY})
