#ifndef APPLICATIONSTORAGE_H
#define APPLICATIONSTORAGE_H

#include <QQmlApplicationEngine>

/**
 * @brief The ApplicationStorage class
 *
 * This class manages file location where information can be saved. It is
 * targeted for mobile applications.
 *
 * By default the storage location is same as a default offline storage
 * location, but for development purposes it can be affected by an enviroment
 * variable APP_ID. In that case a subdirectory is created 'app_<id>'.
 *
 * The path is set for provided QQmlApplicationEngine.
 */
class ApplicationStorage
{
public:
    ApplicationStorage(QQmlApplicationEngine* engine);
    ~ApplicationStorage();

    QString storagePath() const;

private:
    QQmlApplicationEngine* _engine;
};

#endif // APPLICATIONSTORAGE_H
