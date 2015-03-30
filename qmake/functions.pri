message("Functions from gberrylib-functions.pri")

# projects including functions.pri need to defined PROJECTS_ROOT_DIR
isEmpty(PROJECTS_ROOT_DIR) {
    error("PROJECTS_ROOT_DIR has not been defined!")
}
BUILDS_DIR=$${PROJECTS_ROOT_DIR}/builds


# BUILD_NAME is defined via commandline when qmake is invoked.
isEmpty(BUILD_NAME) {
    # When building through QtCreator we use Build name defined by creator
    # but how to get it?
    #   -> now there just needs to be match with BUILD_NAME an QtCreator

    #message("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%")
    #message($$QMAKESPEC)
    #message($$CONFIG)
    #error(test)

    # TODO: what is android x86
    # TODO: also with QtCreator build name can change
    android-g++ {
        BUILD_NAME=Android_for_armeabi_v7a_GCC_4_8_Qt_5_4_0
    }
    linux-g++ {
        BUILD_NAME=Desktop_Qt_5_4_0_GCC_64bit
    }
    linux-rasp-pi-g++ {
        BUILD_NAME=Raspberry
    }

    debug {
        BUILD_NAME="$${BUILD_NAME}-Debug"
    } else {
        BUILD_NAME="$${BUILD_NAME}-Release"
    }

    message("BUILD_NAME not defined -> using default")
}
message(" -- BUILD_NAME: $${BUILD_NAME}")


defineTest(includeStaticLibrary) {
    LIB_NAME=$${1}
    SRC_DIR=$${2}
    BUILD_DIR=$${3}

    message(" -- Adding static lib: $${LIB_NAME} $${SRC_DIR} $${BUILD_DIR}")

    win32:CONFIG(release, debug|release): LIBS += -L$${BUILD_DIR}/release -l$${LIB_NAME}
    else:win32:CONFIG(debug, debug|release): LIBS += -L$${BUILD_DIR}/debug -l$${LIB_NAME}
    else:unix: LIBS += -L$${BUILD_DIR} -l$${LIB_NAME}

    INCLUDEPATH += $$SRC_DIR
    DEPENDPATH += $$SRC_DIR

    win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $${BUILD_DIR}/release/lib$${LIB_NAME}.a
    else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $${BUILD_DIR}/lib$${LIB_NAME}.a
    else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $${BUILD_DIR}/release/$${LIB_NAME}.lib
    else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $${BUILD_DIR}/debug/$${LIB_NAME}.lib
    else:unix: PRE_TARGETDEPS += $${BUILD_DIR}/lib$${LIB_NAME}.a

    export(LIBS)
    export(INCLUDEPATH)
    export(DEPENDPATH)
    export(PRE_TARGETDEPS)
}


defineTest(includeSharedLibrary) {
    LIB_NAME=$${1}
    SRC_DIR=$${2}
    BUILD_DIR=$${3}

    message(" -- Adding shared lib: $${LIB_NAME} $${SRC_DIR} $${BUILD_DIR}")

    win32:CONFIG(release, debug|release): LIBS += -L$${BUILD_DIR}/release -l$${LIB_NAME}
    else:win32:CONFIG(debug, debug|release): LIBS += -L$${BUILD_DIR}/debug -l$${LIB_NAME}
    else:unix: LIBS += -L$${BUILD_DIR} -l$${LIB_NAME}

    INCLUDEPATH += $$SRC_DIR
    DEPENDPATH += $$SRC_DIR

    win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $${BUILD_DIR}/release/lib$${LIB_NAME}.so
    else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $${BUILD_DIR}/lib$${LIB_NAME}.so
    else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $${BUILD_DIR}/release/$${LIB_NAME}.lib
    else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $${BUILD_DIR}/debug/$${LIB_NAME}.lib
    else:unix: PRE_TARGETDEPS += $${BUILD_DIR}/lib$${LIB_NAME}.so

    export(LIBS)
    export(INCLUDEPATH)
    export(DEPENDPATH)
    export(PRE_TARGETDEPS)
}
