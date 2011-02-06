#ifndef Kompas_Plugins_MapyCzRasterModel_h
#define Kompas_Plugins_MapyCzRasterModel_h
/*
    Copyright © 2007, 2008, 2009, 2010, 2011 Vladimír Vondruš <mosra@centrum.cz>

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
 * @brief Class Kompas::Plugins::MapyCzRasterModel
 */

#include "KompasRasterModel/KompasRasterModel.h"

namespace Kompas { namespace Plugins {

/**
 * @brief Mapy.cz
 * @todo @c VERSION-0.2 UtmProjection
 */
class MapyCzRasterModel: public KompasRasterModel {
    public:
        /** @copydoc Plugins::KompasRasterModel::KompasRasterModel */
        MapyCzRasterModel(PluginManager::AbstractPluginManager* manager, const std::string& plugin);

        inline virtual int features() const {
            return KompasRasterModel::features()|MultipleFileFormat|LoadableFromUrl|NonConvertableFormat;
        }
        virtual SupportLevel recognizeFile(const std::string& filename, std::istream& file) const;
        inline virtual std::string celestialBody() const
            { return "EarthCelestialBody"; }
        inline virtual Core::TileSize tileSize() const
            { return Core::TileSize(256,256); }
        inline virtual std::string copyright() const
            { return "© PLANstudio, 2005-2010, © SHOCart, s.r.o., © Seznam.cz"; }

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

        virtual std::string tileUrl(const std::string& layer, Core::Zoom z, const Kompas::Core::TileCoords& coords) const;

    private:
        std::set<Core::Zoom> zoomLevelsOnline;
        Core::TileArea areaOnline;
        std::vector<std::string> layersOnline, overlaysOnline;

        virtual Package* parsePackage(const Utility::Configuration* conf);

        virtual std::string tileFromArchive(const std::string& path, const std::string& layer, Core::Zoom z, std::vector<KompasRasterArchiveReader*>* archives, unsigned int archiveId, int packageVsersion, unsigned int tileId);

        std::string name2to3(const std::string& name);
        std::string name3to2(const std::string& name);
};

}}

#endif
