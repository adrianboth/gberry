#ifndef APPLICATION_H
#define APPLICATION_H

#include <QString>
#include <QSharedPointer>

// TODO: we could have lib in path "console"
#include "server/application/baseapplication.h"

using namespace GBerry::Console::Server;

// TODO: now just wrapper, lets see if we need to have this at all
class Application : public BaseApplication
{

public:
    // standard constructor
    explicit Application(QSharedPointer<ApplicationMeta> meta);
    virtual ~Application();

    ApplicationMeta& metaRef();

    static QSharedPointer<Application> copy(QSharedPointer<IApplication> iapp);
};

#endif // APPLICATION_H
