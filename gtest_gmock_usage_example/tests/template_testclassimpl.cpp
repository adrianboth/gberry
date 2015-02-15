#include "mock_qnetworkaccessmanager.h"

template <class PacketStream>
void PacketReader<PacketStream>::ReadPackets(PacketStream *stream)
{
    // nothing
    QNetworkReply* reply = stream->get(QNetworkRequest(QUrl("http://localhost:9000"))); // doesn't work, unknown address
}

#include <QNetworkAccessManager>

void TemporaryFunction ()
{
    PacketReader<QNetworkAccessManager> TempObj;
    Q_UNUSED(TempObj)
}
