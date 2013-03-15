#-------------------------------------------------
#
# Project created by QtCreator 2012-12-12T14:10:01
#
#-------------------------------------------------

QT       += core gui opengl

DEFINES += GLEW_STATIC

LIBS += -lGLU

TARGET = adcVis2D
TEMPLATE = app


QMAKE_CXXFLAGS_DEBUG += -pg -O3
QMAKE_LFLAGS_DEBUG += -pg


SOURCES += main.cpp\
	MainWindow.cpp \
    OpenGLPanel.cpp \
    glew.c \
    GLCamera2D.cpp \
    GLShader.cpp \
    TerrainLayer.cpp \
    DefaultShader.cpp \
    WaterLayer.cpp \
    GradientShader.cpp \
    RefinedTerrainLayer.cpp \
    RefinedWaterLayer.cpp \
    RefinedShader.cpp \
    Quadtree.cpp \
    Dialog_NormalMesh.cpp \
    Dialog_ShaderEditor.cpp \
    Frame_DefaultShader.cpp \
    LayerManager.cpp \
    Frame_DataDisplay.cpp \
    Frame_LayerDisplay.cpp \
    Frame_GradientShader.cpp \
    Dialog_Compare_Sub_Full.cpp \
    TerrainLayer_Compare_Full_Sub.cpp \
    WaterLayer_Compare_Full_Sub.cpp \
    GradientCompareShader.cpp \
    Frame_GradientCompareShader.cpp

HEADERS  += MainWindow.h \
    OpenGLPanel.h \
    wglew.h \
    glxew.h \
    glew.h \
    GLCamera2D.h \
    GLData.h \
    adcData.h \
    GLShader.h \
    TerrainLayer.h \
    DefaultShader.h \
    Layer.h \
    WaterLayer.h \
    GradientShader.h \
    RefinedTerrainLayer.h \
    RefinedWaterLayer.h \
    RefinedShader.h \
    Quadtree.h \
    Dialog_NormalMesh.h \
    Dialog_ShaderEditor.h \
    Frame_DefaultShader.h \
    LayerManager.h \
    Frame_DataDisplay.h \
    Frame_LayerDisplay.h \
    Frame_GradientShader.h \
    Dialog_Compare_Sub_Full.h \
    TerrainLayer_Compare_Full_Sub.h \
    WaterLayer_Compare_Full_Sub.h \
    GradientCompareShader.h \
    Frame_GradientCompareShader.h

FORMS    += MainWindow.ui \
    Dialog_NormalMesh.ui \
    Dialog_ShaderEditor.ui \
    Frame_DefaultShader.ui \
    Frame_DataDisplay.ui \
    Frame_LayerDisplay.ui \
    Frame_GradientShader.ui \
    Dialog_Compare_Sub_Full.ui \
    Frame_GradientCompareShader.ui

OTHER_FILES += \
    Default.vert \
    Default.fsh \
    GradientShader.vert \
    GradientShader.frag \
    RefinedShader.vert \
    RefinedShader.frag \
    GradientCompareShader.vert \
    GradientCompareShader.frag
