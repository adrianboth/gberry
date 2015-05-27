#ifndef ACTUALDISPLAY_H
#define ACTUALDISPLAY_H

#include <QScreen>
#include <QApplication>
#include <QStyle>
#include <QDebug>

#include "display.h"

class ActualDisplay : public Display
{
    Q_OBJECT

public:
    ActualDisplay(QScreen* usedScreen, QObject* parent = 0);
    ~ActualDisplay();

    virtual int widthPixels() const { return _screen->availableSize().width() - 2*_windowBorderWidthPixelSize; }
    virtual int heightPixels() const {
        if (QApplication::style()) {
            return _screen->availableSize().height() - QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight) - 2*_windowBorderHeightPixelSize;
        } else {
            qDebug() << "[ActualDisplay] No style available, using defaults";
            return _screen->availableSize().height() - _windowTitleBarHeightDefault - 2*_windowBorderHeightPixelSize;
        }
    }

    virtual double widthInches() const { return widthPixels() / xdpi(); }
    virtual double heightInches() const { return heightPixels() / ydpi(); }
    virtual double xdpi() const { return _screen->physicalDotsPerInchX(); }
    virtual double ydpi() const { return _screen->physicalDotsPerInchY(); }

private:
    QScreen* _screen;

    // TODO: these probably depend on system and style, how to get them matched ...

    // there isn't good way to query these until window would be visible,
    // just using reasonable values, we won't be using for dev full size anyway
    // as we try to keep aspect ratio
    int _windowBorderWidthPixelSize = 5;
    int _windowBorderHeightPixelSize = 5;

    int _windowTitleBarHeightDefault = 0;

};

#endif // ACTUALDISPLAY_H
