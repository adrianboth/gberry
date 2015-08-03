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
 
#include <testutils/qtgtest.h>
#include <testutils/waiter.h>

#include <QCoreApplication>
#include <QTime>
#include <QThread>

#include "invocationfactoryimpl.h"
#include "restinvocation.h"
#include "testutils/signalrecorder.h"
#include "utils/testhttpserver.h"


TEST(RestInvocationFactoryImpl, RealGetOperationWithParameters)
{
    InvocationFactoryImpl factoryObj;
    InvocationFactory* factory = &factoryObj; // operate through public interface

    factory->setProperty(InvocationFactory::URL_PREFIX_PROP, "http://localhost:9999/gberryrest/v1");

    RESTInvocation* inv = factory->newRESTInvocation();
    SignalRecorder okRecorder;
    SignalRecorder errRecorder;

    QObject::connect(inv, &RESTInvocation::finishedOK,
                     &okRecorder, &SignalRecorder::signal1_QObjectPointer);

    QObject::connect(inv, &RESTInvocation::finishedError,
                     &errRecorder, &SignalRecorder::signal1_QObjectPointer);

    // set http server to localhost
    TestHttpServer server(9999);

    bool requestReceived = false;
    QString expectedPath("/gberryrest/v1/testop");
    auto mySlot = [&](QHttpRequest* req, QHttpResponse* resp)
    {
        requestReceived = true;

        // TODO: later more flexible checking
        //QRegExp exp("^/console/v1/([a-z]+)?$");
        //if( exp.indexIn(req->path()) != -1 )
        //QString name = exp.capturedTexts()[1];

        if (req->path() != expectedPath)
        {
            resp->writeHead(403);
            QString errorMsg("Expected path: " + expectedPath + "\nGot path: " + req->path());
            resp->end(errorMsg.toLatin1());
            return;
        }

        resp->setHeader("Content-Type", "text/html");
        resp->writeHead(200);

        QString body = "ping";
        resp->end(body.toUtf8());
    };

    QObject::connect(&server, &TestHttpServer::request, mySlot);

    inv->defineGetOperation("/testop");
    inv->defineParameter("test1", "test1value");
    inv->defineParameter("test2", "test2value");
    inv->execute();

    EXPECT_FALSE(okRecorder.received());
    EXPECT_FALSE(errRecorder.received());
    WAIT_AND_ASSERT(requestReceived); // http server got request

    // wait that client side receives response
    WAIT_AND_ASSERT(okRecorder.received_QObjectPointer()); // if not ok, would cause segfaults

    RESTInvocation* inv2 = qobject_cast<RESTInvocation *>(okRecorder.getQObjectPointer());
    ASSERT_TRUE(inv2->responseAvailable());
    EXPECT_EQ(inv2->responseHttpStatusCode(), HTTPInvocationDefinition::OK_200); // TODO: better name
    EXPECT_EQ(inv2->statusCode(), RESTInvocation::RESPONSE_RECEIVED);

    QString responseData = inv2->responseString();
    EXPECT_TRUE(responseData == "ping");
}


TEST(RestInvocationFactoryImpl, RealGetOperation)
{
    InvocationFactoryImpl factoryObj;
    InvocationFactory* factory = &factoryObj; // operate through public interface

    factory->setProperty(InvocationFactory::URL_PREFIX_PROP, "http://localhost:9999/gberryrest/v1");
    //factory->setProperty(InvocationFactory::URL_PREFIX_PROP, "http://localhost:8050/console/v1");

    RESTInvocation* inv = factory->newRESTInvocation();
    SignalRecorder okRecorder;
    SignalRecorder errRecorder;

    QObject::connect(inv, &RESTInvocation::finishedOK,
                     &okRecorder, &SignalRecorder::signal1_QObjectPointer);

    QObject::connect(inv, &RESTInvocation::finishedError,
                     &errRecorder, &SignalRecorder::signal1_QObjectPointer);

    // set http server to localhost
    TestHttpServer server(9999);
    //QString testPath("/gberryrest/v1");

    bool replyReceived = false;
    QString expectedPath("/gberryrest/v1/ping");
    auto mySlot = [&](QHttpRequest* req, QHttpResponse* resp)
    {
        replyReceived = true;

        // TODO: later more flexible checking
        //QRegExp exp("^/console/v1/([a-z]+)?$");
        //if( exp.indexIn(req->path()) != -1 )
        //QString name = exp.capturedTexts()[1];

        if (req->path() != expectedPath)
        {
            resp->writeHead(403);
            QString errorMsg("Expected path: " + expectedPath + "\nGot path: " + req->path());
            resp->end(errorMsg.toLatin1());
            return;
        }

        resp->setHeader("Content-Type", "text/html");
        resp->writeHead(200);

        QString body = "ping";
        resp->end(body.toUtf8());
    };

    QObject::connect(&server, &TestHttpServer::request, mySlot);

    //GBerryConsoleServer gbserver;
    // TODO: detailed handling -> check correct path

    /**
    QRegExp exp("^/console/v1/([a-z]+)?$");
    if( exp.indexIn(req->path()) != -1 )
    {
        resp->setHeader("Content-Type", "text/html");
        resp->writeHead(200);

        QString name = exp.capturedTexts()[1];
        QString body = tr("<html><head><title>Greeting App</title></head><body><h1>Hello %1!</h1></body></html>");
        resp->end(body.arg(name).toUtf8());
        qDebug() << "OK";
    }
    else
    {
        resp->writeHead(403);
        resp->end(QByteArray("You aren't allowed here!"));
        qDebug() << "ERROR 403";
    }
    **/

    qDebug("## BEFORE PING");
    inv->defineGetOperation("/ping");
    inv->execute();
    qDebug("## AFTER PING");

    WAIT_AND_ASSERT(okRecorder.received());

    ASSERT_TRUE(replyReceived);
    ASSERT_TRUE(okRecorder.received_QObjectPointer()); // if not ok, would cause segfaults

    RESTInvocation* inv2 = qobject_cast<RESTInvocation *>(okRecorder.getQObjectPointer());
    ASSERT_TRUE(inv2->responseAvailable());
    EXPECT_EQ(inv2->responseHttpStatusCode(), HTTPInvocationDefinition::OK_200); // TODO: better name
    EXPECT_EQ(inv2->statusCode(), RESTInvocation::RESPONSE_RECEIVED);

    QString responseData = inv2->responseString();
    EXPECT_TRUE(responseData == "ping");
}

// TODO: test case if property not defined
// TODO: test case if connection not allowed
