
QT += widgets serialport network

INCLUDEPATH += $$PWD/include

HEADERS += \
    include/MainWindow.h \
    include/DevicePanel.h \
    include/LensClient.h \
    include/CameraClient.h \
    include/TiffWriter.h

SOURCES += \
    src/SigmaControl.cpp \
    src/MainWindow.cpp \
    src/DevicePanel.cpp \
    src/LensClient.cpp \
    src/CameraClient.cpp \
    src/TiffWriter.cpp

FORMS += \
    src/MainWindow.ui \
    src/DevicePanel.ui

