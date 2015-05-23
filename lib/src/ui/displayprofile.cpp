#include "displayprofile.h"

#include <QProcessEnvironment>

// TODO: later support for reading from a file
namespace {
    // TODO: these should on header side because of documentation!!

    static const char* PREF_WINDOW_WIDTH_VARNAME = "PREF_WINDOW_WIDTH";
    static const char* PREF_WINDOW_HEIGHT_VARNAME = "PREF_WINDOW_HEIGHT";
    static const char* WINDOW_SCALE_FACTOR_VARNAME = "WINDOW_SCALE_FACTOR";

}
DisplayProfile::DisplayProfile(int defaultWindowWidth,
                               int defaultWindowHeight,
                               Mode defaultMode,
                               double defaultSizeScaleFactor,
                               QObject *parent) :
    QObject(parent),
    _mode(defaultMode),
    _preferredWindowWidth(defaultWindowWidth),
    _preferredWindowHeight(defaultWindowHeight),
    _sizeScaleFactor(defaultSizeScaleFactor)
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
    }
}

DisplayProfile::~DisplayProfile()
{

}

// TODO: if real device then match real screen properties
