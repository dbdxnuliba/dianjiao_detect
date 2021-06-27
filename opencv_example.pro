QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DetectResLog.cpp \
    board_A_function.cpp \
    board_B_function.cpp \
    boardctrl.cpp \
    commonFunc.cpp \
    dialogtemplatesetting.cpp \
    fileReadThread.cpp \
    main.cpp \
    mainwindow.cpp \
    thresholdsetting.cpp

HEADERS += \
    DetectResLog.h \
    board_A_function.h \
    board_B_function.h \
    boardctrl.h \
    commonFunc.h \
    dialogtemplatesetting.h \
    fileReadThread.h \
    mainwindow.h \
    thresholdsetting.h

FORMS += \
    dialogtemplatesetting.ui \
    mainwindow.ui \
    thresholdsetting.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += /usr/local/include/opencv4

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lopencv_calib3d -lopencv_core -lopencv_dnn -lopencv_features2d \
-lopencv_flann -lopencv_gapi -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_ml -lopencv_ml \
-lopencv_stitching -lopencv_video -lopencv_video -lopencv_objdetect

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include
