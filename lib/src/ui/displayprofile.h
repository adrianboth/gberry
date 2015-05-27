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
