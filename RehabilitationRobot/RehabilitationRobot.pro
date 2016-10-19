TEMPLATE = app

QT += qml quick
QT +=multimedia
CONFIG += c++11

#Additional files and libs for motion Contrl
#INCLUDEPATH += $${PWD}/../Src/APS Library/include
#INCLUDEPATH += $${PWD}/../Src/NI/include
INCLUDEPATH += $${PWD}/../Src/ATI/ATI Header Files
INCLUDEPATH += $${PWD}/../Src/ATI/ATI Source Files
INCLUDEPATH += $${PWD}/../Src/Eigen3


#LIBS += -L$${PWD}/../Src/NI/lib -lNIDAQmx
#LIBS += -L$${PWD}/../Src/APS Library/lib -lAPS168

SOURCES += main.cpp \
    pasvcontrl.cpp \
    contrlCard.cpp \
    robot.cpp \
    boundarydetection.cpp \
    eyecontrol.cpp \
    encoderacquisit.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.

include(deployment.pri)



unix|win32: LIBS += -L$$PWD/../Src/NI/lib/ -lNIDAQmx

INCLUDEPATH += $$PWD/../Src/NI/include
DEPENDPATH += $$PWD/../Src/NI/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../Src/NI/lib/NIDAQmx.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../Src/NI/lib/libNIDAQmx.a

unix|win32: LIBS += -L$$PWD/'../Src/APS Library/lib/' -lAPS168

INCLUDEPATH += $$PWD/'../Src/APS Library/include'
DEPENDPATH += $$PWD/'../Src/APS Library/include'

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/'../Src/APS Library/lib/APS168.lib'
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/'../Src/APS Library/lib/libAPS168.a'

win32 {
QMAKESPEC = win32-msvc
    INCLUDEPATH += $$(OPENCV_HOME)\include $$(OPENCV_HOME)\include\opencv
    LIBS += -L$$(OPENCV_HOME)\x86\vc12\lib

    win32:CONFIG(release, debug|release): LIBS += -lopencv_world300
    else:win32:CONFIG(debug, debug|release): LIBS += -lopencv_world300d
}
HEADERS += \
    pasvcontrl.h \
    contrlCard.h \
    robot.h \
    boundarydetection.h \
    eyecontrol.h \
    encoderacquisit.h
