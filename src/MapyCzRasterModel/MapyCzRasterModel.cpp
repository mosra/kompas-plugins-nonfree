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

#include "MapyCzRasterModel.h"

#include <cstdlib>
#include <algorithm>

#include "Utility/Directory.h"

using namespace std;
using namespace Kompas::Utility;
using namespace Kompas::Core;

/** @todo Do it differently (nick + translated names) */
#define __zakladni      "Základní"
#define __turisticka    "Turistická"
#define __fotomapa      "Fotomapa"
#define __historicka    "Historická 1836-52"
#define __fotomapa02    "Fotomapa 2002-03"
#define __stinovani     "Stínování"
#define __popisy        "Popisy"
#define __trasy         "Turistické trasy"
#define __cyklo         "Cyklostezky"

PLUGIN_REGISTER(Kompas::Plugins::MapyCzRasterModel,
                "cz.mosra.Kompas.Core.AbstractRasterModel/0.1")

namespace Kompas { namespace Plugins {

MapyCzRasterModel::MapyCzRasterModel(PluginManager::AbstractPluginManager* manager, const std::string& pluginName): KompasRasterModel(manager, pluginName), areaOnline(1, 1, 6, 6) {
    /* All zoom levels for online maps */
    for(Zoom i = 3; i != 19; ++i)
        zoomLevelsOnline.insert(i);

    /* All layers for online maps */
    layersOnline.push_back(__zakladni);
    layersOnline.push_back(__turisticka);
    layersOnline.push_back(__fotomapa);
    layersOnline.push_back(__historicka);
    layersOnline.push_back(__fotomapa02);

    /* All overlays for online maps */
    overlaysOnline.push_back(__stinovani);
    overlaysOnline.push_back(__popisy);
    overlaysOnline.push_back(__trasy);
    overlaysOnline.push_back(__cyklo);
}

AbstractRasterModel::SupportLevel MapyCzRasterModel::recognizeFile(const std::string& filename, istream& file) const {
    SupportLevel parent = KompasRasterModel::recognizeFile(filename, file);
    if(parent != NotSupported) return parent;

    file.clear();
    file.seekg(0, ios::beg);

    if(Directory::filename(filename) != "map.conf") return NotSupported;

    Configuration conf(file, Configuration::ReadOnly);

    if(conf.value<string>("api") == "seznam" && conf.value<bool>("packed"))
        return FullySupported;

    return NotSupported;
}

string MapyCzRasterModel::tileUrl(const std::string& layer, Zoom z, const TileCoords& coords) const {
    /* Random server number */
    int servernum = rand()%4+1;

    /* URL for given layer */
    ostringstream url;
    url << "http://m" << servernum << ".mapserver.mapy.cz/";
    /** @todo Types base / base-n */
    if(layer == __zakladni)
        url << "base-n";
    else if(layer == __turisticka)
        url << "turist";
    else if(layer == __fotomapa)
        url << "ophoto";
    /** @todo army1? */
    else if(layer == __historicka)
        url << "army2";
    else if(layer == __fotomapa02)
        url << "ophoto0203";
    /** @todo Types relief-l / relief-h */
    else if(layer == __stinovani)
        url << "relief-l";
    else if(layer == __popisy)
        url << "hybrid";
    else if(layer == __trasy)
        url << "ttur";
    else if(layer == __cyklo)
        url << "tcyklo";
    else return "";

    unsigned int multiplier = pow2(28-z);
    unsigned int yReverse = pow2(z)-coords.y-1;

    url << "/" << z << "_" << std::hex << multiplier*coords.x << "_" << multiplier*yReverse;

    return url.str();
}

KompasRasterModel::Package* MapyCzRasterModel::parsePackage(const Configuration* conf) {
    Package* p = KompasRasterModel::parsePackage(conf);
    if(p) return p; /* Version 3 package */

    /* Not version 3 package, backward compatibility with version 2 */

    /* Supporting only packed packages and api=seznam */
    if(conf->value<string>("api") != "seznam" && !conf->value<bool>("packed"))
        return 0;

    p = new Package;
    p->version = 2;
    p->filename = conf->filename();

    /* Package name */
    p->name = conf->value<string>("caption");

    /* Zoom levels, sorted */
    vector<Zoom> z = conf->values<Zoom>("zoom");
    p->zoomLevels.insert(z.begin(), z.end());

    /* Divisor for area size */
    unsigned int divisor = pow2(16-*p->zoomLevels.begin());

    /* Area size (hex) */
    p->area.x = conf->value<unsigned int>("begin_x", 0, Configuration::Hex)/divisor;
    p->area.y = conf->value<unsigned int>("end_y", 0, Configuration::Hex)/divisor+2;
    p->area.w = conf->value<unsigned int>("end_x", 0, Configuration::Hex)/divisor-p->area.x;
    p->area.h = p->area.y-conf->value<unsigned int>("begin_y", 0, Configuration::Hex)/divisor-2;

    /* Layers and overlays, convert them to current names */
    vector<string> layers = conf->values<string>("type");
    for(vector<string>::iterator it = layers.begin(); it != layers.end(); ++it)
        p->layers.push_back(name2to3(*it));

    vector<string> overlays = conf->values<string>("overlay");
    for(vector<string>::iterator it = overlays.begin(); it != overlays.end(); ++it)
        p->overlays.push_back(name2to3(*it));

    /* move_x, move_y etc. is ignored */

    /* The package should have non-empty area, at least one layer (zoom tested above) */
    if(p->area == TileArea() || p->layers.empty() || p->zoomLevels.empty())
        return 0;

    return p;
}

string MapyCzRasterModel::tileFromArchive(const std::string& path, const std::string& layer, Zoom z, std::vector< KompasRasterArchiveReader* >* archives, unsigned int archiveId, int packageVersion, unsigned int tileId) {
    /* Convert layer name from version 3 to version 2 if package is in version 2 */
    if(packageVersion == 2)
        return Kompas::Plugins::KompasRasterModel::tileFromArchive(path, name3to2(layer), z, archives, archiveId, 3, tileId);
    else
        return Kompas::Plugins::KompasRasterModel::tileFromArchive(path, layer, z, archives, archiveId, packageVersion, tileId);
}


string MapyCzRasterModel::name2to3(const string& name) {
    if(name == "base")          return __zakladni;
    if(name == "ophoto")        return __fotomapa;
    if(name == "ophoto0203")    return __fotomapa02;
    if(name == "turist")        return __turisticka;
    if(name == "army2")         return __historicka;
    if(name == "hybrid")        return __popisy;
    if(name == "relief-l")      return __stinovani;
    if(name == "ttur")          return __trasy;
    if(name == "tcyklo")        return __cyklo;

    return "";
}

string MapyCzRasterModel::name3to2(const string& name) {
    if(name == __zakladni)      return "base";
    if(name == __fotomapa)      return "ophoto";
    if(name == __fotomapa02)    return "ophoto0203";
    if(name == __turisticka)    return "turist";
    if(name == __historicka)    return "army2";
    if(name == __popisy)        return "hybrid";
    if(name == __stinovani)     return "relief-l";
    if(name == __trasy)         return "ttur";
    if(name == __cyklo)         return "tcyklo";

    return "";
}

}}
