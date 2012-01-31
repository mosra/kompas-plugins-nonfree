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

#include "GoogleMoonRasterModel.h"

using namespace std;
using namespace Corrade::Utility;
using namespace Kompas::Core;

#define __apollo                "apollo"
#define __visible               "visible"
#define __elevation             "elevation"
#define __geo_j_herschel        "geo-j-herschel"
#define __tpo_j_herschel        "tpo-j-herschel"
#define __geo_plato             "geo-plato"
#define __tpo_plato             "tpo-plato"
#define __geo_aristoteles       "geo-aristoteles"
#define __tpo_aristoteles       "tpo-aristoteles"
#define __geo_rumker            "geo-rumker"
#define __tpo_rumker            "tpo-rumker"
#define __geo_sinus_iridum      "geo-sinus-iridum"
#define __tpo_sinus_iridum      "tpo-sinus-iridum"
#define __geo_cassini           "geo-cassini"
#define __tpo_cassini           "tpo-cassini"
#define __geo_eudoxus           "geo-eudoxus"
#define __tpo_eudoxus           "tpo-eudoxus"
#define __geo_geminus           "geo-geminus"
#define __tpo_geminus           "tpo-geminus"
#define __geo_seleucus          "geo-seleucus"
#define __tpo_seleucus          "tpo-seleucus"
#define __geo_aristarchus       "geo-aristarchus"
#define __tpo_aristarchus       "tpo-aristarchus"
#define __geo_timocharis        "geo-timocharis"
#define __tpo_timocharis        "tpo-timocharis"
#define __geo_montes_apenninus  "geo-montes-apenninus"
#define __tpo_montes_apenninus  "tpo-montes-apenninus"
#define __geo_mare_serenitatis  "geo-mare-serenitatis"
#define __tpo_mare_serenitatis  "tpo-mare-serenitatis"
#define __geo_macrobius         "geo-macrobius"
#define __tpo_macrobius         "tpo-macrobius"
#define __geo_cleomedes         "geo-cleomedes"
#define __tpo_cleomedes         "tpo-cleomedes"
#define __geo_hevellus          "geo-hevellus"
#define __tpo_hevellus          "tpo-hevellus"
#define __geo_kepler            "geo-kepler"
#define __tpo_kepler            "tpo-kepler"
#define __geo_copernicus        "geo-copernicus"
#define __tpo_copernicus        "tpo-copernicus"
#define __geo_mare_vaporum      "geo-mare-vaporum"
#define __tpo_mare_vaporum      "tpo-mare-vaporum"
#define __geo_julius_caesar     "geo-julius-caesar"
#define __tpo_julius_caesar     "tpo-julius-caesar"
#define __geo_taruntius         "geo-taruntius"
#define __tpo_taruntius         "tpo-taruntius"
#define __geo_mare_undarum      "geo-mare-undarum"
#define __tpo_mare_undarum      "tpo-mare-undarum"
#define __geo_grimaldi          "geo-grimaldi"
#define __tpo_grimaldi          "tpo-grimaldi"
#define __geo_letronne          "geo-letronne"
#define __tpo_letronne          "tpo-letronne"
#define __geo_riphaeus_mtns     "geo-riphaeus-mtns"
#define __tpo_riphaeus_mtns     "tpo-riphaeus-mtns"
#define __geo_ptolemaeus        "geo-ptolemaeus"
#define __tpo_ptolemaeus        "tpo-ptolemaeus"
#define __geo_theophilus        "geo-theophilus"
#define __tpo_theophilus        "tpo-theophilus"
#define __geo_colombo           "geo-colombo"
#define __tpo_colombo           "tpo-colombo"
#define __geo_langrenus         "geo-langrenus"
#define __tpo_langrenus         "tpo-langrenus"
#define __geo_byrgius           "geo-byrgius"
#define __tpo_byrgius           "tpo-byrgius"
#define __geo_mare_humorum      "geo-mare-humorum"
#define __tpo_mare_humorum      "tpo-mare-humorum"
#define __geo_pitatus           "geo-pitatus"
#define __tpo_pitatus           "tpo-pitatus"
#define __geo_purbach           "geo-purbach"
#define __tpo_purbach           "tpo-purbach"
#define __geo_rupes_altai       "geo-rupes-altai"
#define __tpo_rupes_altai       "tpo-rupes-altai"
#define __geo_fracastorius      "geo-fracastorius"
#define __tpo_fracastorius      "tpo-fracastorius"
#define __geo_petavius          "geo-petavius"
#define __tpo_petavius          "tpo-petavius"
#define __geo_schickard         "geo-schickard"
#define __tpo_schickard         "tpo-schickard"
#define __geo_wilhelm           "geo-wilhelm"
#define __tpo_wilhelm           "tpo-wilhelm"
#define __geo_tycho             "geo-tycho"
#define __tpo_tycho             "tpo-tycho"
#define __geo_maurolycus        "geo-maurolycus"
#define __tpo_maurolycus        "tpo-maurolycus"
#define __geo_rheita            "geo-rheita"
#define __tpo_rheita            "tpo-rheita"
#define __geo_schiller          "geo-schiller"
#define __tpo_schiller          "tpo-schiller"
#define __geo_clavius           "geo-clavius"
#define __tpo_clavius           "tpo-clavius"
#define __geo_hommel            "geo-hommel"
#define __tpo_hommel            "tpo-hommel"

PLUGIN_REGISTER(Kompas::Plugins::GoogleMoonRasterModel,
                "cz.mosra.Kompas.Core.AbstractRasterModel/0.2")

namespace Kompas { namespace Plugins {

GoogleMoonRasterModel::GoogleMoonRasterModel(Corrade::PluginManager::AbstractPluginManager* manager, const std::string& pluginName): KompasRasterModel(manager, pluginName), areaOnline(0, 0, 1, 1) {
    /* All zoom levels for online maps */
    for(Zoom i = 0; i != 20; ++i)
        zoomLevelsOnline.insert(i);

    /* All layers for online maps */
    layersOnline.push_back(__apollo);
    layersOnline.push_back(__visible);
    layersOnline.push_back(__elevation);

    /* All overlays for online maps */
    overlaysOnline.push_back(__geo_j_herschel);
    overlaysOnline.push_back(__geo_plato);
    overlaysOnline.push_back(__geo_aristoteles);
    overlaysOnline.push_back(__geo_rumker);
    overlaysOnline.push_back(__geo_sinus_iridum);
    overlaysOnline.push_back(__geo_cassini);
    overlaysOnline.push_back(__geo_eudoxus);
    overlaysOnline.push_back(__geo_geminus);
    overlaysOnline.push_back(__geo_seleucus);
    overlaysOnline.push_back(__geo_aristarchus);
    overlaysOnline.push_back(__geo_timocharis);
    overlaysOnline.push_back(__geo_montes_apenninus);
    overlaysOnline.push_back(__geo_mare_serenitatis);
    overlaysOnline.push_back(__geo_macrobius);
    overlaysOnline.push_back(__geo_cleomedes);
    overlaysOnline.push_back(__geo_hevellus);
    overlaysOnline.push_back(__geo_kepler);
    overlaysOnline.push_back(__geo_copernicus);
    overlaysOnline.push_back(__geo_mare_vaporum);
    overlaysOnline.push_back(__geo_julius_caesar);
    overlaysOnline.push_back(__geo_taruntius);
    overlaysOnline.push_back(__geo_mare_undarum);
    overlaysOnline.push_back(__geo_grimaldi);
    overlaysOnline.push_back(__geo_letronne);
    overlaysOnline.push_back(__geo_riphaeus_mtns);
    overlaysOnline.push_back(__geo_ptolemaeus);
    overlaysOnline.push_back(__geo_theophilus);
    overlaysOnline.push_back(__geo_colombo);
    overlaysOnline.push_back(__geo_langrenus);
    overlaysOnline.push_back(__geo_byrgius);
    overlaysOnline.push_back(__geo_mare_humorum);
    overlaysOnline.push_back(__geo_pitatus);
    overlaysOnline.push_back(__geo_purbach);
    overlaysOnline.push_back(__geo_rupes_altai);
    overlaysOnline.push_back(__geo_fracastorius);
    overlaysOnline.push_back(__geo_petavius);
    overlaysOnline.push_back(__geo_schickard);
    overlaysOnline.push_back(__geo_wilhelm);
    overlaysOnline.push_back(__geo_tycho);
    overlaysOnline.push_back(__geo_maurolycus);
    overlaysOnline.push_back(__geo_rheita);
    overlaysOnline.push_back(__geo_schiller);
    overlaysOnline.push_back(__geo_clavius);
    overlaysOnline.push_back(__geo_hommel);

    overlaysOnline.push_back(__tpo_j_herschel);
    overlaysOnline.push_back(__tpo_plato);
    overlaysOnline.push_back(__tpo_aristoteles);
    overlaysOnline.push_back(__tpo_rumker);
    overlaysOnline.push_back(__tpo_sinus_iridum);
    overlaysOnline.push_back(__tpo_cassini);
    overlaysOnline.push_back(__tpo_eudoxus);
    overlaysOnline.push_back(__tpo_geminus);
    overlaysOnline.push_back(__tpo_seleucus);
    overlaysOnline.push_back(__tpo_aristarchus);
    overlaysOnline.push_back(__tpo_timocharis);
    overlaysOnline.push_back(__tpo_montes_apenninus);
    overlaysOnline.push_back(__tpo_mare_serenitatis);
    overlaysOnline.push_back(__tpo_macrobius);
    overlaysOnline.push_back(__tpo_cleomedes);
    overlaysOnline.push_back(__tpo_hevellus);
    overlaysOnline.push_back(__tpo_kepler);
    overlaysOnline.push_back(__tpo_copernicus);
    overlaysOnline.push_back(__tpo_mare_vaporum);
    overlaysOnline.push_back(__tpo_julius_caesar);
    overlaysOnline.push_back(__tpo_taruntius);
    overlaysOnline.push_back(__tpo_mare_undarum);
    overlaysOnline.push_back(__tpo_grimaldi);
    overlaysOnline.push_back(__tpo_letronne);
    overlaysOnline.push_back(__tpo_riphaeus_mtns);
    overlaysOnline.push_back(__tpo_ptolemaeus);
    overlaysOnline.push_back(__tpo_theophilus);
    overlaysOnline.push_back(__tpo_colombo);
    overlaysOnline.push_back(__tpo_langrenus);
    overlaysOnline.push_back(__tpo_byrgius);
    overlaysOnline.push_back(__tpo_mare_humorum);
    overlaysOnline.push_back(__tpo_pitatus);
    overlaysOnline.push_back(__tpo_purbach);
    overlaysOnline.push_back(__tpo_rupes_altai);
    overlaysOnline.push_back(__tpo_fracastorius);
    overlaysOnline.push_back(__tpo_petavius);
    overlaysOnline.push_back(__tpo_schickard);
    overlaysOnline.push_back(__tpo_wilhelm);
    overlaysOnline.push_back(__tpo_tycho);
    overlaysOnline.push_back(__tpo_maurolycus);
    overlaysOnline.push_back(__tpo_rheita);
    overlaysOnline.push_back(__tpo_schiller);
    overlaysOnline.push_back(__tpo_clavius);
    overlaysOnline.push_back(__tpo_hommel);
}

string GoogleMoonRasterModel::tileUrl(const std::string& layer, Zoom z, const Kompas::Core::TileCoords& coords) const {
    /* URL for given layer */
    ostringstream url;
    url << "http://mw1.google.com/mw-planetary/lunar/lunarmaps_v1/";

    string extension;

    if(layer == __apollo) {
        url << "apollo";
        extension = ".jpg";
    } else if(layer == __visible) {
        url << "clem_bw";
        extension = ".jpg";
    } else if(layer == __elevation) {
        url << "terrain";
        extension = ".jpg";
    }

    else if(layer == __geo_j_herschel)
        url << "charts/geo/I604";
    else if(layer == __geo_plato)
        url << "charts/geo/I701";
    else if(layer == __geo_aristoteles)
        url << "charts/geo/I725";
    else if(layer == __geo_rumker)
        url << "charts/geo/I805";
    else if(layer == __geo_sinus_iridum)
        url << "charts/geo/I602";
    else if(layer == __geo_cassini)
        url << "charts/geo/I666";
    else if(layer == __geo_eudoxus)
        url << "charts/geo/I705";
    else if(layer == __geo_geminus)
        url << "charts/geo/I841";
    else if(layer == __geo_seleucus)
        url << "charts/geo/I527";
    else if(layer == __geo_aristarchus)
        url << "charts/geo/I465";
    else if(layer == __geo_timocharis)
        url << "charts/geo/I462";
    else if(layer == __geo_montes_apenninus)
        url << "charts/geo/I463";
    else if(layer == __geo_mare_serenitatis)
        url << "charts/geo/I489";
    else if(layer == __geo_macrobius)
        url << "charts/geo/I799";
    else if(layer == __geo_cleomedes)
        url << "charts/geo/I707";
    else if(layer == __geo_hevellus)
        url << "charts/geo/I491";
    else if(layer == __geo_kepler)
        url << "charts/geo/I355";
    else if(layer == __geo_copernicus)
        url << "charts/geo/I515";
    else if(layer == __geo_mare_vaporum)
        url << "charts/geo/I548";
    else if(layer == __geo_julius_caesar)
        url << "charts/geo/I510";
    else if(layer == __geo_taruntius)
        url << "charts/geo/I722";
    else if(layer == __geo_mare_undarum)
        url << "charts/geo/I837";
    else if(layer == __geo_grimaldi)
        url << "charts/geo/I740";
    else if(layer == __geo_letronne)
        url << "charts/geo/I385";
    else if(layer == __geo_riphaeus_mtns)
        url << "charts/geo/I458";
    else if(layer == __geo_ptolemaeus)
        url << "charts/geo/I566";
    else if(layer == __geo_theophilus)
        url << "charts/geo/I546";
    else if(layer == __geo_colombo)
        url << "charts/geo/I714";
    else if(layer == __geo_langrenus)
        url << "charts/geo/I739";
    else if(layer == __geo_byrgius)
        url << "charts/geo/I755";
    else if(layer == __geo_mare_humorum)
        url << "charts/geo/I495";
    else if(layer == __geo_pitatus)
        url << "charts/geo/I485";
    else if(layer == __geo_purbach)
        url << "charts/geo/I822";
    else if(layer == __geo_rupes_altai)
        url << "charts/geo/I690";
    else if(layer == __geo_fracastorius)
        url << "charts/geo/I720";
    else if(layer == __geo_petavius)
        url << "charts/geo/I794";
    else if(layer == __geo_schickard)
        url << "charts/geo/I823";
    else if(layer == __geo_wilhelm)
        url << "charts/geo/I824";
    else if(layer == __geo_tycho)
        url << "charts/geo/I713";
    else if(layer == __geo_maurolycus)
        url << "charts/geo/I695";
    else if(layer == __geo_rheita)
        url << "charts/geo/I694";
    else if(layer == __geo_schiller)
        url << "charts/geo/I691";
    else if(layer == __geo_clavius)
        url << "charts/geo/I706";
    else if(layer == __geo_hommel)
        url << "charts/geo/I702";

    else if(layer == __tpo_j_herschel)
        url << "charts/lac/LAC11";
    else if(layer == __tpo_plato)
        url << "charts/lac/LAC12";
    else if(layer == __tpo_aristoteles)
        url << "charts/lac/LAC13";
    else if(layer == __tpo_rumker)
        url << "charts/lac/LAC23";
    else if(layer == __tpo_sinus_iridum)
        url << "charts/lac/LAC24";
    else if(layer == __tpo_cassini)
        url << "charts/lac/LAC25";
    else if(layer == __tpo_eudoxus)
        url << "charts/lac/LAC26";
    else if(layer == __tpo_geminus)
        url << "charts/lac/LAC27";
    else if(layer == __tpo_seleucus)
        url << "charts/lac/LAC38";
    else if(layer == __tpo_aristarchus)
        url << "charts/lac/LAC39";
    else if(layer == __tpo_timocharis)
        url << "charts/lac/LAC40";
    else if(layer == __tpo_montes_apenninus)
        url << "charts/lac/LAC41";
    else if(layer == __tpo_mare_serenitatis)
        url << "charts/lac/LAC42";
    else if(layer == __tpo_macrobius)
        url << "charts/lac/LAC43";
    else if(layer == __tpo_cleomedes)
        url << "charts/lac/LAC44";
    else if(layer == __tpo_hevellus)
        url << "charts/lac/LAC56";
    else if(layer == __tpo_kepler)
        url << "charts/lac/LAC57";
    else if(layer == __tpo_copernicus)
        url << "charts/lac/LAC58";
    else if(layer == __tpo_mare_vaporum)
        url << "charts/lac/LAC59";
    else if(layer == __tpo_julius_caesar)
        url << "charts/lac/LAC60";
    else if(layer == __tpo_taruntius)
        url << "charts/lac/LAC61";
    else if(layer == __tpo_mare_undarum)
        url << "charts/lac/LAC62";
    else if(layer == __tpo_grimaldi)
        url << "charts/lac/LAC74";
    else if(layer == __tpo_letronne)
        url << "charts/lac/LAC75";
    else if(layer == __tpo_riphaeus_mtns)
        url << "charts/lac/LAC76";
    else if(layer == __tpo_ptolemaeus)
        url << "charts/lac/LAC77";
    else if(layer == __tpo_theophilus)
        url << "charts/lac/LAC78";
    else if(layer == __tpo_colombo)
        url << "charts/lac/LAC79";
    else if(layer == __tpo_langrenus)
        url << "charts/lac/LAC80";
    else if(layer == __tpo_byrgius)
        url << "charts/lac/LAC92";
    else if(layer == __tpo_mare_humorum)
        url << "charts/lac/LAC93";
    else if(layer == __tpo_pitatus)
        url << "charts/lac/LAC94";
    else if(layer == __tpo_purbach)
        url << "charts/lac/LAC95";
    else if(layer == __tpo_rupes_altai)
        url << "charts/lac/LAC96";
    else if(layer == __tpo_fracastorius)
        url << "charts/lac/LAC97";
    else if(layer == __tpo_petavius)
        url << "charts/lac/LAC98";
    else if(layer == __tpo_schickard)
        url << "charts/lac/LAC110";
    else if(layer == __tpo_wilhelm)
        url << "charts/lac/LAC111";
    else if(layer == __tpo_tycho)
        url << "charts/lac/LAC112";
    else if(layer == __tpo_maurolycus)
        url << "charts/lac/LAC113";
    else if(layer == __tpo_rheita)
        url << "charts/lac/LAC114";
    else if(layer == __tpo_schiller)
        url << "charts/lac/LAC125";
    else if(layer == __tpo_clavius)
        url << "charts/lac/LAC126";
    else if(layer == __tpo_hommel)
        url << "charts/lac/LAC127";

    else return "";

    url << '/' << z << '/' << coords.x << '/' << pow2(z)-coords.y-1 << extension;
    return url.str();
}

}}
