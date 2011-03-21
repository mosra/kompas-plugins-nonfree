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

/** @todo Layers "Roky snímkování" and "Evidovaná kontaminovaná místa" are
regenerated on every request to fit the viewport bounding box. Skipping them
for now as the URL looks crazy, but this approach seems to be common on other
GIS-powered map servers, so it would be good to have it supported. */

/*
geoportal.gov.cz/ArcGIS/rest/services/CENIA/cenia_nikm_klady/MapServer/export?size=1195%2C390&f=image&bboxSR=102067&dpi=96&layers=show%3A0&bbox=%2D743029%2E5910591821%2C%2D1103890%2E7797815595%2C%2D666539%2E8780797562%2C%2D1078927%2E6098552197&format=png8&transparent=true&imageSR=102067 (years)

geoportal.gov.cz/ArcGIS/rest/services/CENIA/cenia_nikm_km/MapServer/export?size=1195%2C390&f=image&bboxSR=102067&dpi=96&bbox=%2D740733%2E1843310654%2C%2D1095561%2E1637409527%2C%2D664243%2E4713516395%2C%2D1070597%2E993814613&format=png8&transparent=true&imageSR=102067 (contamination)

#define __years         "years"
#define __contamination "contamination"
*/

PLUGIN_REGISTER(Kompas::Plugins::KontaminaceCeniaCzRasterModel,
                "cz.mosra.Kompas.Core.AbstractRasterModel/0.2")

using namespace std;
using namespace Kompas::Core;

namespace Kompas { namespace Plugins {

KontaminaceCeniaCzRasterModel::KontaminaceCeniaCzRasterModel(PluginManager::AbstractPluginManager* manager, const std::string& pluginName): KompasRasterModel(manager, pluginName), areaOnline(0, 0, 8, 5) {
    /* All zoom levels for online maps */
    /** @todo Zoom level 0? */
    for(Zoom i = 3; i != 14; ++i)
        zoomLevelsOnline.insert(i);

    /* All layers and overlays for online maps */
    layersOnline.push_back(__historic);
    layersOnline.push_back(__current);
    layersOnline.push_back(__landsat1);
    layersOnline.push_back(__landsat2);

    overlaysOnline.push_back(__labels);
    // overlaysOnline.push_back(__years);
    // overlaysOnline.push_back(__contamination);
}

string KontaminaceCeniaCzRasterModel::tileUrl(const std::string& layer, Zoom z, const Core::TileCoords& coords) const {
    ostringstream url;
    url << "http://geoportal.gov.cz/ArcGIS/rest/services/CENIA/cenia";

    /* URL for given layer */
    if(layer == __historic)             url << "_rt_ortofotomapa_historicka";
    else if(layer == __current)         url << "_rt_ortofotomapa_aktualni";
    else if(layer == __landsat1)        url << "_landsat_rgb";
    else if(layer == __landsat2)        url << "_landsat_635";
    else if(layer == __labels)          url << "_t_popisky";
    // else if(layer == __years)           url << FIXME;
    // else if(layer == __contamination)   url << FIXME;
    else return "";

    url << "/MapServer/tile/"
        << z << '/' << coords.y << '/' << coords.x;

    return url.str();
}

}}
