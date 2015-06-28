#ifndef APPLICATIONPARAMETERS_H
#define APPLICATIONPARAMETERS_H

#include "common/runtimeparameters.h"

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
