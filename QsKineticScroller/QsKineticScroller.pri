!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    QT += core gui
	
	DEPENDPATH += $$PWD
    INCLUDEPATH += $$PWD

    SOURCES += \
            $$PWD/QsKineticScroller.cpp

    HEADERS  += \
            $$PWD/QsKineticScroller.h
}