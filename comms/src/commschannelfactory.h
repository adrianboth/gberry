#ifndef COMMSCHANNELFACTORY_H
#define COMMSCHANNELFACTORY_H

#include "server/channelfactory.h"

class CommsChannelFactory : public ChannelFactory
{
public:
    CommsChannelFactory(QSharedPointer<IApplications> apps);
    ~CommsChannelFactory();

    virtual ControlChannel* createControlChannel(int connectionId) override;

private:
    QSharedPointer<IApplications> _iapps;
};

#endif // COMMSCHANNELFACTORY_H
