!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    QT += core gui

    !include($$PWD/PluginInterface/UrgBenriPluginInterface.pri) {
            error("Unable to include Viewer Plugin Interface.")
    }

    !include($$PWD/UrgDrawWidget3D/UrgDrawWidget3D.pri) {
            error("Unable to include Draw Widget 3D.")
    }

    !include($$PWD/QsKineticScroller/QsKineticScroller.pri) {
            error("Unable to include QsKineticScroller.")
    }

    !include($$PWD/ColorButton/ColorButton.pri) {
            error("Unable to include Color Button.")
    }


    DEPENDPATH += $$PWD
    INCLUDEPATH += $$PWD

    SOURCES += \
            $$PWD/Plot3DViewerPlugin.cpp \
            $$PWD/PlotSettingsWidget.cpp \
            $$PWD/MathUtils.cpp

    HEADERS  += \
            $$PWD/Plot3DViewerPlugin.h \
            $$PWD/PlotSettingsWidget.h \
            $$PWD/MovingAverage.h \
            $$PWD/MathUtils.h

    FORMS += \
            $$PWD/Plot3DViewerPlugin.ui \
            $$PWD/PlotSettingsWidget.ui

    RESOURCES += \
            $$PWD/Plot3DViewerPlugin.qrc

    TRANSLATIONS = $$PWD/i18n/plugin_fr.ts \
            $$PWD/i18n/plugin_en.ts \
            $$PWD/i18n/plugin_ja.ts
}

HEADERS += \
    $$PWD/PixmapDelegate.h

SOURCES += \
    $$PWD/PixmapDelegate.cpp
