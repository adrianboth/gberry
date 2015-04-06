#ifndef DISPLAYPROFILE_H
#define DISPLAYPROFILE_H

#include <QObject>

class DisplayProfile
{
    Q_OBJECT
public:
    DisplayProfile();
    ~DisplayProfile();


    QQmlPropertyMap screenProps;
    screenProps.insert("name", QVariant(QString("John Smith")));

    // TODO: read for environment
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    if (env.contains("PREF_WINDOW_WIDTH")) {
        screenProps.insert("preferredWindowWidth", QVariant(env.value("PREF_WINDOW_WIDTH").toInt()));
    } else {
        screenProps.insert("preferredWindowWidth", QVariant(320));
    }

    if (env.contains("PREF_WINDOW_HEIGHT")) {
        screenProps.insert("preferredWindowHeight", QVariant(env.value("PREF_WINDOW_HEIGHT").toInt()));
    } else {
        screenProps.insert("preferredWindowHeight", QVariant(400));
    }
};

#endif // DISPLAYPROFILE_H
