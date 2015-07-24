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
 
 #include "resultmessageformatter.h"

#include <QString>
#include <QJsonArray>
#include <QCoreApplication>
#include <QRegExp>
#include <QDebug>

namespace GBerryLib {

class Metas {
public:
    Metas(const Result& result) : _result(result), _reason(Result::Reason()) {}
    Metas(const Result& result, const Result::Reason& reason) :
        _result(result), _reason(reason) {}

    bool hasMeta(const QString& key) const {
        return _result.hasMeta(key) || _reason.hasMeta(key);
    }
    Result::Meta meta(const QString& key) const {
        if (_reason.hasMeta(key))
            return _reason.meta(key);
        else
            return _result.meta(key);
    }

private:
    Result _result;
    Result::Reason _reason;
};

static QString createDeveloperMessageFrom(const Result& result);
static QString createEndUserMessageFrom(const Result& result); // TODO: localized
static QJsonObject jsonFrom(const Result& result);
static QString translateError(const Error& err, const Result& result);
static QString translateReason(const Result::Reason& err, const Result& result);
static QString expand(const QString& err, const Metas& metas, bool localize);



ResultMessageFormatter::ResultMessageFormatter(const Result &result) :
    _result(result)
{
}

ResultMessageFormatter::~ResultMessageFormatter()
{
}

QString ResultMessageFormatter::createDeveloperMessage() const
{
    return createDeveloperMessageFrom(_result);
}

QString ResultMessageFormatter::createEndUserMessage() const
{
    return createEndUserMessageFrom(_result);
}

QJsonObject ResultMessageFormatter::toJson() const
{
    return jsonFrom(_result);
}

// -----------------------------------------------------------------------------

QString createEndUserMessageFrom(const Result& result)
{
    // TODO: localization
    if (result.errors().isEmpty())
        return QT_TRANSLATE_NOOP("Errors", "TXT_No errors.");

    QString msg;

    foreach(Error err, result.errors()) {
        if (msg.size() > 0)
            msg.append(" "); // space between sentences

        if (err.localizable()) {
            QString errmsg = translateError(err, result);
            msg.append(errmsg);

            if (!errmsg.endsWith("."))
                msg.append(".");
        }
    }

    if (!result.reasons().isEmpty()) {
        bool firstLocalizableFound = false;

        foreach(Result::Reason reason, result.reasons()) {
            if (firstLocalizableFound && msg.size() > 0)
                msg.append(" "); // space between sentences

            if (reason.localizable()) {
                // now there is at least one localized reason convert delimiter between errors and reasons
                // between errors and reasons there is ":"
                if (!firstLocalizableFound) {
                    if (msg.endsWith(".")) {
                        msg.replace(msg.length() - 1, 1, ":");
                        msg.append((" "));
                    }
                    firstLocalizableFound = true;
                }
                QString errmsg = translateReason(reason, result);
                msg.append(errmsg);

                if (!errmsg.endsWith("."))
                    msg.append(".");
            }
        }
    }

    foreach(Result subresult, result.subresults()) {
        QString submsg = createEndUserMessageFrom(subresult);
        if (submsg.length() > 0) {
            if (msg.size() > 0)
                msg.append(" "); // space between sentences

            msg.append(submsg);
        }
    }

    return msg;
}

QString createDeveloperMessageFrom(const Result& result)
{
    if (result.errors().isEmpty())
        return "No errors.";

    QString msg;

    foreach(Error err, result.errors()) {
        if (msg.size() > 0)
            msg.append(" "); // space between sentences

        QString errmsg = expand(err.description(), result, false);
        msg.append(errmsg);

        if (!errmsg.endsWith("."))
            msg.append(".");
    }

    if (!result.reasons().isEmpty()) {
        // between errors and reasons there is ":"
        if (msg.endsWith("."))
            msg.replace(msg.length() - 1, 1, ":");

        foreach(Result::Reason reason, result.reasons()) {
            if (msg.size() > 0)
                msg.append(" "); // space between sentences

            QString errmsg = expand(reason.description(), Metas(result, reason), false);
            msg.append(errmsg);

            if (!errmsg.endsWith("."))
                msg.append(",");
            else
                msg.replace(msg.length() - 1, 1, ',');
        }

        if (msg.endsWith(","))
            msg.replace(msg.length() - 1, 1, '.');
    }

    foreach(Result subresult, result.subresults()) {
        QString submsg = createDeveloperMessageFrom(subresult);
        if (submsg.length() > 0) {
            if (msg.size() > 0)
                msg.append(" "); // space between sentences

            msg.append(submsg);
        }
    }

    return msg;
}

QString translateError(const Error &err, const Result& result)
{
    return expand(QCoreApplication::translate(
                      err.errorL10nContext().toUtf8(),
                      err.errorL10nKey().toUtf8()),
                  Metas(result),
                  true);
}

QString translateReason(const Result::Reason &reason, const Result& result)
{
    return expand(QCoreApplication::translate(
                      reason.l10nContext().toUtf8(),
                      reason.l10nKey().toUtf8()),
                  Metas(result, reason),
                  true);
}

QString expand(const QString& msg, const Metas& metas, bool localize)
{
    QRegExp rx("#\\{([a-zA-Z_0-9]+)\\}");
    QString m(msg);
    int pos = 0;
    while ((pos = rx.indexIn(msg, pos)) != -1) {
        QString key = rx.cap(1);
        if (metas.hasMeta(key)) {
            Result::Meta meta = metas.meta(key);
            QString metaStr;
            if (localize && meta.localizable()) {
                metaStr = QCoreApplication::translate(meta.l10nContext().toUtf8(), meta.l10nKey().toUtf8());
            } else {
                metaStr = meta.metaString();
            }
            m.replace(rx.cap(0), metaStr);
        }
        pos += rx.matchedLength();
    }

    return m;
}

QJsonObject jsonFrom(const Result& result)
{
    QJsonObject json;
    json["error_message"] = createEndUserMessageFrom(result); // TODO: should have both, localized and
    json["error_string"] = createDeveloperMessageFrom(result); // TODO: should have both, localized and

    if (!result.errors().isEmpty()) {
        QJsonArray errors;

        foreach(Error err, result.errors()) {
            QJsonObject errJson;
            errJson["code"] = err.code();
            errJson["name"] = err.name();

            errors.append(errJson);
        }
        json["errors"] = errors;
    }

    if (!result.reasons().isEmpty()) {
        QJsonArray reasons;

        foreach(Result::Reason reason, result.reasons()) {
            QJsonObject reasonJson;

            // 'code' is optional for reason
            if (!reason.code().isEmpty())
                reasonJson["code"] = reason.code();

            reasonJson["description"] = reason.description();
            reasons.append(reasonJson);
        }
        json["reasons"] = reasons;
    }

    if (!result.metas().isEmpty()) {
        QJsonArray metas;

        foreach(Result::Meta meta, result.metas()) {
            QJsonObject metaJson;
            metaJson["key"] = meta.key();
            metaJson["value"] = meta.value();
            metas.append(metaJson);
        }
        json["metas"] = metas;
    }

    if (!result.subresults().isEmpty()) {
        QJsonArray subresults;

        foreach(Result sub, result.subresults()) {
            subresults.append(jsonFrom(sub));
        }
        json["subresults"] = subresults;

    }

    return json;
}

} // eon
