QT += opengl widgets

TARGET = JogoDaMemoria

TEMPLATE = app

SOURCES += \
        main.cpp \
        jogodamemoria.cpp

RESOURCES += res.qrc

HEADERS += \
        jogodamemoria.h

win32 {
    LIBS += \
            -lopengl32 \
            -lglu32
}

unix {
    LIBS += \
            -lGL \
            -lGLU
}

