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

#include "BingMapsRasterModel.h"

#include <algorithm>

#include "Utility/Directory.h"

using namespace std;
using namespace Corrade::Utility;
using namespace Kompas::Core;

#define __base              "base"
#define __satellite         "satellite"
#define __satellitelabels   "satellitelabels"
#define __birdseye          "birdseye"
#define __birdseyelabels    "birdseyelabels"

PLUGIN_REGISTER(BingMapsRasterModel, Kompas::Plugins::BingMapsRasterModel,
                "cz.mosra.Kompas.Core.AbstractRasterModel/0.2")

namespace Kompas { namespace Plugins {

/**
@todo Birds Eye satellite images. They are computed depending on viewer
position, which is absolutely unsupported in current core library.

Birds Eye without hybrid - tile URL:
http://ecn.t3.tiles.virtualearth.net/tiles/o03201023300-9766-20-49.jpeg?g=604

Birds Eye + Hybrid URL:
http://ecn.t3.tiles.virtualearth.net/tiles/cmd/ObliqueHybrid?a=03201023300-9766-20-49&g=604

*/

BingMapsRasterModel::BingMapsRasterModel(Corrade::PluginManager::AbstractPluginManager* manager, const std::string& pluginName): KompasRasterModel(manager, pluginName), areaOnline(0, 0, 2, 2) {
    /* All zoom levels for online maps */
    for(Zoom i = 1; i != 21; ++i)
        zoomLevelsOnline.insert(i);

    /* All layers for online maps */
    layersOnline.push_back(__base);
    layersOnline.push_back(__satellite);
    layersOnline.push_back(__satellitelabels);
}

string BingMapsRasterModel::tileUrl(const std::string& layer, Zoom z, const Kompas::Core::TileCoords& coords) const {
    /* Random server number */
    int servernum = rand()%4;

    /* Tile number */
    ostringstream tilenum;
    TileCoords remaining = coords;
    for(Zoom i = z; i != 0; --i) {
        TileCoords c = remaining/pow2(i-1);
        tilenum << c.y*2 + c.x;
        remaining -= c*pow2(i-1);
    }

    /* URL for given layer */
    ostringstream url;
    url << "http://ecn.t" << servernum << ".tiles.virtualearth.net/tiles/";
    if(layer == __base)
        url << 'r' << tilenum.str() << "?g=604&mkt=en-us&lbl=l1&stl=h&shading=hill&n=z";
    else if(layer == __satellite)
        url << 'a' << tilenum.str() << ".jpeg?g=604&mkt=en-us&n=z";
    else if(layer == __satellitelabels)
        url << 'h' << tilenum.str() << ".jpeg?g=604&mkt=en-us&n=z";
    else return "";

    return url.str();
}

}}
