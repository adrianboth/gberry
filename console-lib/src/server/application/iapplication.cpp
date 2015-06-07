#include "iapplication.h"

#define LOG_AREA "IApplication"
#include "log/log.h"

namespace GBerry {
    namespace Console {
        namespace Server {

IApplication::IApplication()
{
}

IApplication::~IApplication()
{
}

QString IApplication::stateString() const
{
    switch (state()) {
    case Valid:
        return "valid";
    case Invalid:
        return "invalid";
    case Downloading:
        return "downloading";

    default:
        ERROR("Unknown application state" << state() << ". Can't convert to string.");
        return "";
    }
}

QString IApplication::id() const
{
    return meta()->applicationId() + "-" + meta()->version();
}

}}} // eon

