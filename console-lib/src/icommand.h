#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <QObject>
#include <QString>
#include <QJsonObject>

class ICommandResponse
{
public:
    virtual void set(QJsonObject json) = 0;
};

class ICommand : public QObject
{
    Q_OBJECT

public:
    ICommand(const QString& id) : _id(id) {}
    ~ICommand() {}

    QString id() const { return _id; }

    virtual bool process(const QJsonObject& json, ICommandResponse& response) = 0;

private:
    QString _id;
};

#endif // ICOMMAND_H

