#ifndef DIALOG_SHADEREDITOR_H
#define DIALOG_SHADEREDITOR_H

#include <QDialog>
#include <iostream>
#include "GLShader.h"
#include "DefaultShader.h"
#include "GradientShader.h"
#include "GradientCompareShader.h"

#include "Frame_DefaultShader.h"
#include "Frame_GradientShader.h"
#include "Frame_GradientCompareShader.h"

namespace Ui {
	class Dialog_ShaderEditor;
}

class Dialog_ShaderEditor : public QDialog
{
		Q_OBJECT
		
	public:
		explicit Dialog_ShaderEditor(QWidget *parent = 0);
		~Dialog_ShaderEditor();

		// Initialization functions
		void initialTab(int index);
		void setDefaultShaderName(QString name);

		// Return value getters
//		GLShader*	getNewShader();
		DefaultShader*		getNewDefaultShader();
		GradientShader*		getNewGradientShader();
		GradientCompareShader*	getNewGradientCompareShader();
		QString			getShaderName();
		int			getShaderType();
		int			getTerrainOrWater();
		
	private:
		Ui::Dialog_ShaderEditor *ui;

		// Frame Stuff
		int defaultShaderIndexTerrain;
		int defaultShaderIndexWater;
		int gradientShaderIndexTerrain;
		int gradientShaderIndexWater;
		int gradientCompareShaderIndex;
		Frame_DefaultShader		*defaultShaderFrameTerrain;
		Frame_DefaultShader		*defaultShaderFrameWater;
		Frame_GradientShader		*gradientShaderFrameTerrain;
		Frame_GradientShader		*gradientShaderFrameWater;
		Frame_GradientCompareShader	*gradientCompareShaderFrame;

	private slots:
		void shaderPicked(int index);

};

#endif // DIALOG_SHADEREDITOR_H
