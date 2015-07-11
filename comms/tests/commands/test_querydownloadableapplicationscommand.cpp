#include <testutils/qtgtest.h>

#include "commands/querydownloadableapplicationscommand.h"

using namespace GBerry;

class TestCommandResponse : public ICommandResponse
{
public:
    void set(QJsonObject json_) override { json = json_; }
    QJsonObject json;
};


TEST(QueryDownloadableApplicationsCommand, OK)
{
    /*
    DownloadableApplicationCache applicationCache; // so simple that no mock need ...
    HeadServerConnection headServerConnection; // TODO: some interface for this
    ServerSideControlChannel controlChannel; // TODO: some interface for message sending

    QueryDownloadableApplicationsCommand cmd();
    */
}

