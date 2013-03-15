#ifndef DEFAULTSHADER_H
#define DEFAULTSHADER_H

#include "GLShader.h"
#include <iostream>
#include <fstream>

class DefaultShader : public GLShader
{
	public:
		DefaultShader();
		void SetColor(float r, float g, float b, float a);


	protected:

		// Define Variables
		GLfloat rgba[4];
		std::string vertexShaderFile;
		std::string fragmentShaderFile;

		void	CompileShader();
		void	UpdateUniforms();

};

#endif // DEFAULTSHADER_H
