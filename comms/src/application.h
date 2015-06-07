#ifndef APPLICATION_H
#define APPLICATION_H

#include <QString>
#include <QSharedPointer>

// TODO: we could have lib in path "console"
#include "server/application/baseapplication.h"

namespace using GBerryConsoleServer;

// TODO: now just wrapper, lets see if we need to have this at all
class Application : public BaseApplication
{

public:
    Application(QSharedPointer<ApplicationMeta> meta);
    virtual ~Application();

};

#endif // APPLICATION_H
