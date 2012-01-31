#ifndef Kompas_Plugins_BingMapsRasterModel_h
#define Kompas_Plugins_BingMapsRasterModel_h
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
 * @brief Class Kompas::Plugins::BingMapsRasterModel
 */

#include "KompasRasterModel/KompasRasterModel.h"
#include "MercatorProjection/MercatorProjection.h"

namespace Kompas { namespace Plugins {

/**
@brief Bing Maps

@section BingMapsRasterModel_TileNumbering Tile numbering
Tile coordinates is one long string composed of characters <tt>0, 1, 2, 3</tt>.
The numbering is done recursive from lowest zoom. In zoom 1, the map is 2x2
tiles and the tiles are numbered like this:
<pre>
+---+---+
| 0 | 1 |
+---+---+
| 2 | 3 |
+---+---+
</pre>
In next (larger) zoom, every tile is divided into another four and these are
numbered the same way. Their number is added to number of tile in previous
(smaller) zoom:
<pre>
+----+----+----+----+
| 00 | 01 | 10 | 11 |
+----+----+----+----+
| 02 | 03 | 12 | 13 |
+----+----+----+----+
| 20 | 21 | 30 | 31 |
+----+----+----+----+
| 22 | 23 | 32 | 33 |
+----+----+----+----+
</pre>
And so on until the destination zoom level is reached.
 */
class BingMapsRasterModel: public KompasRasterModel {
    public:
        /** @copydoc Plugins::KompasRasterModel::KompasRasterModel */
        BingMapsRasterModel(Corrade::PluginManager::AbstractPluginManager* manager, const std::string& plugin);

        inline int features() const {
            return KompasRasterModel::features()|MultipleFileFormat|LoadableFromUrl|NonConvertableFormat|ConvertableCoords;
        }
        inline const Core::AbstractProjection* projection() const
            { return &_projection; }
        inline std::string celestialBody() const
            { return "EarthCelestialBody"; }
        inline Core::TileSize tileSize() const
            { return Core::TileSize(256,256); }
        inline std::string copyright() const
            { return "© 2010 Microsoft Corporation © 2010 NAVTEQ"; }

        inline std::set<Core::Zoom> zoomLevels() const {
            return online() ? zoomLevelsOnline : KompasRasterModel::zoomLevels();
        }
        inline Core::TileArea area() const {
            return online() ? areaOnline : KompasRasterModel::area();
        }
        std::vector<std::string> layers() const {
            return online() ? layersOnline : KompasRasterModel::layers();
        }

        std::string tileUrl(const std::string& layer, Core::Zoom z, const Core::TileCoords& coords) const;

    private:
        MercatorProjection _projection;

        std::set<Core::Zoom> zoomLevelsOnline;
        Core::TileArea areaOnline;
        std::vector<std::string> layersOnline;
};

}}

#endif
