#ifndef REFINEDSHADER_H
#define REFINEDSHADER_H

#include "GLShader.h"
#include <iostream>
#include <fstream>

class RefinedShader : public GLShader
{
	public:
		RefinedShader();
		void	SetBottomColor(float r, float g, float b, float a);
		void	SetTopColor(float r, float g, float b, float a);
		void	SetNoErrorColor(float r, float g, float b, float a);
		void	SetNoErrorRange(float low, float high);
		void	SetErrorRange(float low, float high);

	protected:

		// Define Variables
		GLfloat	bottomColor[4];
		GLfloat	topColor[4];
		GLfloat noErrorColor[4];
		GLfloat	noErrorRange[2];
		GLfloat	errorRange[2];
		GLint	uniforms[6];

		std::string	vertexShaderFile;
		std::string	fragmentShaderFile;

		void	CompileShader();
		void	UpdateUniforms();
};

#endif // REFINEDSHADER_H
