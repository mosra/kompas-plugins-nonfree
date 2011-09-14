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

#include "GoogleMapsRasterModelTest.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtTest/QTest>

#include "Utility/Directory.h"
#include "testConfigure.h"

using namespace std;
using namespace Kompas::Utility;
using namespace Kompas::Core;

/* QEventLoop cannot be used without QApplication, thus not using QTEST_APPLESS_MAIN */
QTEST_MAIN(Kompas::Plugins::Test::GoogleMapsRasterModelTest)

namespace Kompas { namespace Plugins { namespace Test {

void GoogleMapsRasterModelTest::version2() {
    QVERIFY(model.addPackage(Directory::join(GOOGLEMAPSRASTERMODEL_TEST_DIR, "map.conf")) == 0);
    QVERIFY(model.area() == TileArea(62, 41, 3, 2));
    QVERIFY(model.packageAttribute(0, AbstractRasterModel::Name) == "Jih Anglie");

    set<Zoom> zoomLevels;
    zoomLevels.insert(7);
    zoomLevels.insert(8);
    zoomLevels.insert(9);
    zoomLevels.insert(10);
    QVERIFY(model.zoomLevels() == zoomLevels);

    vector<string> layers;
    layers.push_back("base");
    layers.push_back("satellite");
    QVERIFY(model.layers() == layers);

    vector<string> overlays;
    overlays.push_back("labels");
    QVERIFY(model.overlays() == overlays);

    QVERIFY(model.tileFromPackage("base", 7, TileCoords(64, 41)) == "3");
    QVERIFY(model.tileFromPackage("base", 7, TileCoords(63, 42)) == "5");
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
        << "map.conf" << "version=3\nmodel=KompasRasterModel"
        << (int) AbstractRasterModel::NotSupported;
    QTest::newRow("version2Unpacked")
        << "map.conf" << "api=google\npacked=false"
        << (int) AbstractRasterModel::NotSupported;
    QTest::newRow("version2Packed")
        << "map.conf" << "api=google\npacked=true"
        << (int) AbstractRasterModel::DeprecatedSupport;
}

void GoogleMapsRasterModelTest::recognizeFile() {
    QFETCH(QString, filename);
    QFETCH(QString, file);
    QFETCH(int, state);

    istringstream i(file.toStdString());

    QVERIFY(model.recognizeFile(filename.toStdString(), i) == state);
}

void GoogleMapsRasterModelTest::checkCompatibility() {
    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.get(QNetworkRequest(QUrl("http://maps.google.com/")));

    QEventLoop eventLoop;
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QString data = QString::fromUtf8(reply->readAll());

    /* Check base layer version string */
    QRegExp baseRx("http://mt1\\.google\\.com/vt/lyrs=m@(\\d+)");
    QVERIFY(baseRx.indexIn(data) != -1);
    QCOMPARE(baseRx.cap(1), QString::fromStdString(GoogleMapsRasterModel::baseVersion));

    /* Check sattelite layer version string */
    QRegExp satteliteRx("http://khm1\\.google\\.com/kh/v=(\\d+)");
    QVERIFY(satteliteRx.indexIn(data) != -1);
    QCOMPARE(satteliteRx.cap(1), QString::fromStdString(GoogleMapsRasterModel::satteliteVersion));

    /* Check labels overlay version string */
    QRegExp labelsRx("http://mt1\\.google\\.com/vt/lyrs=h@(\\d+)");
    QVERIFY(labelsRx.indexIn(data) != -1);
    QCOMPARE(labelsRx.cap(1), QString::fromStdString(GoogleMapsRasterModel::labelsVersion));
}

}}}
