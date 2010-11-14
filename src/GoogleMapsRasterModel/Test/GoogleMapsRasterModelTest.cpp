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

#include "GoogleMapsRasterModelTest.h"

#include <QtTest/QTest>

#include "Utility/Directory.h"
#include "testConfigure.h"

using namespace std;
using namespace Map2X::Utility;
using namespace Map2X::Core;

QTEST_APPLESS_MAIN(Map2X::Plugins::Test::GoogleMapsRasterModelTest)

namespace Map2X { namespace Plugins { namespace Test {

void GoogleMapsRasterModelTest::version2() {
    QVERIFY(model.addPackage(Directory::join(GOOGLEMAPSRASTERMODEL_TEST_DIR, "map.conf")) == 0);
    QVERIFY(model.area() == TileArea(62, 41, 3, 2));
    QVERIFY(model.attribute(AbstractRasterModel::Name, 0) == "Jih Anglie");

    vector<Zoom> zoomLevels;
    zoomLevels.push_back(7);
    zoomLevels.push_back(8);
    zoomLevels.push_back(9);
    zoomLevels.push_back(10);
    QVERIFY(model.zoomLevels() == zoomLevels);

    vector<string> layers;
    layers.push_back("Map");
    layers.push_back("Satellite");
    QVERIFY(model.layers() == layers);

    vector<string> overlays;
    overlays.push_back("Labels");
    QVERIFY(model.overlays() == overlays);

    QVERIFY(model.tileFromPackage("Map", 7, TileCoords(64, 41)) == "3");
    QVERIFY(model.tileFromPackage("Map", 7, TileCoords(63, 42)) == "5");
}

void GoogleMapsRasterModelTest::recognizeFile_data() {
    QTest::addColumn<QString>("filename");
    QTest::addColumn<QString>("file");
    QTest::addColumn<int>("state");

    QTest::newRow("badFilename")
        << "map.shp" << ""
        << (int) AbstractRasterModel::NotSupported;
    QTest::newRow("thisModel")
        << "map.conf" << "version=3\nmodel=GoogleMapsRasterModel"
        << (int) AbstractRasterModel::FullySupported;
    QTest::newRow("genericModel")
        << "map.conf" << "version=3\nmodel=Map2XRasterModel"
        << (int) AbstractRasterModel::NotSupported;
    QTest::newRow("version2Unpacked")
        << "map.conf" << "api=google\npacked=false"
        << (int) AbstractRasterModel::NotSupported;
    QTest::newRow("version2Packed")
        << "map.conf" << "api=google\npacked=true"
        << (int) AbstractRasterModel::FullySupported;
}

void GoogleMapsRasterModelTest::recognizeFile() {
    QFETCH(QString, filename);
    QFETCH(QString, file);
    QFETCH(int, state);

    istringstream i(file.toStdString());

    QVERIFY(model.recognizeFile(filename.toStdString(), i) == state);
}

}}}
