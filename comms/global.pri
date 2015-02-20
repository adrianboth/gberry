message("Setting from global.pri")
INSTALLDIR=/home/tevuore/gberry-deploy

message("INSTALLDIR=$${INSTALLDIR}")

defineTest(includeStaticLibrary) {
    LIB_NAME=$${1}
    SRC_DIR=$${2}
    BUILD_DIR=$${3}

    message(" -- adding static lib: $${LIB_NAME} $${SRC_DIR} $${BUILD_DIR}")

    win32:CONFIG(release, debug|release): LIBS += -L$${BUILD_DIR}/release -l$${LIB_NAME}
    else:win32:CONFIG(debug, debug|release): LIBS += -L$${BUILD_DIR}/debug -l$${LIB_NAME}
    else:unix: LIBS += -L$${BUILD_DIR} -l$${LIB_NAME}

    INCLUDEPATH += $$SRC_DIR
    DEPENDPATH += $$SRC_DIR

    win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $${BUILD_DIR}/release/lib$${LIB_NAME}.a
    else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $${GBERRYLIB_BUILD_DIR}/lib$${LIB_NAME}.a
    else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $${BUILD_DIR}/release/$${LIB_NAME}.lib
    else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $${BUILD_DIR}/debug/$${LIB_NAME}.lib
    else:unix: PRE_TARGETDEPS += $${BUILD_DIR}/lib$${LIB_NAME}.a

    export(LIBS)
    export(INCLUDEPATH)
    export(DEPENDPATH)
    export(PRE_TARGETDEPS)
}
