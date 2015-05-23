#ifndef DISPLAYPROFILE_H
#define DISPLAYPROFILE_H

#include <QObject>

/**
 * This class support development time scaling of display to match
 * target hardware display specifications.
 *
 */
class DisplayProfile : public QObject
{
    Q_OBJECT

    enum Mode { PixelMatch, DimensionMatch };

    Q_PROPERTY(Mode mode READ mode NOTIFY modeChanged)
    Q_PROPERTY(int windowWidth READ windowWidth NOTIFY windowWidthChanged)
    Q_PROPERTY(int windowHeight READ windowHeight NOTIFY windowHeightChanged)
    Q_PROPERTY(double sizeScaleFactor READ sizeScaleFactor NOTIFY sizeScaleFactorChanged)


public:
    DisplayProfile(int defaultWindowWidth,
                   int defaultWindowHeight,
                   Mode defaultMode = DisplayProfile::PixelMatch,
                   double defaultSizeScaleFactor = 1.0,
                   QObject* parent = 0);

    ~DisplayProfile();

    Mode mode() const { return _mode; }
    // TODO: implement dimensions mode
    int windowWidth() const { return _preferredWindowWidth * _sizeScaleFactor; }
    int windowHeight() const { return _preferredWindowHeight * _sizeScaleFactor; }
    double sizeScaleFactor() const { return _sizeScaleFactor; }


signals:
    void modeChanged();
    void windowWidthChanged();
    void windowHeightChanged();
    void sizeScaleFactorChanged();

private:
    Mode _mode;
    int _preferredWindowWidth; // this just target, scale factor affect valid value
    int _preferredWindowHeight; // this just target, scale factor affect valid value
    double _sizeScaleFactor;

};

#endif // DISPLAYPROFILE_H
