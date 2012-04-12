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

#include "OviMapsRasterModelTest.h"

#include <QtCore/QRegExp>
#include <QtTest/QTest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include "OviMapsRasterModel/OviMapsRasterModel.h"

/* QEventLoop cannot be used without QApplication, thus not using QTEST_APPLESS_MAIN */
QTEST_MAIN(Kompas::Plugins::Test::OviMapsRasterModelTest)

namespace Kompas { namespace Plugins { namespace Test {

void OviMapsRasterModelTest::checkCompatibility() {
    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.get(QNetworkRequest(QUrl("http://static1.maps.nokia.com/34047/core/js/libs.js")));

    QEventLoop eventLoop;
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QString data = QString::fromUtf8(reply->readAll());

    /* Check server URL */
    QRegExp serverRx("\"ovi.url\":\"([^\"]+)\"");
    QVERIFY(serverRx.indexIn(data) != -1);
    QCOMPARE(serverRx.cap(1), QString::fromStdString(OviMapsRasterModel::urlPrefix));
}

}}}
