#include <testutils/qtgtest.h>

#include <QTemporaryDir>

#include "applicationconfigreaderwriter.h"
#include "application.h"
#include "server/application/applicationmeta.h"

#define LOG_AREA "ApplicationConfigReaderWriterTests"
#include "log/log.h"

TEST(ApplicationConfigReaderWriter, WriteNewAndReadOk)
{
    // there needs to be (at this point of implemenation work) ready cached app
    QSharedPointer<ApplicationMeta> meta(new ApplicationMeta);
    QSharedPointer<Application> app(new Application(meta));

    meta->setApplicationId("TestOffering");
    meta->setVersion("1.0");
    meta->setName("Test Offering");
    meta->setDescription("test desc");

    QTemporaryDir tempDir;
    QDir d(tempDir.path());
    // as we writing fully new app specific dir doesn't exist
    // TODO: should dir logic be in readerwriter?? now in applicationstorage
    QString appdir(d.filePath("testoffering-1.0"));
    DEBUG("Writing to" << appdir);
    ApplicationConfigReaderWriter writer(appdir);

// -- test

    ApplicationConfigReaderWriter::Result result;
    bool writeResult = writer.writeApplication(*app.data(), result);

    EXPECT_TRUE(writeResult);
    ASSERT_FALSE(result.hasError()) << result.errorString;

    QDir dirObj(appdir);
    QString cfgFilePath = dirObj.filePath("appcfg.json");

    ASSERT_TRUE(QFileInfo(cfgFilePath).exists());

// --
    ApplicationConfigReaderWriter reader(appdir);
    QSharedPointer<Application> readApp = reader.readApplication();
    ASSERT_FALSE(readApp.isNull());

    EXPECT_TRUE(readApp->id() == app->id());
    EXPECT_TRUE(readApp->meta()->name() == app->meta()->name());
    EXPECT_TRUE(readApp->meta()->description() == app->meta()->description());

}
