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

#include "CelestialMap1675RasterModel.h"

#include "constants.h"

using namespace std;

PLUGIN_REGISTER(Kompas::Plugins::CelestialMap1675RasterModel,
                "cz.mosra.Kompas.Core.AbstractRasterModel/0.2")

namespace Kompas { namespace Plugins {

CelestialMap1675RasterModel::CelestialMap1675RasterModel(PluginManager::AbstractPluginManager* manager, const string& plugin): KompasRasterModel(manager, plugin), areaOnline(0, 0, 1, 1) {
    /*
        width:  1926
        height: 1625
        left:   97
        top:    400, 396 => 398
        right:  1826
        bottom: 1276, 1274 => 1275
        gap:    0
    */
    _projection.setShift(Coords<double>(97/1926.0, 398/1625.0));
    _projection.setStretch(Coords<double>((1826-97)/1926.0, (1275-398)/1625.0));

    /* All zoom levels and layers for online map */
    zoomLevelsOnline.insert(0);
    layersOnline.push_back("base");
}

}}
