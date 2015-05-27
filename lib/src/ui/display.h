#ifndef DISPLAY_H
#define DISPLAY_H

#include <QObject>

class Display : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int widthPixels READ widthPixels)
    Q_PROPERTY(int heightPixels READ heightPixels)
    Q_PROPERTY(double widthInches READ widthInches)
    Q_PROPERTY(double heightInches READ heightInches)
    Q_PROPERTY(double xdpi READ xdpi)
    Q_PROPERTY(double ydpi READ ydpi)

public:
    explicit Display(QObject *parent = 0) : QObject(parent) {}
    ~Display() {}

    virtual int widthPixels() const = 0;
    virtual int heightPixels() const = 0;
    virtual double widthInches() const = 0;
    virtual double heightInches() const = 0;
    virtual double xdpi() const = 0;
    virtual double ydpi() const = 0;

};

#endif // DISPLAY_H

