CONFIG += ordered

TEMPLATE = subdirs

SUBDIRS += src \
           examples

examples.depends = src

OTHER_FILES += \
    LICENSE \
    README.md \
    qhttpserver.pri \
    TODO
