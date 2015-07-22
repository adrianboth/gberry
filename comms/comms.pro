TEMPLATE = subdirs

SUBDIRS += src app tests integration

app.depends = src
tests.depends = src
integration.depends = src

CONFIG += ordered

OTHER_FILES = TODO.txt
