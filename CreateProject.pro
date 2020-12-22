QT += qml quick core gui widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        config.cpp \
        fileprocess.cpp \
        listdatalist.cpp \
        listdatamodel.cpp \
        main.cpp \
        task.cpp

RESOURCES += qml.qrc \
    image.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = myapp.ico

RC_FILE += myapp.rc

HEADERS += \
    config.h \
    fileprocess.h \
    listdatalist.h \
    listdatamodel.h \
    task.h
