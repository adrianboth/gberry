message("Setting from global.pri")

# PWD is relative to this *.pri file
include($$PWD/../global.pri)

DISTFILES += \
    $$PWD/scripts/start.sh

