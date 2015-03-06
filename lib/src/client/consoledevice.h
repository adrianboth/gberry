#ifndef CONSOLEDEVICE_H
#define CONSOLEDEVICE_H

#include <QString>

namespace mobile
{

class ConsoleDevice
{
public:
    ConsoleDevice(QString hostAddress);
    ~ConsoleDevice();

    QString host() { return _hostAddress; }

private:
    QString _hostAddress;
};

} // eon

#endif // CONSOLEDEVICE_H
