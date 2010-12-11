#ifndef Kompas_Plugins_GoogleMapsRasterModel_h
#define Kompas_Plugins_GoogleMapsRasterModel_h
/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Kompas.

    Kompas is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Kompas is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

/** @file
 * @brief Class Kompas::Plugins::GoogleMapsRasterModel
 */

#include "KompasRasterModel/KompasRasterModel.h"
#include "MercatorProjection/MercatorProjection.h"

namespace Kompas { namespace Plugins {

/** @brief Google Maps */
class GoogleMapsRasterModel: public KompasRasterModel {
    public:
        /** @copydoc Plugins::KompasRasterModel::KompasRasterModel */
        GoogleMapsRasterModel(PluginManager::AbstractPluginManager* manager, const std::string& pluginName);

        inline virtual int features() const {
            return KompasRasterModel::features()|LoadableFromUrl|NonConvertableFormat|ConvertableCoords;
        }
        virtual SupportLevel recognizeFile(const std::string& filename, std::istream& file) const;
        inline virtual const Kompas::Core::AbstractProjection* projection() const
            { return &_projection; }
        inline virtual Core::TileSize tileSize() const
            { return Core::TileSize(256,256); }
        inline virtual std::string copyright() const
            { return "© 2010 Google, Map Data © 2010 Tele Atlas."; }

        inline virtual std::set<Core::Zoom> zoomLevels() const {
            return online() ? zoomLevelsOnline : KompasRasterModel::zoomLevels();
        }
        inline virtual Core::TileArea area() const {
            return online() ? areaOnline : KompasRasterModel::area();
        }
        virtual std::vector<std::string> layers() const {
            return online() ? layersOnline : KompasRasterModel::layers();
        }
        virtual std::vector<std::string> overlays() const {
            return online() ? overlaysOnline : KompasRasterModel::overlays();
        }

        /**
         * Server number and Galielo secure string algorithm stolen from
         * http://www.codeproject.com/KB/scrapbook/googlemap.aspx
         */
        virtual std::string tileUrl(const std::string& layer, Core::Zoom z, const Kompas::Core::TileCoords& coords) const;

    private:
        Plugins::MercatorProjection _projection;

        std::set<Core::Zoom> zoomLevelsOnline;
        Core::TileArea areaOnline;
        std::vector<std::string> layersOnline, overlaysOnline;

        virtual Package* parsePackage(const Kompas::Utility::Configuration* conf);

        virtual std::string tileFromArchive(const std::string& path, const std::string& layer, Core::Zoom z, std::vector< KompasRasterArchiveReader*>* archives, unsigned int archiveId, int packageVersion, unsigned int tileId);

        std::string name2to3(const std::string& name, int* packageVersion);
        std::string name3to2(const std::string& name, int packageVersion);
};

}}

#endif
