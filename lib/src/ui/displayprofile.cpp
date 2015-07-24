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
 
 #include "displayprofile.h"

#include <QGuiApplication>
#include <QProcessEnvironment>
#include <QScreen>
#include <QDebug>

#include "actualdisplay.h"

// TODO: later support for reading from a file
namespace {
    // TODO: these should on header side because of documentation!!

    static const char* PREF_WINDOW_WIDTH_VARNAME = "PREF_WINDOW_WIDTH";
    static const char* PREF_WINDOW_HEIGHT_VARNAME = "PREF_WINDOW_HEIGHT";
    static const char* WINDOW_SCALE_FACTOR_VARNAME = "WINDOW_SCALE_FACTOR";
    static const char* WINDOW_EMULATION_MODE_VARNAME = "WINDOW_EMULATION_MODE";

}
DisplayProfile::DisplayProfile(int defaultWindowWidth,
                               int defaultWindowHeight,
                               Mode defaultMode,
                               double defaultSizeScaleFactor,
                               Display* actualDisplay,
                               QObject *parent) :
    QObject(parent),
    _mode(defaultMode),
    _preferredWindowWidth(defaultWindowWidth),
    _preferredWindowHeight(defaultWindowHeight),
    _sizeScaleFactor(defaultSizeScaleFactor),
    _targetDisplay(NULL),
    _actualDisplay(actualDisplay),
    _autoscaling(false)
{

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    if (env.contains(PREF_WINDOW_WIDTH_VARNAME)) {
        _preferredWindowWidth = env.value(PREF_WINDOW_WIDTH_VARNAME).toInt();
    }

    if (env.contains(PREF_WINDOW_HEIGHT_VARNAME)) {
        _preferredWindowHeight = env.value(PREF_WINDOW_HEIGHT_VARNAME).toInt();
    }

    if (env.contains(WINDOW_SCALE_FACTOR_VARNAME)) {
        _sizeScaleFactor = env.value(WINDOW_SCALE_FACTOR_VARNAME).toDouble();
        qDebug() << "[DisplayProfile] read from env: sizeScaleFactor =" << _sizeScaleFactor;
    }

    if (env.contains(WINDOW_EMULATION_MODE_VARNAME)) {
        QString modeStr = env.value(WINDOW_EMULATION_MODE_VARNAME).toLower();
        if (modeStr == "pixel") {
            _mode = PixelMatch;
        } else if (modeStr == "dimension") {
            _mode = DimensionMatch;
        }
        qDebug() << "[DisplayProfile] read from env: mode =" << modeStr;
    }

    if (!_actualDisplay) {
        QList<QScreen*> screens = QGuiApplication::screens();
        // by default pick first one
        _actualDisplay = new ActualDisplay(screens.at(0), this);
    }

    // auto scale
    if (_sizeScaleFactor == 0.0) {
        _autoscaling = true;
        recalculateAutoScale();
    }
}

DisplayProfile::~DisplayProfile()
{

}

int DisplayProfile::windowWidth() const
{
    if (_targetDisplay) {
        //qDebug() << "TEST: widthPixels: " << _targetDisplay->widthPixels();

        if (_mode == PixelMatch) {
            return validateWidth(_targetDisplay->widthPixels() * _sizeScaleFactor);
        }
        else
        {
            // dimension match
            //qDebug() << "TEST2: inches: " << _targetDisplay->widthInches();
            return validateWidth(_targetDisplay->widthInches() * _actualDisplay->xdpi() * _sizeScaleFactor);
        }
    }
    //qDebug() << "TEST3: window width: " << _preferredWindowWidth;
    return validateWidth(_preferredWindowWidth * _sizeScaleFactor);
}

int DisplayProfile::windowHeight() const
{
    if (_targetDisplay) {
        if (_mode == PixelMatch) {
            return validateHeigth(_targetDisplay->heightPixels() * _sizeScaleFactor);
        }
        else
        {
            return validateHeigth(_targetDisplay->heightInches() * _actualDisplay->ydpi() * _sizeScaleFactor);
        }
    }
    return validateHeigth(_preferredWindowHeight * _sizeScaleFactor);
}

double DisplayProfile::xdpi() const
{
    if (_mode == PixelMatch) {
        if (_targetDisplay) {
            //qDebug() << "[DisplayProfile] USING: xdpi: " << _targetDisplay->xdpi();
            return _targetDisplay->xdpi() * _sizeScaleFactor;
        }

    }

    // default
    return _actualDisplay->xdpi() * _sizeScaleFactor;
}

double DisplayProfile::ydpi() const
{
    if (_mode == PixelMatch) {
        if (_targetDisplay)
            return _targetDisplay->ydpi() * _sizeScaleFactor;
    }

    // default
    return _actualDisplay->ydpi() * _sizeScaleFactor;

}

void DisplayProfile::useTargetDisplay(Display *targetDisplay)
{
    // memory release by QObject owner
    _targetDisplay = targetDisplay;

    // auto scale
    recalculateAutoScale();
}

// TODO: if real device then match real screen properties

int DisplayProfile::validateWidth(int w) const
{
    if (w <= 0) {
        qCritical() << "[DisplayProfile] Requested width was invalid: " << w;
        return _actualDisplay->widthPixels();
    }
    if (w > _actualDisplay->widthPixels())
        qWarning() << "[DisplayProfile] Requested width is larger than screen:" << w << ">" << _actualDisplay->widthPixels();

    // we accept larger than screen (to zoom for testing), but avoid excess
    if (w > 2*_actualDisplay->widthPixels())
        return _actualDisplay->widthPixels();

    return w;
}

int DisplayProfile::validateHeigth(int h) const
{
    if (h <= 0) {
        qCritical() << "[DisplayProfile] Requested width was invalid: " << h;
        return _actualDisplay->heightPixels();
    }

    if (h > _actualDisplay->heightPixels())
        qWarning() << "[DisplayProfile] Requested height is larger than screen:" << h << ">" << _actualDisplay->heightPixels();

    // we accept larger than screen (to zoom for testing), but avoid excess
    if (h > 2*_actualDisplay->heightPixels())
        return _actualDisplay->heightPixels();

    return h;
}

void DisplayProfile::recalculateAutoScale()
{
    // auto scale
    if (_autoscaling) {
        if (_targetDisplay) {
            if (_mode == PixelMatch) {
                double byWidth = _actualDisplay->widthPixels() / (double) _targetDisplay->widthPixels();
                double byHeight = _actualDisplay->heightPixels() / (double)_targetDisplay->heightPixels();
                _sizeScaleFactor = byWidth < byHeight ? byWidth : byHeight;
                qDebug() << "[DisplayProfile] pixel autoscale" << _sizeScaleFactor;
            } else {
                double byWidth = _actualDisplay->widthInches() / (double) _targetDisplay->widthInches();
                double byHeight = _actualDisplay->heightInches() / (double) _targetDisplay->heightInches();
                _sizeScaleFactor = byWidth < byHeight ? byWidth : byHeight;
                qDebug() << "[DisplayProfile] dimension autoscale" << _sizeScaleFactor;
            }
        } else {
            // as we don't have targetDisplay we need to assume dpi values are same
            //  -> mode doesn't have meaning
            double byWidth = _actualDisplay->widthPixels() / (double)_preferredWindowWidth;
            double byHeight = _actualDisplay->heightPixels() / (double)_preferredWindowHeight;
            _sizeScaleFactor = byWidth < byHeight ? byWidth : byHeight;
            qDebug() << "[DisplayProfile] defaut autoscale" << _sizeScaleFactor;
        }
    }
}
