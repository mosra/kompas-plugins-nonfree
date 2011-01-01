/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Kompas.

    Kompas is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Kompas is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "GoogleMapsRasterModel.h"

#include <algorithm>

#include "Utility/Directory.h"

using namespace std;
using namespace Kompas::Core;
using namespace Kompas::Utility;

#define __base      "base"
#define __satellite "satellite"
#define __labels    "labels"

PLUGIN_REGISTER(Kompas::Plugins::GoogleMapsRasterModel,
                "cz.mosra.Kompas.Core.AbstractRasterModel/0.1")

namespace Kompas { namespace Plugins {

GoogleMapsRasterModel::GoogleMapsRasterModel(PluginManager::AbstractPluginManager* manager, const std::string& pluginName): KompasRasterModel(manager, pluginName), areaOnline(0, 0, 1, 1) {
    /* All zoom levels for online maps */
    for(Zoom i = 0; i != 23; ++i)
        zoomLevelsOnline.insert(i);

    /* All layers for online maps */
    layersOnline.push_back(__base);
    layersOnline.push_back(__satellite);

    /* All overlays for online maps */
    overlaysOnline.push_back(__labels);
}

AbstractRasterModel::SupportLevel GoogleMapsRasterModel::recognizeFile(const std::string& filename, istream& file) const {
    SupportLevel parent = KompasRasterModel::recognizeFile(filename, file);
    if(parent != NotSupported) return parent;

    file.clear();
    file.seekg(0, ios::beg);

    if(Directory::filename(filename) != "map.conf") return NotSupported;

    Configuration conf(file, Configuration::ReadOnly);

    if(conf.value<string>("api") == "google" && conf.value<bool>("packed"))
        return DeprecatedSupport;

    return NotSupported;
}

string GoogleMapsRasterModel::tileUrl(const std::string& layer, Zoom z, const Kompas::Core::TileCoords& coords) const {
    /* "Random" server number */
    int servernum = (coords.x + 2*coords.y)%4;

    /* Galileo substring */
    string galileo = string("Galileo").substr(0, (coords.x*3 + coords.y)%8);

    /* URL for given layer */
    ostringstream url;
    url << "http://";
    if(layer == __base)
        url << "mt" << servernum << ".google.com/vt/lyrs=m@130";
    else if(layer == __satellite)
        url << "khm" << servernum << ".google.com/kh/v=66";
    else if(layer == __labels)
        url << "mt" << servernum << ".google.com/vt/lyrs=h@130";
    else return "";

    url << "&x=" << coords.x << "&y=" << coords.y << "&z=" << z << "&s=" << galileo;

    return url.str();
}

KompasRasterModel::Package* GoogleMapsRasterModel::parsePackage(const Configuration* conf) {
    Package* p = KompasRasterModel::parsePackage(conf);
    if(p) return p; /* Version 3 package */

    /* Not version 3 package, backward compatibility with version 2 */

    /* Supporting only packed packages and api=seznam */
    if(conf->value<string>("api") != "google" && !conf->value<bool>("packed"))
        return 0;

    p = new Package;
    p->version = 0;
    p->filename = conf->filename();

    /* Package name */
    p->name = conf->value<string>("caption");

    /* Zoom levels, sorted */
    vector<Zoom> z = conf->values<Zoom>("zoom");
    p->zoomLevels.insert(z.begin(), z.end());

    /* Area size */
    p->area.x = conf->value<unsigned int>("begin_x");
    p->area.y = conf->value<unsigned int>("begin_y");
    p->area.w = conf->value<unsigned int>("end_x")-p->area.x;
    p->area.h = conf->value<unsigned int>("end_y")-p->area.y;

    /* Layers and overlays, convert them to current names */
    vector<string> layers = conf->values<string>("type");
    for(vector<string>::iterator it = layers.begin(); it != layers.end(); ++it)
        p->layers.push_back(name2to3(*it, &p->version));

    vector<string> overlays = conf->values<string>("overlay");
    for(vector<string>::iterator it = overlays.begin(); it != overlays.end(); ++it)
        p->overlays.push_back(name2to3(*it, &p->version));

    /* move_x, move_y etc. is ignored */

    /* The package should have non-empty area, at least one layer and zoom */
    if(p->area == TileArea() || p->layers.empty() || p->zoomLevels.empty())
        return 0;

    return p;
}

string GoogleMapsRasterModel::tileFromArchive(const string& path, const string& layer, Zoom z, vector< KompasRasterArchiveReader*>* archives, unsigned int archiveId, int packageVersion, unsigned int tileId) {
    /* Convert layer name from version 3 to version 2 if package is in version 2 */
    if(packageVersion == 3)
        return Kompas::Plugins::KompasRasterModel::tileFromArchive(path, layer, z, archives, archiveId, packageVersion, tileId);
    else
        return Kompas::Plugins::KompasRasterModel::tileFromArchive(path, name3to2(layer, packageVersion), z, archives, archiveId, 2, tileId);
}

string GoogleMapsRasterModel::name2to3(const string& name, int* packageVersion) {
    if(name.empty()) return "";

    string ret;
    int shift = 0;

    if(name[2] == '.') {
        shift = 20;
        ret = __base;
    } else if(name[2] == 'p') {
        shift = 10;
        ret = __satellite;
    } else if(name[2] == 't') {
        shift = 0;
        ret = __labels;
    } else return "";

    /* Add layer numbers to packageVersion */

    /* character '2' from w2.80 */
    *packageVersion |= (name[1] & 7) << (shift+7);

    /* number '80' from w2.80 */
    std::istringstream i(name.substr(name.size()-2));
    int version;
    i >> version;
    *packageVersion |= (version & 127) << shift;

    return ret;
}

string GoogleMapsRasterModel::name3to2(const string& name, int packageVersion) {
    string character;
    int shift;

    if(name == __base) {
        shift = 20;
    } else if(name == __satellite) {
        shift = 10;
        character = "p";
    } else if(name == __labels) {
        shift = 0;
        character = "t";
    } else return "";

    /* Get layer numbers from packageVersion */
    ostringstream o;
    o << 'w';

    /* character '2' from w2.80 */
    o << static_cast<int>((packageVersion >> (shift+7) & 7));

    o << '.' << character;

    /* number '80' from w2.80 */
    o << ((packageVersion >> shift) & 127);

    return o.str();
}

}}
