#ifndef TARGETDISPLAY_H
#define TARGETDISPLAY_H

#include <QObject>

class TargetDisplay : public QObject
{
    Q_OBJECT
public:
    explicit TargetDisplay(QObject *parent = 0);
    ~TargetDisplay();

signals:

public slots:
};

#endif // TARGETDISPLAY_H
