#include "result.h"

#include <QSharedData>
#include <QMap>

namespace GBerryLib {

class Result::Reason::Private : public QSharedData {
public:
    Private() {}
    Private(const QString& code_) : code(code_) {} // non-localizable
    Private(const QString& code_, const QString& description) : code(code_), description(description) {}
    Private(const QString& code_, const QString& description, const QString& l10nContext_) : code(code_), description(description), l10nContext(l10nContext_) {}

    QString code;
    QString description;
    QString l10nContext;
    QMap<QString, Result::Meta> metas;
};

Result::Reason::Reason() :
    _d(new Private())
{

}

Result::Reason::Reason(const Result::Reason &other) :
    _d(other._d)
{
}

Result::Reason::Reason(const QString& code, const QString& description, const QString& l10nContext) :
    _d(new Result::Reason::Private(code, description, l10nContext))
{}

Result::Reason::~Reason()
{
}

QString Result::Reason::code() const
{
    return _d->code;
}
QString Result::Reason::description() const
{
    if (localizable()) {
       QString desc(_d->description);
       return desc.remove(0, 4); // TXT_
    } else {
       return _d->description;
    }

}

QString Result::Reason::l10nKey() const
{
    if (localizable())
        return _d->description;
    else
        return "";
}

QString Result::Reason::l10nContext() const
{
    return _d->l10nContext;
}

bool Result::Reason::localizable() const
{
    return _d->description.startsWith("TXT_");
}

Result::Reason &Result::Reason::operator=(const Result::Reason &other)
{
    _d->code = other.code();
    _d->description = other.description();
    _d->l10nContext = other.l10nContext();
    return *this;
}

Result::Reason &Result::Reason::operator<<(const Result::Meta &meta)
{
    _d->metas[meta.key()] = meta;
    return *this;
}

QList<Result::Meta> Result::Reason::metas() const
{
    return _d->metas.values();
}

bool Result::Reason::hasMeta(const QString& key) const
{
    return _d->metas.contains(key);
}

Result::Meta Result::Reason::meta(const QString &key) const
{
    if (hasMeta(key))
        return _d->metas[key];
    else
        return Result::Meta();
}

// -----------------------------------------------------------------------------

class Result::Meta::Private : public QSharedData {
public:
    Private() {}
    Private(const QString& key_, const QString& value_) :
        key(key_), value(value_) {}
    Private(const QString& key_, const QString& value_, const QString& l10nContext_) :
        key(key_), value(value_), l10nContext(l10nContext_) {}

    QString key;
    QString value;
    QString l10nContext{"Errors"};
};


Result::Meta::Meta() :
    _d(new Result::Meta::Private())
{
}

Result::Meta::Meta(const Result::Meta &other) :
    _d(other._d)
//    _d(new Result::Meta::Private(other.key(), other.value(), other.l10nContext()))
{
}

Result::Meta::Meta(const QString& key, const QString& value) :
    _d(new Result::Meta::Private(key, value))
{}

Result::Meta::~Meta()
{
}

QString Result::Meta::key() const
{
    return _d->key;
}

QString Result::Meta::value() const
{
    return _d->value;
}

QString Result::Meta::l10nKey() const
{
    if (localizable())
        return _d->value;
    else
        return "";
}

QString Result::Meta::l10nContext() const
{
    return _d->l10nContext;
}

QString Result::Meta::metaString()
{
    if (localizable()) {
        return QString(_d->value).remove(0, 4);
    } else {
        return _d->value;
    }
}

bool Result::Meta::localizable() const
{
    return _d->value.startsWith("TXT_");
}

bool Result::Meta::valid() const
{
    return !_d->key.isEmpty();
}

Result::Meta &Result::Meta::operator=(const Result::Meta &other)
{
    _d->key = other.key();
    _d->value = other.value();
    _d->l10nContext = other.l10nContext();
    return *this;
}

// -----------------------------------------------------------------------------

class Result::ResultPrivate : public QSharedData {
public:
    QList<Error> errors;
    QMap<QString, Meta> metas;
    QList<Reason> reasons;
    QList<Result> subresults;
};

Result::Reason Result::reasonFromCode(const QString &code, const QString &description, const QString &l10nContext)
{
    return Result::Reason(code, description, l10nContext);
}

Result::Reason Result::reasonFromDesc(const QString &description, const QString &l10nContext)
{
    return Result::Reason("", description, l10nContext);
}

Result::Result() : _d(new ResultPrivate())
{
}

Result::Result(const Result& other) :
  _d(other._d)
{
}

Result::Result(const Error& err) : _d(new ResultPrivate())
{
    _d->errors.append(err);
}

Result::Result(const QList<Error>& errors) : _d(new ResultPrivate())
{
    foreach(auto err, errors)
        _d->errors.append(err);
}

Result::~Result()
{
}

bool Result::hasErrors() const
{
    if (!_d->errors.isEmpty())
        return true;

    for (int i = 0; i < _d->subresults.length(); i++) {
        if (_d->subresults.at(i).hasErrors())
            return true;
    }

    return false;
}

QList<Error> Result::errors() const
{
    return _d->errors;
}

bool Result::hasSubresults() const
{
    return !_d->subresults.isEmpty();
}

QList<Result> Result::subresults() const
{
    return _d->subresults;
}

QList<Result::Meta> Result::metas() const
{
    return _d->metas.values();
}

bool Result::hasMeta(const QString& key) const
{
    return _d->metas.contains(key);
}

QString Result::metaValue(const QString &key) const
{
    if (hasMeta(key))
        return _d->metas[key].value();
    else
        return "";
}

Result::Meta Result::meta(const QString &key) const
{
    if (hasMeta(key))
        return _d->metas[key];
    else
        return Result::Meta();
}

bool Result::hasReasons() const
{
    return !_d->reasons.isEmpty();
}

QList<Result::Reason> Result::reasons() const
{
    return _d->reasons;
}

Result& Result::operator <<(const Error& item)
{
    _d->errors.append(item);
    return *this;
}

Result& Result::operator <<(const Meta& item)
{
    _d->metas[item.key()] = item;
    return *this;
}

Result& Result::operator <<(const Reason& item)
{
    _d->reasons.append(item);
    return *this;
}

Result& Result::operator <<(const Result& item)
{
    _d->subresults.append(item);
    return *this;
}

Result &Result::operator=(const Result &other)
{
    _d->errors = other._d->errors;
    _d->metas = other._d->metas;
    _d->reasons = other._d->reasons;
    _d->subresults = other._d->subresults;
    return *this;
}

} // eon
