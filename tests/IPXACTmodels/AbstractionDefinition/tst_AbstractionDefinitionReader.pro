#-----------------------------------------------------------------------------
# File: tst_AbstractionDefinitionReader.pro
#-----------------------------------------------------------------------------
# Project: Kactus 2
# Author: Esko Pekkarinen
# Date: 03.08.2015
#
# Description:
# Qt project file template for running unit tests for class AbstractionDefinition.
#-----------------------------------------------------------------------------

TEMPLATE = app

TARGET = tst_AbstractionDefinitionReader

QT += core xml testlib
QT -= gui

CONFIG += c++11 testcase console

linux-g++ | linux-g++-64 | linux-g++-32 {
 LIBS += -L../../../executable \
     -lIPXACTmodels

}
win64 | win32 {
 LIBS += -L../../../x64/executable \
     -lIPXACTmodelsd
}

DESTDIR = ./release

INCLUDEPATH += $$DESTDIR
INCLUDEPATH += ../../../

DEPENDPATH += .

OBJECTS_DIR += $$DESTDIR

MOC_DIR += ./generatedFiles
UI_DIR += ./generatedFiles
RCC_DIR += ./generatedFiles

include(tst_AbstractionDefinitionReader.pri)
