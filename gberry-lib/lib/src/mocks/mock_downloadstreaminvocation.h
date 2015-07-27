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
#ifndef MOCK_DOWNLOADSTREAMINVOCATION_CPP
#define MOCK_DOWNLOADSTREAMINVOCATION_CPP

#include <QString>
#include <QJsonDocument>

#include "downloadstreaminvocation.h"

#include <gmock/gmock.h>


class MockDownloadStreamInvocation : public DownloadStreamInvocation
{
 public:
     MockDownloadStreamInvocation();
     virtual ~MockDownloadStreamInvocation();

     MOCK_METHOD1(defineGetOperation, void(const QString& path));
     MOCK_METHOD2(definePostOperation, void(const QString&  path, const QJsonDocument& jsondoc));
     MOCK_CONST_METHOD0(statusCode, Invocation::InvocationStatus());
     MOCK_CONST_METHOD0(responseHttpStatusCode, HTTPInvocationDefinition::Status());
     MOCK_METHOD1(setOutputFilePath, void(const QString& path));
     MOCK_CONST_METHOD0(progressPercentage, int());
     MOCK_CONST_METHOD0(result, Result());

     MOCK_METHOD0(execute, void());
     MOCK_METHOD0(abort, void());

     // no need to mock signals 'cos they don't have impl and they are not called
     // TODO: we could have a macro generate following kinds of functions
     void emitFinishedOK() { emit finishedOK(this); }
     void emitFinishedError() { emit finishedError(this); }
     void emitDownloadStarted() { emit downloadStarted(this); }
     void emitDownloadProgress() { emit downloadProgress(this); }
};


#endif // MOCK_DOWNLOADSTREAMINVOCATION_CPP

