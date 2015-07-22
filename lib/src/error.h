#ifndef ERROR_H
#define ERROR_H

#include <QString>
#include <QList>
#include <QSharedDataPointer>

#define ERRORCLASS(newClass) class newClass : public GBerryLib::Error {\
    public: \
        newClass(uint code) : GBerryLib::Error(code) {} \
        newClass(uint code, const QString& name) : GBerryLib::Error(code, name) {} \
        newClass(uint code, const QString& name, const QString& description) : GBerryLib::Error(code, name, description) {} \
        newClass(uint code, const QString& name, const QString& description, const QString& l10nContext) : GBerryLib::Error(code, name, description, l10nContext) {} \
        virtual ~newClass() {} \
    };

namespace GBerryLib {

// WHAT, WHY
class Error
{
public:
    Error(uint code);
    Error(uint code, const QString& name);
    Error(uint code, const QString& name, const QString& description);
    Error(uint code, const QString& name, const QString& description, const QString& l10nContext);
    Error(const Error& err);
    virtual ~Error();

    uint code() const;
    QString name() const;
    QString description() const;
    QString errorL10nKey() const;
    QString errorL10nContext() const;
    bool localizable() const;

    Error& operator=(const Error& err);
    bool operator==(const Error&err) const;

private:
    class ErrorPrivate;
    QSharedDataPointer<ErrorPrivate> _d;
};

} // eon

Q_DECLARE_TYPEINFO(GBerryLib::Error, Q_MOVABLE_TYPE);

#endif // ERROR_H
