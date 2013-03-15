#ifndef GRADIENTCOMPARESHADER_H
#define GRADIENTCOMPARESHADER_H

#include "GLShader.h"
#include <iostream>
#include <fstream>

class GradientCompareShader : public GLShader
{
	public:
		GradientCompareShader();
		void	SetBottomColor(float r, float g, float b, float a);
		void	SetTopColor(float r, float g, float b, float a);
		void	SetWDColor(float r, float g, float b, float a);
		void	SetHeightRange(float low, float high);
		bool	UseCustomHeightRange;

	protected:

		// Define variables
		GLfloat	bottomColor[4];
		GLfloat	topColor[4];
		GLfloat	wdColor[4];
		GLfloat heightRange[2];

		std::string	vertexShaderFile;
		std::string	fragmentShaderFile;

		void	CompileShader();
		void	UpdateUniforms();
};

#endif // GRADIENTCOMPARESHADER_H
