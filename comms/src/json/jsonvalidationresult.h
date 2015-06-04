#ifndef JSONVALIDATIONRESULT_H
#define JSONVALIDATIONRESULT_H

#include <QStringList>

class JsonValidationResult
{
public:
    JsonValidationResult() {}
    ~JsonValidationResult() {}
    bool hasErrors() const          { return _errors.isEmpty(); }
    void appendError(QString error) { _errors.append(error); }
    void appendErrors(QStringList errors) { _errors << errors; }
    QStringList errors() const      { return _errors; }
    void clear()                    { _errors.clear(); }

private:
    QStringList _errors;
};

#endif // JSONVALIDATIONRESULT_H
