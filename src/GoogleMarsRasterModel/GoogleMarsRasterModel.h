#ifndef Kompas_Plugins_GoogleMarsRasterModel_h
#define Kompas_Plugins_GoogleMarsRasterModel_h
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
 * @brief Class Kompas::Plugins::GoogleMarsRasterModel
 */

#include "KompasRasterModel/KompasRasterModel.h"
#include "MercatorProjection/MercatorProjection.h"

namespace Kompas { namespace Plugins {

/**
@brief Google Mars

@section GoogleMarsRasterModel_TileNumbering Tile numbering
Tile numbering is similar to @ref BingMapsRasterModel_TileNumbering "Bing Maps",
but the tiles are numbered with characters <tt>q, r, s, t</tt> instead of <tt>0,
1, 2, 3</tt>. Also the numbering has slightly different order:
<pre>
+---+---+
| q | r |
+---+---+
| t | s |
+---+---+
</pre>
In zoom 0, the tile has coordinates <tt>t</tt>, this character is prefixed to
coordinates in all other zoom levels.
*/
class GoogleMarsRasterModel: public KompasRasterModel {
    public:
        /** @copydoc Plugins::KompasRasterModel::KompasRasterModel */
        GoogleMarsRasterModel(Corrade::PluginManager::AbstractPluginManager* manager, const std::string& plugin);

        inline int features() const {
            return KompasRasterModel::features()|MultipleFileFormat|LoadableFromUrl|NonConvertableFormat|ConvertableCoords;
        }
        inline const Kompas::Core::AbstractProjection* projection() const
            { return &_projection; }
        inline std::string celestialBody() const
            { return "MarsCelestialBody"; }
        inline Core::TileSize tileSize() const
            { return Core::TileSize(256,256); }
        inline std::string copyright() const
            { return ""; }

        inline std::set<Core::Zoom> zoomLevels() const {
            return online() ? zoomLevelsOnline : KompasRasterModel::zoomLevels();
        }
        inline Core::TileArea area() const {
            return online() ? areaOnline : KompasRasterModel::area();
        }
        std::vector<std::string> layers() const {
            return online() ? layersOnline : KompasRasterModel::layers();
        }
        std::vector<std::string> overlays() const {
            return online() ? overlaysOnline : KompasRasterModel::overlays();
        }

        std::string tileUrl(const std::string& layer, Core::Zoom z, const Kompas::Core::TileCoords& coords) const;

    private:
        Plugins::MercatorProjection _projection;

        std::set<Core::Zoom> zoomLevelsOnline;
        Core::TileArea areaOnline;
        std::vector<std::string> layersOnline, overlaysOnline;
};

}}

#endif
