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

#include "GoogleMarsRasterModel.h"

#include <algorithm>

using namespace std;
using namespace Corrade::Utility;
using namespace Kompas::Core;

PLUGIN_REGISTER(Kompas::Plugins::GoogleMarsRasterModel,
                "cz.mosra.Kompas.Core.AbstractRasterModel/0.2")

#define __elevation "elevation"
#define __visible   "visible"
#define __infrared  "infrared"

namespace Kompas { namespace Plugins {

GoogleMarsRasterModel::GoogleMarsRasterModel(Corrade::PluginManager::AbstractPluginManager* manager, const std::string& pluginName): KompasRasterModel(manager, pluginName), areaOnline(0, 0, 1, 1) {
    /* All zoom levels for online maps */
    for(Zoom i = 0; i != 13; ++i)
        zoomLevelsOnline.insert(i);

    /* All layers for online maps */
    layersOnline.push_back(__elevation);
    layersOnline.push_back(__visible);
    layersOnline.push_back(__infrared);
}

string GoogleMarsRasterModel::tileUrl(const std::string& layer, Zoom z, const Kompas::Core::TileCoords& coords) const {
    ostringstream url;
    url << "http://mw1.google.com/mw-planetary/mars/" << layer << "/t";

    /* Tile number */
    TileCoords remaining = coords;
    for(Zoom i = z; i != 0; --i) {
        TileCoords c = remaining/pow2(i-1);
        url << static_cast<char>('q' + (c.y == 0 ? c.x : 3-c.x));
        remaining -= c*pow2(i-1);
    }

    url << ".jpg";
    return url.str();
}

}}
