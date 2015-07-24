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
 
 #ifndef RESULT_H
#define RESULT_H

#include <QString>
#include <QList>
#include <QSharedDataPointer>

#include "error.h"

namespace GBerryLib {

class Result
{
public:
    class Meta {
    public:
        Meta();
        Meta(const Meta& other);
        Meta(const QString& key, const QString& value);
        Meta(const QString& key, const QString& value, const QString& l10nContext);
        virtual ~Meta();

        QString key() const;
        QString value() const;
        QString l10nKey() const;
        QString l10nContext() const;
        QString metaString();
        bool localizable() const;
        bool valid() const;
        Meta &operator=(const Meta& other);

    private:
        class Private;
        QSharedDataPointer<Private> _d;

    };


    class Reason {
    public:
        Reason();
        Reason(const Reason& other);
        Reason(const QString& code, const QString& description, const QString& l10nContext);
        virtual ~Reason();

        QString code() const;
        QString description() const;
        QString l10nKey() const;
        QString l10nContext() const;
        bool localizable() const;
        Reason &operator=(const Reason& other);

        Reason& operator <<(const Meta& item);

        QList<Meta> metas() const;
        bool hasMeta(const QString& key) const;
        Meta meta(const QString& key) const;

    private:
        class Private;
        QSharedDataPointer<Private> _d;

    };

    static Reason reasonFromCode(const QString& code, const QString& description = QString(), const QString& l10nContext = QString("Errors"));
    static Reason reasonFromDesc(const QString& description, const QString& l10nContext = QString("Errors"));


    // -------------------------------------------------------------------------

    Result();
    Result(const Result& other);
    Result(const Error& err);
    Result(const QList<Error>& errors);
    virtual ~Result();

    bool hasErrors() const;
    QList<Error> errors() const;

    bool hasSubresults() const;
    QList<Result> subresults() const;

    QList<Meta> metas() const;
    bool hasMeta(const QString& key) const;
    Meta meta(const QString& key) const;
    QString metaValue(const QString& key) const;

    bool hasReasons() const;
    QList<Reason> reasons() const;

    Result& operator <<(const Error& item);
    Result& operator <<(const Meta& item);
    Result& operator <<(const Reason& item);
    Result& operator <<(const Result& item);

    Result& operator=(const Result& other);

private:
    class ResultPrivate;
    QSharedDataPointer<ResultPrivate> _d;
};

} // eon

Q_DECLARE_TYPEINFO(GBerryLib::Result, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(GBerryLib::Result::Meta, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(GBerryLib::Result::Reason, Q_MOVABLE_TYPE);

#endif // RESULT_H
