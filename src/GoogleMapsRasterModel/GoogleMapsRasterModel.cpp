/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Map2X.

    Map2X is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Map2X is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "GoogleMapsRasterModel.h"

using namespace Map2X::Core;
using namespace std;

namespace Map2X { namespace Plugins {

vector<Zoom> GoogleMapsRasterModel::zoomLevels() const {
    vector<Zoom> z;
    for(Zoom i = 0; i != 23; ++i)
        z.push_back(i);
    return z;
}

TileArea GoogleMapsRasterModel::area() const {
    return TileArea(0, 0, 1, 1);
}

vector<string> GoogleMapsRasterModel::layers() const {
    vector<string> l;
    l.push_back("Map");
    l.push_back("Satellite");
    return l;
}

vector<string> GoogleMapsRasterModel::overlays() const {
    vector<string> l;
    l.push_back("Labels");
    return l;
}

string GoogleMapsRasterModel::tileUrl(const std::string& layer, Zoom z, const Map2X::Core::TileCoords& coords) const {
    /* "Random" server number */
    int servernum = (coords.x + 2*coords.y)%4;

    /* Galileo substring */
    string galileo = string("Galileo").substr(0, (coords.x*3 + coords.y)%8);

    /* URL for given layer */
    ostringstream url;
    url << "http://";
    if(layer == "Map")
        url << "mt" << servernum << ".google.com/vt/lyrs=m@130";
    else if(layer == "Satellite")
        url << "khm" << servernum << ".google.com/kh/v=66";
    else if(layer == "Labels")
        url << "mt" << servernum << ".google.com/vt/lyrs=h@130";
    else return "";

    url << "&x=" << coords.x << "&y=" << coords.y << "&z=" << z << "&s=" << galileo;

    return url.str();

}

}}
