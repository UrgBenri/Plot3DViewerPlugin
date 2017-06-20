TEMPLATE    = lib
CONFIG      += plugin
#DESTDIR     = $$PWD/../bin/plugins
DESTDIR     = $$PWD/../bin/UrgBenriPlus.app/plugins
CONFIG      += c++11
TARGET      = Plot3DViewerPlugin

!include(Plot3DViewerPlugin.pri) {
        error("Unable to include Plot Viewer Plugin 3D.")
}

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
