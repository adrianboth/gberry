#ifndef COMMSPARAMETERS_H
#define COMMSPARAMETERS_H

#include "systemservices.h"
#include "common/runtimeparameters.h"

namespace Comms
{

/**
 * @brief Parameter handler
 *
 * Handles parameters for 'comms' application from various sources:
 *  - command line
 *  - environment variables
 *  - built time
 *
 * Contains a generic approach but also option specific handling.
 *
 * Typical order of predescence (top most effective if defined):
 *  - command line
 *  - environment variable
 *  - build time default
 */
class CommsParameters : public GBerryConsoleCommon::RuntimeParameters
{
public:
    explicit CommsParameters(IEnvironmentVariables& env);
    virtual ~CommsParameters();

    static const QString ROOT_PATH;
    static const QString DISABLE_WAITAPP;
    static const QString DISABLE_MAINUI;
    static const QString COMMS_CONFIG;
};

} // eon

#endif // COMMSPARAMETERS_H
