#ifndef REALSYSTEMSERVICES_H
#define REALSYSTEMSERVICES_H

#include "systemservices.h"

class RealSystemServices : public SystemServices
{
public:
    RealSystemServices();
    ~RealSystemServices();

    virtual void singleshotTimer(int msec, QObject *receiver, const char *member);

    void registerItself();
};

#endif // REALSYSTEMSERVICES_H
