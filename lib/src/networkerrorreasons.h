#ifndef NETWORKERRORREASONS_H
#define NETWORKERRORREASONS_H

#include <QNetworkReply>

#include "result.h"

namespace GBerryLib {

class NetworkErrorReasons
{
public:
    NetworkErrorReasons();
    ~NetworkErrorReasons();

    static Result::Reason from(QNetworkReply::NetworkError error);
};

}

#endif // NETWORKERRORREASONS_H
