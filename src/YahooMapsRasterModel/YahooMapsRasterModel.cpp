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

#include "YahooMapsRasterModel.h"

#include <algorithm>

#include "Utility/Directory.h"

using namespace std;
using namespace Corrade::Utility;
using namespace Kompas::Core;

#define __base      "base"
#define __satellite "satellite"
#define __labels    "labels"

PLUGIN_REGISTER(YahooMapsRasterModel, Kompas::Plugins::YahooMapsRasterModel,
                "cz.mosra.Kompas.Core.AbstractRasterModel/0.2")

namespace Kompas { namespace Plugins {

YahooMapsRasterModel::YahooMapsRasterModel(Corrade::PluginManager::AbstractPluginManager* manager, const std::string& pluginName): KompasRasterModel(manager, pluginName), areaOnline(0, 0, 2, 2) {
    /* All zoom levels for online maps */
    for(Zoom i = 1; i != 20; ++i)
        zoomLevelsOnline.insert(i);

    /* All layers for online maps */
    layersOnline.push_back(__base);
    layersOnline.push_back(__satellite);

    /* All overlays for online maps */
    overlaysOnline.push_back(__labels);
}

string YahooMapsRasterModel::tileUrl(const std::string& layer, Zoom z, const Core::TileCoords& coords) const {
    /* Random server number */
    int servernum = rand()%3+1;

    /* URL for given layer */
    ostringstream url;
    url << "http://maps" << servernum << ".yimg.com/";
    if(layer == __base)
        url << "hx/tl?b=1&v=4.3";
    else if(layer == __satellite)
        url << "ae/ximg?v=1.9&t=a&s=256";
    else if(layer == __labels)
        url << "hx/tl?b=1&v=4.3&t=h";
    else return "";

    url << "&.intl=en&x=" << coords.x << "&y=" << (coords.y < pow2(z-1) ? static_cast<int>(pow2(z-1)-coords.y-1) : -(static_cast<int>(coords.y-pow2(z-1)+1))) << "&z=" << z+1 << "&r=1";

    return url.str();
}

}}
