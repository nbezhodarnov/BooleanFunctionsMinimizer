TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        BooleanFunction/booleanfunction.cpp \
        BooleanFunctionMinimizers/BlakeBooleanFunctionMinimizer/blakebooleanfunctionminimizer.cpp \
        BooleanFunctionsExporters/PlaFileBooleanFunctionsExporter/plafilebooleanfunctionsexporter.cpp \
        BooleanFunctionsImporters/PlaFileBooleanFunctionsImporter/plafilebooleanfunctionsimporter.cpp \
        main.cpp

HEADERS += \
    BooleanFunction/booleanfunction.h \
    BooleanFunctionMinimizers/BlakeBooleanFunctionMinimizer/blakebooleanfunctionminimizer.h \
    BooleanFunctionMinimizers/abstractbooleanfunctionminimizer.h \
    BooleanFunctionsExporters/PlaFileBooleanFunctionsExporter/plafilebooleanfunctionsexporter.h \
    BooleanFunctionsExporters/abstractbooleanfunctionsexporter.h \
    BooleanFunctionsImporters/PlaFileBooleanFunctionsImporter/plafilebooleanfunctionsimporter.h \
    BooleanFunctionsImporters/abstractbooleanfunctionsimporter.h
