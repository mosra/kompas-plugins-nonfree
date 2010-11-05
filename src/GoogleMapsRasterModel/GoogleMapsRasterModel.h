#ifndef Map2X_Plugins_GoogleMapsRasterModel_h
#define Map2X_Plugins_GoogleMapsRasterModel_h
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

/** @file
 * @brief Class Map2X::Plugins::GoogleMapsRasterModel
 */

#include "Map2XRasterModel/Map2XRasterModel.h"
#include "MercatorProjection/MercatorProjection.h"

namespace Map2X { namespace Plugins {

/** @brief Google Maps */
class GoogleMapsRasterModel: public Map2XRasterModel {
    public:
        /** @copydoc Plugins::Map2XRasterModel::Map2XRasterModel */
        GoogleMapsRasterModel(PluginManager::AbstractPluginManager* manager, const std::string& pluginName):
            Map2XRasterModel(manager, pluginName) {}

        inline virtual int features() const {
            return Map2XRasterModel::features()|LoadableFromUrl|NonConvertableFormat;
        }
        virtual const Map2X::Core::AbstractProjection* projection() const
            { return &_projection; }
        inline virtual Core::TileSize tileSize() const
            { return Core::TileSize(256,256); }
        virtual std::string copyright() const
            { return "© 2010 Google, Map Data © 2010 Tele Atlas."; }

        virtual std::vector<Core::Zoom> zoomLevels() const;
        virtual Core::TileArea area() const;
        virtual std::vector<std::string> layers() const;
        virtual std::vector<std::string> overlays() const;

        /**
         * Server number and Galielo secure string algorithm stolen from
         * http://www.codeproject.com/KB/scrapbook/googlemap.aspx
         */
        virtual std::string tileUrl(const std::string& layer, Core::Zoom z, const Map2X::Core::TileCoords& coords) const;

    private:
        Plugins::MercatorProjection _projection;
};

}}

PLUGIN_REGISTER(Map2X::Plugins::GoogleMapsRasterModel,
                "cz.mosra.Map2X.Core.AbstractRasterModel/0.1")

#endif
