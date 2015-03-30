message("Setting from global.pri")
INSTALLDIR=/home/tevuore/gberry-deploy

message("INSTALLDIR=$${INSTALLDIR}")

# PWD is relative to this *.pri file
include($$PWD/../global.pri)

DISTFILES += \
    $$PWD/scripts/start.sh

