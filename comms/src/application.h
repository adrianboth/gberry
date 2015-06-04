#ifndef APPLICATION_H
#define APPLICATION_H

#include <QString>
#include <QSharedPointer>

#include "applicationmeta.h"


class Application
{

public:
    Application(QSharedPointer<ApplicationMeta> meta);
    virtual ~Application();

    enum State { Valid, Invalid, Downloading };
    State state() const;
    void markState(State newState);

    QString id() const;

    QSharedPointer<ApplicationMeta> meta() const;

private:
    QSharedPointer<ApplicationMeta> _meta;
    State _state;
};

#endif // APPLICATION_H
