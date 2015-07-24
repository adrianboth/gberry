/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
 #include "error.h"

#include <QSharedData>

namespace GBerryLib {

class Error::ErrorPrivate : public QSharedData {
public:
    ErrorPrivate(uint code_) : code(code_) {}
    ErrorPrivate(uint code_, const QString& name_) : code(code_), name(name_) {}
    ErrorPrivate(uint code_, const QString& name_, const QString& description_) : code(code_), name(name_), description(description_) {}
    ErrorPrivate(uint code_, const QString& name_, const QString& description_, const QString& l10nContext_) : code(code_), name(name_), description(description_), l10nContext(l10nContext_) {}

    int code{0};
    QString name;
    QString description;
    QString l10nContext{"Errors"};
};

Error::Error(uint code) :
    _d(new ErrorPrivate(code))
{}

Error::Error(uint code, const QString &name) :
    _d(new ErrorPrivate(code, name))
{}

Error::Error(uint code, const QString &name, const QString &description) :
    _d(new ErrorPrivate(code, name, description))
{}

Error::Error(uint code, const QString &name, const QString &description, const QString &l10nContext) :
    _d(new ErrorPrivate(code, name, description, l10nContext))
{}

Error::Error(const Error &err) :
    _d(err._d)
{}

Error::~Error()
{
}

int Error::code() const
{
    return _d->code;
}

QString Error::name() const
{
    return _d->name;
}

QString Error::description() const
{
    if (localizable()) {
        QString desc(_d->description);
        return desc.remove(0, 4); // TXT_
    } else {
        return _d->description;
    }

}

QString Error::errorL10nKey() const
{
    if (localizable()) {
        return _d->description;
    } else {
        return "";
    }
}

QString Error::errorL10nContext() const
{
    return _d->l10nContext;
}

bool Error::localizable() const
{
    return _d->description.startsWith("TXT_");
}

Error &Error::operator=(const Error &err)
{
    _d->code = err.code();
    _d->name = err.name();
    _d->description = err.description();
    _d->l10nContext = err.errorL10nContext();
    return *this;
}

bool Error::operator==(const Error &err) const
{
    return _d->code == err.code();
}

} // eon
