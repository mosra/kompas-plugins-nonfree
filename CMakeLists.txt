cmake_minimum_required(VERSION 2.6)

project(KompasPluginsNonfree)

option(BUILD_TESTS "Build unit tests (requires Qt4)." OFF)
option(QT_PLUGINS "Build also plugins for Kompas-Qt (requires Qt4)." ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${KompasPluginsNonfree_SOURCE_DIR}/modules/")

find_package(KompasCore REQUIRED)
find_package(KompasPlugins REQUIRED)

if(BUILD_TESTS)
    find_package(Qt4)

    if(NOT QT4_FOUND)
        message(WARNING "Qt4 is required for building unit tests. No tests will be build.")
        set(BUILD_TESTS OFF)
    else()
        enable_testing()
    endif()

endif()

if(QT_PLUGINS)
    find_package(Qt4)
    find_package(KompasQt)

    if(NOT QT4_FOUND OR NOT KOMPASQT_FOUND)
        message(WARNING "Qt4 and Kompas Qt GUI is required for building Qt-specific plugins. No Qt-specific plugins will be build.")
        set(QT_PLUGINS OFF)
    endif()
endif()

add_subdirectory(modules)
add_subdirectory(src)

if(NOT WIN32)
    install(DIRECTORY src/ DESTINATION ${KOMPAS_PLUGINS_INCLUDE_INSTALL_DIR}
                FILES_MATCHING PATTERN "*.h"
                PATTERN "*/Test" EXCLUDE)
endif()
