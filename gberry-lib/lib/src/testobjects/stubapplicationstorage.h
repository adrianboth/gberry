#ifndef STUBAPPLICATIONSTORAGE_H
#define STUBAPPLICATIONSTORAGE_H

#include <QTemporaryDir>

#include "client/iapplicationstorage.h"

class TempDirApplicationStorage : public GBerryClient::IApplicationStorage {
    Q_DISABLE_COPY(TempDirApplicationStorage)

public:
    explicit TempDirApplicationStorage() {}

    virtual QString storagePath() const {
        return tempDir.path();
    }

    QTemporaryDir tempDir;
};

#endif // STUBAPPLICATIONSTORAGE_H
