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

    int code() const;
    QString name() const;

    /**
     * Returns developer description of error. If original description contains
     * TXT_ prefix (meaning localizable key) it is removed.
     */
    QString description() const;
    /**
     * Returns a localization key, which is a description if it has TXT_ prefix.
     * If not localizable then empty string is returned, i.e. no localization.
     */
    QString errorL10nKey() const;
    QString errorL10nContext() const;

    /**
     * Returns true if description can be localized. If localizable then
     * errorL10Key() returns non-empty localization key. Error description is
     * marked to be localizable by putting TXT_ prefix into description.
     */
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
