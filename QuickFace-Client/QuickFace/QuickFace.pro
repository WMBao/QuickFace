TEMPLATE = app
TARGET = QuickFace
QT += core gui widgets qml quick



SOURCES += main.cpp \
    winsockmattransmissionclient.cpp \
    widget.cpp

RESOURCES +=qml.qrc

HEADERS += \
    winsockmattransmissionclient.h \
    widget.h

INCLUDEPATH+= C:\opencv\build\include
INCLUDEPATH+= C:\opencv\build\include\opencv
INCLUDEPATH+= C:\opencv\build\include\opencv2
INCLUDEPATH+= C:\opencv\build\include\opencv2\highgui
INCLUDEPATH+= C:\opencv\build\include\opencv2\core
INCLUDEPATH+= C:\opencv\build\include\opencv2\imgproc
INCLUDEPATH+= C:\opencv\build\include\opencv2\objdetect

LIBS+= C:\opencv\build\x86\vc12\lib\opencv_calib3d2413.lib
LIBS+= C:\opencv\build\x86\vc12\lib\opencv_contrib2413.lib
LIBS+= C:\opencv\build\x86\vc12\lib\opencv_core2413.lib
LIBS+= C:\opencv\build\x86\vc12\lib\opencv_features2d2413.lib
LIBS+= C:\opencv\build\x86\vc12\lib\opencv_flann2413.lib
LIBS+= C:\opencv\build\x86\vc12\lib\opencv_gpu2413.lib
LIBS+= C:\opencv\build\x86\vc12\lib\opencv_highgui2413.lib
LIBS+= C:\opencv\build\x86\vc12\lib\opencv_imgproc2413.lib
LIBS+= C:\opencv\build\x86\vc12\lib\opencv_legacy2413.lib
LIBS+= C:\opencv\build\x86\vc12\lib\opencv_ml2413.lib
LIBS+= C:\opencv\build\x86\vc12\lib\opencv_nonfree2413.lib
LIBS+= C:\opencv\build\x86\vc12\lib\opencv_objdetect2413.lib
LIBS+= C:\opencv\build\x86\vc12\lib\opencv_ocl2413.lib
LIBS+= C:\opencv\build\x86\vc12\lib\opencv_photo2413.lib
LIBS+= C:\opencv\build\x86\vc12\lib\opencv_stitching2413.lib
LIBS+= C:\opencv\build\x86\vc12\lib\opencv_superres2413.lib
LIBS+= C:\opencv\build\x86\vc12\lib\opencv_ts2413.lib
LIBS+= C:\opencv\build\x86\vc12\lib\opencv_video2413.lib
LIBS+= C:\opencv\build\x86\vc12\lib\opencv_videostab2413.lib


FORMS += \
    QuickFace.ui

RESOURCES += \
    qml.qrc

DISTFILES +=
