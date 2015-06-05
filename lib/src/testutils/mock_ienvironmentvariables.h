#ifndef MOCK_IENVIRONMENTVARIABLES_H
#define MOCK_IENVIRONMENTVARIABLES_H

#include "systemservices.h"

#include <QMap>

#include <gmock/gmock.h>


class MockIEnvironmentVariables : public IEnvironmentVariables
{
public:
    MOCK_CONST_METHOD1(contains, bool(const QString&));
    MOCK_CONST_METHOD1(value, QString(const QString&));
    MOCK_CONST_METHOD2(value, QString(const QString&, const QString& defaultValue));
};

class StubIEnvironmentVariables : public IEnvironmentVariables
{
public:
    StubIEnvironmentVariables(QMap<QString, QString>& _values) : _map(_values) {}
    virtual ~StubIEnvironmentVariables() {}

    virtual bool contains(const QString &key) const override {
        return _map.contains(key);
    }

    virtual QString value(const QString &key, const QString &defaultValue) const {
        return _map.value(key, defaultValue);
    }
private:
    QMap<QString, QString>& _map;
};

#endif // MOCK_IENVIRONMENTVARIABLES_H

