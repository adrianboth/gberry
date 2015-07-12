#ifndef COMMSCONFIG_H
#define COMMSCONFIG_H

class QSettings;

#include "commsparameters.h"
using namespace Comms;

namespace GBerryComms {

/**
 * CommsConfig is a configuration class and its values are read form a
 * *.ini file.
 */
class CommsConfig
{
public:
    // parameters used to find config file
    CommsConfig(CommsParameters* parameters);
    ~CommsConfig();

    bool hasKey(const QString& key) const;
    QString value(const QString& key) const;
    QString value(const QString& key, const QString& defaultValue) const;

private:
    QSettings* _settings;
};

} // eon

#endif // COMMSCONFIG_H
