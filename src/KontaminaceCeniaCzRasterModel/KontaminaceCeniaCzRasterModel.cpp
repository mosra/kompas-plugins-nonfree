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

#include "KontaminaceCeniaCzRasterModel.h"

#define __historic      "historic"
#define __current       "current"
#define __landsat1      "landsat1"
#define __landsat2      "landsat2"
#define __labels        "labels"
#define __years         "years"
#define __contamination "contamination"

PLUGIN_REGISTER(Kompas::Plugins::KontaminaceCeniaCzRasterModel,
                "cz.mosra.Kompas.Core.AbstractRasterModel/0.2")

using namespace std;
using namespace Kompas::Core;

namespace Kompas { namespace Plugins {

KontaminaceCeniaCzRasterModel::KontaminaceCeniaCzRasterModel(PluginManager::AbstractPluginManager* manager, const std::string& pluginName): KompasRasterModel(manager, pluginName), areaOnline(0, 0, 4, 3) {
    /* All zoom levels for online maps */
    /** @todo Zoom level 0? */
    for(Zoom i = 1; i != 12; ++i)
        zoomLevelsOnline.insert(i);

    /* All layers and overlays for online maps */
    layersOnline.push_back(__historic);
    layersOnline.push_back(__current);
    layersOnline.push_back(__landsat1);
    layersOnline.push_back(__landsat2);

    overlaysOnline.push_back(__labels);
    overlaysOnline.push_back(__years);
    overlaysOnline.push_back(__contamination);
}

string KontaminaceCeniaCzRasterModel::tileUrl(const std::string& layer, Zoom z, const Core::TileCoords& coords) const {
    ostringstream url;
    url << "http://kontaminace.cenia.cz/ArcGIS/rest/services/";

    /* URL for given layer */
    if(layer == __historic)             url << "historicke_1890";
    else if(layer == __current)         url << "soucasne_1890";
    else if(layer == __landsat1)        url << "landsat_rgb";
    else if(layer == __landsat2)        url << "landsat_635";
    else if(layer == __labels)          url << "popisy";
    else if(layer == __years)           url << "historicke_roky_1890";
    else if(layer == __contamination)   url << "sekm_zateze";
    else return "";

    url << "/MapServer/tile/"
        << z << '/' << coords.y << '/' << coords.x;

    return url.str();
}

}}
