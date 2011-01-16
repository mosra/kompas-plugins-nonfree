#ifndef Kompas_Plugins_CelestialMap1675RasterModel_h
#define Kompas_Plugins_CelestialMap1675RasterModel_h
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
 * @brief Class Kompas::Plugins::CelestialMap1675RasterModel
 */

#include "KompasRasterModel/KompasRasterModel.h"

#include "StereographicProjection/StereographicProjection.h"

using namespace std;
using namespace Kompas::Core;

namespace Kompas { namespace Plugins {

/** @brief Celestial map 1675 */
class CelestialMap1675RasterModel: public KompasRasterModel {
    public:
        /** @copydoc Plugins::KompasRasterModel::KompasRasterModel */
        CelestialMap1675RasterModel(PluginManager::AbstractPluginManager* manager, const std::string& plugin);

        inline virtual int features() const {
            return KompasRasterModel::features()|LoadableFromUrl|NonConvertableFormat|ConvertableCoords;
        }
        virtual const AbstractProjection* projection() const { return &_projection; }
        virtual TileSize tileSize() const { return TileSize(1926, 1675); }

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

        inline virtual string tileUrl(const string& layer, Zoom z, const TileCoords& coords) const {
            return "http://www.raremaps.com/maps/large/23230.jpg";
        }

    private:
        StereographicProjection _projection;
        std::set<Core::Zoom> zoomLevelsOnline;
        Core::TileArea areaOnline;
        std::vector<std::string> layersOnline, overlaysOnline;
};

}}

#endif
