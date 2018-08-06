#-------------------------------------------------
#
# Project created by QtCreator 2018-06-10T11:00:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = job
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
	main.cpp\
    firstgantt.cpp \
	mainwidget.cpp \
    src/datastructure.cpp \
    src/giffler.cpp \
    src/input.cpp \
    src/output.cpp \
    src/queue.cpp \
    src/tabusearch.cpp \
    src/topograph.cpp \
    secondgantt.cpp

HEADERS  += \
	src/datastructure.h \
	src/giffler.h \
	src/input.h \
	src/output.h \
	src/queue.h \
	src/tabusearch.h \
	src/topograph.h \
    firstgantt.h \
	mainwidget.h \
    secondgantt.h


RESOURCES += \
    res.qrc
