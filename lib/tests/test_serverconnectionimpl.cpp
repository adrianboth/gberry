/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
 #include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Mock;
using ::testing::StrictMock;
using ::testing::_;

#include <QtCore/qglobal.h>

#include <QCoreApplication>
#include <QVariant>

#include "serverconnectionimpl.h"
#include "mocks/mock_restinvocation.h"
#include "mocks/mock_restinvocationfactory.h"
#include "testutils/signalrecorder.h"
#include "testobjects/stub_systemservices.h"


// TODO: better common location

// It's important that the << operator is defined in the SAME
// namespace that defines Bar.  C++'s look-up rules rely on that.
::std::ostream& operator<<(::std::ostream& os, const QString& bar) {
  return os << qPrintable(bar);
}


TEST(serverConnectionImpl, pingOk_first_then_fail)
{
    MockRESTInvocationFactory factoryMock;
    MockRESTInvocation invocationMock;

    QCoreApplication* coreapp = QCoreApplication::instance();
    TestSystemServices* testservices=  new TestSystemServices();
    QObject *obj = testservices;
    //QVariant var(QMetaType::QObjectStar, obj); //QVariant::fromValue(obj);
    QVariant var = QVariant::fromValue(obj);
    coreapp->setProperty("systemservices", var);

    // TODO: direct test
    QVariant var2 = coreapp->property("systemservices");
    ASSERT_TRUE(var2.isValid());
    if (var2.canConvert<QObject*>())
    {
        qDebug("YES CAN CONVERT");
    }
    else
    {
        qDebug("NO CONVERT");
    }
    //QObject* obj2 = qvariant_cast<QObject *>(&var2);
    QObject* obj2 = var2.value<QObject*>();
    SystemServices* services = qobject_cast<SystemServices*>(obj2);

    services->singleshotTimer(500, coreapp, SLOT(quit()));
    //QMetaType::QObjectStar


    ServerConnectionImpl serverImpl(&factoryMock);
    ServerConnection* server = &serverImpl; // testing through interface

    EXPECT_FALSE(server->isConnected());

    SignalRecorder connectedSignal;
    SignalRecorder disconnectedSignal;

    SignalRecorder::connect(server,              &ServerConnection::connected,
                            &connectedSignal,    &SignalRecorder::signal0);
    SignalRecorder::connect(server,              &ServerConnection::disconnected,
                            &disconnectedSignal, &SignalRecorder::signal0);

    EXPECT_FALSE(connectedSignal.received());
    EXPECT_FALSE(disconnectedSignal.received());

    // no calls yet
    Mock::VerifyAndClearExpectations(&factoryMock);

    // setup mocks
    //  - factory to return invocation
    //  - invocation to take get() call
    EXPECT_CALL(factoryMock, newRESTInvocation())
              .Times(1)
              .WillOnce( Return(&invocationMock));

    EXPECT_CALL(invocationMock, defineGetOperation(QString("/ping"))).Times(1);
    EXPECT_CALL(invocationMock, execute()).Times(1);

    server->open(); // ping REST call waits event loop processing

    EXPECT_FALSE(server->isConnected());
    EXPECT_FALSE(connectedSignal.received());
    EXPECT_FALSE(disconnectedSignal.received());

    // now server waits invocation to finish
    invocationMock.emitFinishedOK();

    // TODO: called pingReady() directly? is following needed?
    QCoreApplication::processEvents();

    EXPECT_TRUE(server->isConnected());
    EXPECT_TRUE(connectedSignal.received());
    EXPECT_FALSE(disconnectedSignal.received());

    // keep timer on hold (no other events are generated)
    QCoreApplication::processEvents();
    connectedSignal.reset();
    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(&invocationMock);

    EXPECT_TRUE(server->isConnected());
    EXPECT_FALSE(connectedSignal.received());
    EXPECT_FALSE(disconnectedSignal.received());

    // expectations for next ping

    EXPECT_CALL(factoryMock, newRESTInvocation())
              .Times(1)
              .WillOnce( Return(&invocationMock));

    EXPECT_CALL(invocationMock, defineGetOperation(QString("/ping"))).Times(1);
    EXPECT_CALL(invocationMock, execute()).Times(1);

    // fast forward clock to send next ping
    testservices->invokeSingleshotTimer();

    QCoreApplication::processEvents();
    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(&invocationMock);

    invocationMock.emitFinishedError();
    QCoreApplication::processEvents();

    EXPECT_FALSE(server->isConnected());
    EXPECT_FALSE(connectedSignal.received());
    EXPECT_TRUE(disconnectedSignal.received());
}
