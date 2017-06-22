!contains( included_modules, $$PWD ) {

    included_modules += $$PWD
    QT += core gui widgets opengl
    win32:LIBS += -lOpengl32 -lGlu32
    macx:LIBS += -framework GLUT

    DEPENDPATH += \
        $$PWD

    INCLUDEPATH += \
        $$PWD

    SOURCES += \
        $$PWD/UrgDrawWidget3D.cpp \
        $$PWD/ColorScheme.cpp \
        $$PWD/Camera.cpp \
        $$PWD/Axis.cpp \
        $$PWD/GridPlaneXY.cpp \
        $$PWD/gl_utils.cpp \
        $$PWD/Scene.cpp \
        $$PWD/Viewport.cpp \
        $$PWD/RenderableItem.cpp

    HEADERS  += \
        $$PWD/UrgDrawWidget3D.h \
        $$PWD/ColorScheme.h \
        $$PWD/Camera.h \
        $$PWD/Axis.h \
        $$PWD/GridPlaneXY.h \
        $$PWD/gl_utils.h \
        $$PWD/glfont_mono.h \
        $$PWD/glfont_sans.h \
        $$PWD/glfont_serif.h \
        $$PWD/Scene.h \
        $$PWD/Viewport.h \
        $$PWD/RenderableItem.h
}

HEADERS += \
    $$PWD/Text.h \
    $$PWD/PointCloud.h \
    $$PWD/ColorModel.h

SOURCES += \
    $$PWD/Text.cpp \
    $$PWD/PointCloud.cpp \
    $$PWD/ColorModel.cpp

