#ifndef APPLICATION2JSON_H
#define APPLICATION2JSON_H

#include <QJsonObject>

#include "server/application/iapplication.h"

namespace GBerry {
    namespace Console {
        namespace Server {

class Application2Json
{
public:
    static QJsonObject from(const IApplication& app);
};

}}} // eon
#endif // APPLICATION2JSON_H
