#ifndef APPLICATIONPARAMETERS_H
#define APPLICATIONPARAMETERS_H

#include "runtimeparameters.h"

namespace GBerryApplication {

class ApplicationParameters : public GBerryConsoleCommon::RuntimeParameters
{
public:
    explicit ApplicationParameters(IEnvironmentVariables& env);
    virtual ~ApplicationParameters();

    static const QString APPLICATION_CODE;
};

} // eon

#endif // APPLICATIONPARAMETERS_H
