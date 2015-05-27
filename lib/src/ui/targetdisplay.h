#ifndef TARGETDISPLAY_H
#define TARGETDISPLAY_H

#include "display.h"

class TargetDisplay : public Display
{
    Q_OBJECT

public:
    explicit TargetDisplay(int widthPixels, int heightPixels, double xdpi, double ydpi, QObject *parent = 0);
    ~TargetDisplay();

    virtual int widthPixels() const { return _widthPixels; }
    virtual int heightPixels() const { return _heightPixels; }
    virtual double widthInches() const { return _widthPixels / _xdpi; }
    virtual double heightInches() const { return _heightPixels / _ydpi; }
    virtual double xdpi() const { return _xdpi; }
    virtual double ydpi() const { return _ydpi; }

signals:

public slots:

protected:
    TargetDisplay(QObject* parent = 0);

private:
    Q_DISABLE_COPY(TargetDisplay)

    int _widthPixels;
    int _heightPixels;
    double _xdpi;
    double _ydpi;
};

#endif // TARGETDISPLAY_H
