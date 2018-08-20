QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



HEADERS += \
    MeasuareMethod.h \
    cfileoperator.h \
    qfileoperator.h \
    offlinemeasuare.h \
    datatypedefine.h

SOURCES += main.cpp \
MeasuareMethod.cpp \
    cfileoperator.cpp \
    qfileoperator.cpp \
    offlinemeasuare.cpp

INCLUDEPATH += /home/philip/opencv-3.4.1/Build_openCV-3.4.1/include\
 /home/philip/opencv-3.4.1/Build_openCV-3.4.1/include/opencv\
 /home/philip/opencv-3.4.1/Build_openCV-3.4.1/include/opencv2
LIBS += /home/philip/opencv-3.4.1/Build_openCV-3.4.1/lib/libopencv_*.so



