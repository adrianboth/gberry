#include "application2json.h"

namespace GBerry {
    namespace Console {
        namespace Server {

QJsonObject Application2Json::from(const IApplication& app)
{
    QJsonObject json;

    json["id"] = app.id();
    json["version"] = app.meta()->version();
    json["application_id"] = app.meta()->applicationId();
    json["name"] = app.meta()->name();
    json["description"] = app.meta()->description();
    json["catalog_image"] = app.meta()->catalogImageFilePath();
    json["status"] = app.stateString();

    return json;
}

}}} // eon
