#ifndef Map2X_Plugins_GoogleMapsRasterModel_h
#define Map2X_Plugins_GoogleMapsRasterModel_h
/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Map2X.

    Map2X is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Map2X is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

/** @file
 * @brief Class Map2X::Plugins::GoogleMapsRasterModel
 */

#include "Map2XRasterModel/Map2XRasterModel.h"
#include "MercatorProjection/MercatorProjection.h"

namespace Map2X { namespace Plugins {

/** @brief Google Maps */
class GoogleMapsRasterModel: public Map2XRasterModel {
    public:
        /** @copydoc Plugins::Map2XRasterModel::Map2XRasterModel */
        GoogleMapsRasterModel(PluginManager::AbstractPluginManager* manager, const std::string& pluginName);

        inline virtual int features() const {
            return Map2XRasterModel::features()|LoadableFromUrl|NonConvertableFormat;
        }
        virtual SupportLevel recognizeFile(const std::string& filename, std::istream& file) const;
        inline virtual const Map2X::Core::AbstractProjection* projection() const
            { return &_projection; }
        inline virtual Core::TileSize tileSize() const
            { return Core::TileSize(256,256); }
        inline virtual double zoomStep() const
            { return 2; }
        inline virtual std::string copyright() const
            { return "© 2010 Google, Map Data © 2010 Tele Atlas."; }

        inline virtual std::vector<Core::Zoom> zoomLevels() const {
            return online() ? zoomLevelsOnline : Map2XRasterModel::zoomLevels();
        }
        inline virtual Core::TileArea area() const {
            return online() ? areaOnline : Map2XRasterModel::area();
        }
        virtual std::vector<std::string> layers() const {
            return online() ? layersOnline : Map2XRasterModel::layers();
        }
        virtual std::vector<std::string> overlays() const {
            return online() ? overlaysOnline : Map2XRasterModel::overlays();
        }

        /**
         * Server number and Galielo secure string algorithm stolen from
         * http://www.codeproject.com/KB/scrapbook/googlemap.aspx
         */
        virtual std::string tileUrl(const std::string& layer, Core::Zoom z, const Map2X::Core::TileCoords& coords) const;

    private:
        Plugins::MercatorProjection _projection;

        std::vector<Core::Zoom> zoomLevelsOnline;
        Core::TileArea areaOnline;
        std::vector<std::string> layersOnline, overlaysOnline;

        virtual Package* parsePackage(const Map2X::Utility::Configuration* conf);

        virtual std::string tileFromArchive(const std::string& path, const std::string& layer, Core::Zoom z, std::vector< Map2XRasterArchiveReader*>* archives, unsigned int archiveId, int packageVersion, unsigned int tileId);

        std::string name2to3(const std::string& name, int* packageVersion);
        std::string name3to2(const std::string& name, int packageVersion);
};

}}

#endif
