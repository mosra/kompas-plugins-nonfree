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

#include "MapyCzRasterModelTest.h"

#include <QtTest/QTest>

#include "Utility/Directory.h"
#include "testConfigure.h"

using namespace std;
using namespace Kompas::Utility;
using namespace Kompas::Core;

QTEST_APPLESS_MAIN(Kompas::Plugins::Test::MapyCzRasterModelTest)

namespace Kompas { namespace Plugins { namespace Test {

void MapyCzRasterModelTest::version2() {
    QVERIFY(model.addPackage(Directory::join(MAPYCZRASTERMODEL_TEST_DIR, "map.conf")) == 0);
    QVERIFY(model.area() == TileArea(255, 257, 4, 4));
    QVERIFY(model.packageAttribute(0, AbstractRasterModel::Name) == "České Budějovice a okolí");

    vector<Zoom> zoomLevels;
    zoomLevels.push_back(9);
    zoomLevels.push_back(10);
    zoomLevels.push_back(11);
    QVERIFY(model.zoomLevels() == zoomLevels);

    vector<string> layers;
    layers.push_back("Základní");
    layers.push_back("Fotomapa");
    QVERIFY(model.layers() == layers);

    vector<string> overlays;
    overlays.push_back("Popisy");
    overlays.push_back("Stínování");
    QVERIFY(model.overlays() == overlays);

    QVERIFY(model.tileFromPackage("Základní", 9, TileCoords(257, 257)) == "3");
    QVERIFY(model.tileFromPackage("Základní", 9, TileCoords(255, 258)) == "5");
}

void MapyCzRasterModelTest::recognizeFile_data() {
    QTest::addColumn<QString>("filename");
    QTest::addColumn<QString>("file");
    QTest::addColumn<int>("state");

    QTest::newRow("badFilename")
        << "map.shp" << ""
        << (int) AbstractRasterModel::NotSupported;
    QTest::newRow("thisModel")
        << "map.conf" << "version=3\nmodel=MapyCzRasterModel"
        << (int) AbstractRasterModel::FullySupported;
    QTest::newRow("genericModel")
        << "map.conf" << "version=3\nmodel=KompasRasterModel"
        << (int) AbstractRasterModel::NotSupported;
    QTest::newRow("version2Unpacked")
        << "map.conf" << "api=seznam\npacked=false"
        << (int) AbstractRasterModel::NotSupported;
    QTest::newRow("version2Packed")
        << "map.conf" << "api=seznam\npacked=true"
        << (int) AbstractRasterModel::FullySupported;
}

void MapyCzRasterModelTest::recognizeFile() {
    QFETCH(QString, filename);
    QFETCH(QString, file);
    QFETCH(int, state);

    istringstream i(file.toStdString());

    QVERIFY(model.recognizeFile(filename.toStdString(), i) == state);
}

}}}
