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
 
 #include "localapplicationsstorage.h"

#include <QTemporaryDir>

#include "testutils/waiter.h"
#include "testutils/qtgtest.h"
#include "testutils/util_testdata.h"
#include <utils/fileutils.h>

#define LOG_AREA "LocalApplicationsStorageTests"
#include "log/log.h"


TEST(LocalApplicationsStorage, SimpleAppsDir_ParsingInformation)
{
    QString appsDir = TestUtils::testdataDirPath("simple_case1");
    LocalApplicationsStorage storage(appsDir);

    QList<QSharedPointer<Application>> apps = storage.applications();

    // order of items doesn't matter -> create utility for checking
    QMap<QString, QSharedPointer<Application>> appsById;
    foreach(QSharedPointer<Application> app, apps) { appsById[app->id()] = app; }

    EXPECT_EQ(apps.length(), 2);
    ASSERT_TRUE(appsById.contains(QString("app1-1.0.0")));
    ASSERT_TRUE(appsById.contains(QString("app2-1.5.2")));

    QSharedPointer<Application> app1 = appsById["app1-1.0.0"];
    QSharedPointer<Application> app2 = appsById["app2-1.5.2"];

    QSharedPointer<IApplicationMeta> meta1 = app1->meta();
    QSharedPointer<IApplicationMeta> meta2 = app2->meta();

    EXPECT_TRUE(meta1->applicationId() == "app1");
    EXPECT_TRUE(meta2->applicationId() == "app2");

    EXPECT_TRUE(meta1->name() == "Application1");
    EXPECT_TRUE(meta2->name() == "Application2");

    EXPECT_TRUE(meta1->version() == "1.0.0");
    EXPECT_TRUE(meta2->version() == "1.5.2");

    EXPECT_TRUE(meta1->id() == "app1-1.0.0");
    EXPECT_TRUE(meta2->id() == "app2-1.5.2");

    EXPECT_TRUE(meta1->description() == "This is a test application 1");
    EXPECT_TRUE(meta2->description() == "This is a test application 2");

    QString app1Dir = QDir(appsDir).absoluteFilePath("app1-1.0.0");
    QString app2Dir = QDir(appsDir).absoluteFilePath("app2-1.5.2");

    EXPECT_TRUE(meta1->applicationDirPath() == app1Dir);
    EXPECT_TRUE(meta2->applicationDirPath() == app2Dir);

    EXPECT_TRUE(meta1->applicationExecutablePath() == QDir(app1Dir).filePath("app1"));
    EXPECT_TRUE(meta2->applicationExecutablePath() == QDir(app2Dir).filePath("app2"));

    auto joinpath = [&] (QString appDir, QString path) {
        return QDir(appDir).absoluteFilePath(path);
    };

    EXPECT_TRUE(meta1->catalogImageFilePath() == joinpath(app1Dir, "app1_image.png"));
    EXPECT_TRUE(meta2->catalogImageFilePath() == joinpath(app2Dir, "app2_image.png")) << meta2->catalogImageFilePath() << joinpath(app2Dir, "app2_image.png");

}


TEST(LocalApplicationsStorage, DifferentApplicationStates)
{
    QString appsDir = TestUtils::testdataDirPath("states_case1");
    LocalApplicationsStorage storage(appsDir);

    QList<QSharedPointer<Application>> apps = storage.applications();
    ASSERT_EQ(apps.length(), 4);

    // order of items doesn't matter -> create utility for checking
    QMap<QString, QSharedPointer<Application>> appsById;
    foreach(QSharedPointer<Application> app, apps) {
        DEBUG("Found app:" << app->id());
        appsById[app->id()] = app;
    }

    EXPECT_TRUE(appsById["app1-valid-1.0.0"]->state() == Application::Valid);
    EXPECT_TRUE(appsById["app2-invalid-1.0.0"]->state() == Application::Invalid);
    EXPECT_TRUE(appsById["app3-downloading-1.0.0"]->state() == Application::Downloading);
    EXPECT_TRUE(appsById["app4-notdefined-valid-1.0.0"]->state() == Application::Valid);
}


TEST(LocalApplicationsStorage, UpdateApplicationState)
{
    QString appsDir = TestUtils::testdataDirPath("states_case1");

    // as we are going to modify data -> make copy
    QTemporaryDir temporaryDir;
    DEBUG("Copy testdata from" << appsDir << "to" << temporaryDir.path());
    ASSERT_TRUE(GBerryLib::copyRecursively(appsDir, temporaryDir.path())); // verify ok

    LocalApplicationsStorage storage(temporaryDir.path());

    // -- interface test (should get all updates)
    QSharedPointer<LocalApplications> app_xx = storage.localApplications();
    EXPECT_TRUE(app_xx->application("app3-downloading-1.0.0")->state() == Application::Downloading);
    // TODO: is problem assigment??
    //QSharedPointer<IApplications> iapp_xx = (qSharedPointerCast<IApplications>(app_xx));
    //QSharedPointer<IApplications> iapp_xx(qSharedPointerCast<IApplications>(app_xx));
    // --

    QSharedPointer<LocalApplications> apps = storage.localApplications();
    QSharedPointer<IApplication> iapp = apps->application("app3-downloading-1.0.0");
    ASSERT_FALSE(iapp.isNull()); // sanity check that test data ok
    EXPECT_TRUE(iapp->state() == Application::Downloading);

    // TODO: dummy test: does taking new localApps disconnect old ones??
    QSharedPointer<LocalApplications> apps_yy = storage.localApplications();

    // TODO: nasty casting, any other way
    QSharedPointer<Application> app(qSharedPointerCast<Application>(iapp));
    app->markState(IApplication::Valid);

    LocalApplicationsStorage::Result updateResult;
    int updateResultFlag = storage.updateApplication(*app.data(), updateResult);
    EXPECT_TRUE(updateResultFlag); // true == ok
    ASSERT_FALSE(updateResult.hasError()) << updateResult.errorString;

    // read to verify
    QSharedPointer<LocalApplications> apps2 = storage.localApplications();
    QSharedPointer<IApplication> iapp2 = apps2->application("app3-downloading-1.0.0");
    EXPECT_TRUE(iapp2->state() == Application::Valid);

    // and actually existing LocalApplications should update itself
    QSharedPointer<IApplication> iapp3 = apps->application("app3-downloading-1.0.0");
    EXPECT_TRUE(iapp3->state() == Application::Valid);

// -- interface test
    QSharedPointer<IApplication> iapp4 = app_xx->application("app3-downloading-1.0.0");
    EXPECT_TRUE(iapp4->state() == Application::Valid);

}


TEST(LocalApplicationsStorage, UpdateApplicationState2)
{
    QString appsDir = TestUtils::testdataDirPath("states_case1");

    // as we are going to modify data -> make copy
    QTemporaryDir temporaryDir;
    DEBUG("Copy testdata from" << appsDir << "to" << temporaryDir.path());
    ASSERT_TRUE(GBerryLib::copyRecursively(appsDir, temporaryDir.path())); // verify ok

    LocalApplicationsStorage storage(temporaryDir.path());

    // -- interface test (should get all updates)
    QSharedPointer<LocalApplications> apps_xx = storage.localApplications();
    QSharedPointer<IApplication> iapp_xx = apps_xx->application("app3-downloading-1.0.0");
    EXPECT_TRUE(iapp_xx->state() == Application::Downloading);
    // TODO: is problem assigment??
    //QSharedPointer<IApplications> iapp_xx = (qSharedPointerCast<IApplications>(app_xx));
    //QSharedPointer<IApplications> iapp_xx(qSharedPointerCast<IApplications>(app_xx));
    // --

    QSharedPointer<LocalApplications> apps = storage.localApplications();
    QSharedPointer<IApplication> iapp = storage.localApplications()->application("app3-downloading-1.0.0");
    EXPECT_TRUE(iapp->state() == Application::Downloading);

    // TODO: dummy test: does taking new localApps disconnect old ones??
    QSharedPointer<LocalApplications> apps_yy = storage.localApplications();
    QSharedPointer<IApplication> iapp_yy = apps_yy->application("app3-downloading-1.0.0");
    QSharedPointer<Application> app_yy(qSharedPointerCast<Application>(iapp_yy));

    // CHANGE STATE TO VALID

    QSharedPointer<Application> app(qSharedPointerCast<Application>(iapp_xx)); app->markState(IApplication::Valid);
    LocalApplicationsStorage::Result updateResult; storage.updateApplication(*app.data(), updateResult);

    // there are events in event queue (signals)
    QCoreApplication::processEvents(QEventLoop::AllEvents, 5000);

    // existing LocalApplications should update itself;
    EXPECT_TRUE(apps->application("app3-downloading-1.0.0")->state() == Application::Valid);
    EXPECT_TRUE(apps_yy->application("app3-downloading-1.0.0")->state() == Application::Valid);
    EXPECT_TRUE(apps_xx->application("app3-downloading-1.0.0")->state() == Application::Valid);

}
// TODO: how downloading works?

// TODO: when signaling that new apps available

// TODO: how to have state? how to refresh it?

// TODO: test case if two apps with exactly same id -> fail second
