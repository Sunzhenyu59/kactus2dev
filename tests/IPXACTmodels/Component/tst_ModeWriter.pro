#-----------------------------------------------------------------------------
# File: tst_ModeWriter.pro
#-----------------------------------------------------------------------------
# Project: Kactus 2
# Author: Esko Pekkarinen
# Date: 07.08.2023
#
# Description:
# Qt project file for running unit tests for Mode Writer.
#-----------------------------------------------------------------------------

TEMPLATE = app

TARGET = tst_ModeWriter

QT += core xml testlib
QT -= gui

CONFIG += c++11 testcase console

LIBS += -L../../../executable -lIPXACTmodels

INCLUDEPATH += $$DESTDIR
INCLUDEPATH += ../../../

DEPENDPATH += .
DEPENDPATH += ../../../

OBJECTS_DIR += $$DESTDIR

MOC_DIR += ./generatedFiles
UI_DIR += ./generatedFiles
RCC_DIR += ./generatedFiles
include(tst_ModeWriter.pri)
