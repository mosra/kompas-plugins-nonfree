configure_file(${CMAKE_CURRENT_SOURCE_DIR}/testConfigure.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/testConfigure.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

map2x_add_test(MapyCzRasterModelTest MapyCzRasterModelTest.h MapyCzRasterModelTest.cpp Map2XCore MapyCzRasterModelTestLib)
