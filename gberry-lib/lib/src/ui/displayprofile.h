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
 
 #ifndef DISPLAYPROFILE_H
#define DISPLAYPROFILE_H

#include <QObject>

#include "targetdisplay.h"
#include "display.h"

/**
 * This class support development time scaling of display to match
 * target hardware display specifications.
 *
 */
class DisplayProfile : public QObject
{
    Q_OBJECT

public:
    enum Mode { PixelMatch, DimensionMatch };

    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(int windowWidth READ windowWidth NOTIFY windowWidthChanged)
    Q_PROPERTY(int windowHeight READ windowHeight NOTIFY windowHeightChanged)
    Q_PROPERTY(double sizeScaleFactor READ sizeScaleFactor NOTIFY sizeScaleFactorChanged)
    Q_PROPERTY(double xdpi READ xdpi NOTIFY xdpiChanged)
    Q_PROPERTY(double ydpi READ ydpi NOTIFY ydpiChanged)


    DisplayProfile(int defaultWindowWidth,
                   int defaultWindowHeight,
                   Mode defaultMode = DisplayProfile::PixelMatch,
                   double defaultSizeScaleFactor = 0.0, // auto scale
                   Display* actualDisplay = NULL,
                   QObject* parent = 0);

    ~DisplayProfile();

    Mode mode() const { return _mode; }
    Q_INVOKABLE QString modeString() const { return _mode == PixelMatch ? "pixel" : "dimension"; }
    Q_INVOKABLE bool isTargetHardware() const;

    int windowWidth() const;
    int windowHeight() const;
    double sizeScaleFactor() const { return _sizeScaleFactor; }
    double xdpi() const;
    double ydpi() const;

    void setMode(Mode newMode) { _mode = newMode; recalculateAutoScale(); modeChanged(); }

    void useTargetDisplay(Display* targetDisplay);

signals:
    void modeChanged();
    void windowWidthChanged();
    void windowHeightChanged();
    void sizeScaleFactorChanged();
    void xdpiChanged();
    void ydpiChanged();

private:
    Q_DISABLE_COPY(DisplayProfile)

    Mode _mode;
    int _preferredWindowWidth; // this just target, scale factor affect valid value
    int _preferredWindowHeight; // this just target, scale factor affect valid value
    double _sizeScaleFactor;
    Display* _targetDisplay;
    Display* _actualDisplay;

    int validateWidth(int w) const;
    int validateHeigth(int h) const;
    void recalculateAutoScale();

    bool _autoscaling;

};

#endif // DISPLAYPROFILE_H
