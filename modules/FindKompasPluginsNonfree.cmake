# FindKompasPluginsNonfree - Kompas nonfree plugins handling module for CMake
#
# This module depends on Kompas plugins and additionaly defines:
#
# KOMPASPLUGINSNONFREE_FOUND       - True if Kompas plugins were found
#

find_package(KompasCore REQUIRED)
find_package(KompasPlugins REQUIRED)

if (NOT KOMPASPLUGINSNONFREE_FOUND)
    # Paths
    find_path(_KOMPAS_PLUGINS_NONFREE_INCLUDE_DIR
        NAMES GoogleMapsRasterModel
        PATH_SUFFIXES Kompas/Plugins
    )

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args("KompasPluginsNonfree" DEFAULT_MSG
        _KOMPAS_PLUGINS_NONFREE_INCLUDE_DIR
    )

endif()
