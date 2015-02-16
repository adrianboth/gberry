TEMPLATE = subdirs
CONFIG += ordered

# qhttpserver for tests
!android { SUBDIRS += qhttpserver }
