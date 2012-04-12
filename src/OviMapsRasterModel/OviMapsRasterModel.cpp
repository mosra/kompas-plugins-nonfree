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

#include "OviMapsRasterModel.h"

#include <cstdlib>

#include "Utility/Directory.h"

using namespace std;
using namespace Corrade::Utility;
using namespace Kompas::Core;

#define __normal    "normal"
#define __satellite "satellite"
#define __terrain   "terrain"

PLUGIN_REGISTER(Kompas::Plugins::OviMapsRasterModel,
                "cz.mosra.Kompas.Core.AbstractRasterModel/0.2")

namespace Kompas { namespace Plugins {

OviMapsRasterModel::OviMapsRasterModel(Corrade::PluginManager::AbstractPluginManager* manager, const std::string& pluginName): KompasRasterModel(manager, pluginName), areaOnline(0, 0, 8, 8) {
    /* All zoom levels for online maps */
    for(Zoom i = 3; i != 19; ++i)
        zoomLevelsOnline.insert(i);

    /* All layers for online maps */
    layersOnline.push_back(__normal);
    layersOnline.push_back(__satellite);
    layersOnline.push_back(__terrain);
}

string OviMapsRasterModel::tileUrl(const std::string& layer, Zoom z, const Core::TileCoords& coords) const {
    /* Random server number */
    int servernum = rand()%3+1;

    /* URL for given layer */
    ostringstream url;
    url << "http://maptile.mapplayer" << servernum
        << ".maps.svc.ovi.com/maptiler/maptile/newest/" << layer << ".day/"
        << z << '/' << coords.x << '/' << coords.y << "/256/png8";

    return url.str();
}

}}
