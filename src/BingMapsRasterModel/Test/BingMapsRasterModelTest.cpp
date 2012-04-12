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

#include "BingMapsRasterModelTest.h"

#include <QtCore/QRegExp>
#include <QtTest/QTest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include "BingMapsRasterModel/BingMapsRasterModel.h"

/* QEventLoop cannot be used without QApplication, thus not using QTEST_APPLESS_MAIN */
QTEST_MAIN(Kompas::Plugins::Test::BingMapsRasterModelTest)

namespace Kompas { namespace Plugins { namespace Test {

void BingMapsRasterModelTest::checkCompatibility() {
    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.get(QNetworkRequest(QUrl("http://www.bing.com/maps/")));

    QEventLoop eventLoop;
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QString data = QString::fromUtf8(reply->readAll());
    QString urlParts("\"([^\\{]+)\\{subdomain\\}([^\\{]+)\\{quadkey\\}([^\"]+)\"");

    QRegExp baseRx("\"roadUriFormat\":" + urlParts);
    QVERIFY(baseRx.indexIn(data) != -1);
    QCOMPARE(baseRx.cap(1), QString::fromStdString(BingMapsRasterModel::baseSubdomain));
    QCOMPARE(baseRx.cap(2), QString::fromStdString(BingMapsRasterModel::baseServer));
    QCOMPARE(baseRx.cap(3), QString::fromStdString(BingMapsRasterModel::baseData));

    QRegExp satelliteRx("\"aerialUriFormat\":" + urlParts);
    QVERIFY(satelliteRx.indexIn(data) != -1);
    QCOMPARE(satelliteRx.cap(1), QString::fromStdString(BingMapsRasterModel::satelliteSubdomain));
    QCOMPARE(satelliteRx.cap(2), QString::fromStdString(BingMapsRasterModel::satelliteServer));
    QCOMPARE(satelliteRx.cap(3), QString::fromStdString(BingMapsRasterModel::satelliteData));

    QRegExp satelliteLabelsRx("\"aerialWithLabelsUriFormat\":" + urlParts);
    QVERIFY(satelliteLabelsRx.indexIn(data) != -1);
    QCOMPARE(satelliteLabelsRx.cap(1), QString::fromStdString(BingMapsRasterModel::satelliteLabelsSubdomain));
    QCOMPARE(satelliteLabelsRx.cap(2), QString::fromStdString(BingMapsRasterModel::satelliteLabelsServer));
    QCOMPARE(satelliteLabelsRx.cap(3), QString::fromStdString(BingMapsRasterModel::satelliteLabelsData));
}

}}}
