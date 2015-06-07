#ifndef IAPPLICATION
#define IAPPLICATION

#include <QSharedPointer>

#include "iapplicationmeta.h"

namespace GBerry {
    namespace Console {
        namespace Server {

class IApplication
{

public:
    IApplication();
    virtual ~IApplication();

    enum State { Valid, Invalid, Downloading };
    virtual State state() const = 0;

    virtual QString stateString() const;
    virtual QString id() const;

    // TODO: if IApplication and meta are tightly coupled -> do we need Qsharedpointer?
    virtual QSharedPointer<IApplicationMeta> meta() const = 0;
};

}}} // eon

#endif // IAPPLICATION

