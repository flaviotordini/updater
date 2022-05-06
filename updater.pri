DEFINES *= UPDATER

INCLUDEPATH += $$PWD/src
DEPENDPATH += $$PWD/src

HEADERS += $$PWD/src/updater.h
SOURCES += $$PWD/src/updater.cpp

macx:!contains(DEFINES, UPDATER_NO_SPARKLE) {
    DEFINES += UPDATER_SPARKLE
    INCLUDEPATH += $$PWD/src/sparkle
    DEPENDPATH += $$PWD/src/sparkle
    LIBS += -F/Library/Frameworks -framework Sparkle
    INCLUDEPATH += /Library/Frameworks/Sparkle.framework/Headers

    # Hack to get the inclusion of headers working
    # Sparkle includes stuff using <Sparkle/SUClass>
    # We need to create a Sparkle subdir inside Sparkle-headers
    INCLUDEPATH += /Library/Frameworks/Sparkle-headers

    HEADERS += $$PWD/src/sparkle/sparkleupdater.h
    OBJECTIVE_SOURCES += $$PWD/src/sparkle/sparkleupdater.mm
} else {
    DEFINES += UPDATER_DEFAULT
    QT *= network widgets
    INCLUDEPATH += $$PWD/src/impl
    DEPENDPATH += $$PWD/src/impl

    HEADERS += \
    $$PWD/src/impl/checker.h \
    $$PWD/src/impl/defaultupdater.h \
    $$PWD/src/impl/dialog.h \
    $$PWD/src/impl/downloader.h \
    $$PWD/src/impl/installer.h \
    $$PWD/src/impl/parser.h \
    $$PWD/src/impl/runinstaller.h \
    $$PWD/src/impl/simplexmlparser.h \
    $$PWD/src/impl/appcastparser.h \
    $$PWD/src/impl/openinstaller.h

    SOURCES +=  \
    $$PWD/src/impl/checker.cpp \
    $$PWD/src/impl/defaultupdater.cpp \
    $$PWD/src/impl/dialog.cpp \
    $$PWD/src/impl/downloader.cpp \
    $$PWD/src/impl/runinstaller.cpp \
    $$PWD/src/impl/simplexmlparser.cpp \
    $$PWD/src/impl/appcastparser.cpp \
    $$PWD/src/impl/openinstaller.cpp
}
