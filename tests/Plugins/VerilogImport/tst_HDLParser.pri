# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

# This is a reminder that you are using a generated .pro file.
# Remove it when you are finished editing this file.
message("You are running qmake on a generated .pro file. This may not work!")


HEADERS += ../../../IPXACTmodels/utilities/ComponentSearch.h \
    ../../../Plugins/common/HDLParser/HDLParserCommon.h \
    ../../../Plugins/common/HDLParser/MetaComponent.h \
    ../../../Plugins/common/HDLParser/MetaDesign.h \
    ../../../Plugins/common/HDLParser/MetaInstance.h \
    ../../../Plugins/VerilogGenerator/ModelParameterVerilogWriter/ModelParameterVerilogWriter.h \
    ../../../Plugins/common/PortSorter/PortSorter.h \
    ../../../IPXACTmodels/utilities/Search.h \
    ../../../editors/common/TopComponentParameterFinder.h \
    ../../MockObjects/LibraryMock.h \
    ../../../Plugins/PluginSystem/GeneratorPlugin/MessagePasser.h
SOURCES += ../../../IPXACTmodels/utilities/ComponentSearch.cpp \
    ../../MockObjects/LibraryMock.cpp \
    ../../../Plugins/PluginSystem/GeneratorPlugin/MessagePasser.cpp \
    ../../../Plugins/common/HDLParser/MetaComponent.cpp \
    ../../../Plugins/common/HDLParser/MetaDesign.cpp \
    ../../../Plugins/common/HDLParser/MetaInstance.cpp \
    ../../../editors/common/TopComponentParameterFinder.cpp \
    ./tst_HDLParser.cpp
