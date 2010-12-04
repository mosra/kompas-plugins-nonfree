#ifndef Kompas_Plugins_Test_MapyCzRasterModelTest_h
#define Kompas_Plugins_Test_MapyCzRasterModelTest_h
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

#include <QtCore/QObject>

#include "MapyCzRasterModel/MapyCzRasterModel.h"

namespace Kompas { namespace Plugins { namespace Test {

class MapyCzRasterModelTest: public QObject {
    Q_OBJECT

    public:
        MapyCzRasterModelTest(QObject* parent = 0): QObject(parent), model(0, "MapyCzRasterModel") {}

    private slots:
        void version2();

        void recognizeFile_data();
        void recognizeFile();

    private:
        MapyCzRasterModel model;
};

}}}

#endif