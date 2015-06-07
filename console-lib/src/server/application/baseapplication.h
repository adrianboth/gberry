#ifndef BASEAPPLICATION_H
#define BASEAPPLICATION_H

#include <QSharedPointer>

#include "server/application/iapplication.h"
#include "server/application/applicationmeta.h"

namespace GBerry {
    namespace Console {
        namespace Server {


/**
 * @brief Very basic implementation of IApplication
 *
 * This class is usefull for tests, but also as intermediate class for
 * advanced implementations.
 */
class BaseApplication : public IApplication
{

public:
    BaseApplication(QSharedPointer<ApplicationMeta> meta);
    virtual ~BaseApplication();

    // IApplication
    virtual State state() const override;
    virtual QString id() const override;
    virtual QSharedPointer<IApplicationMeta> meta() const override;

    // --
    void markState(State newState);

private:
    QSharedPointer<IApplicationMeta> _meta;
    State _state;
};

}}} // eon

#endif // BASEAPPLICATION_H
