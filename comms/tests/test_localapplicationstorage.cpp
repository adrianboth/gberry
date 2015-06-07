
#include "localapplicationsstorage.h"

#include "testutils/waiter.h"
#include "testutils/qtgtest.h"
#include "testutils/util_enablelog.h"
#include "testutils/util_testdata.h"


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


// TODO: how downloading works?

// TODO: when signaling that new apps available

// TODO: how to have state? how to refresh it?

// TODO: test case if two apps with exactly same id -> fail second
