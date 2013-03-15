#include "DefaultShader.h"

#include <iostream>

DefaultShader::DefaultShader()
{
	vertexShaderFile = "/home/tristan/Qt Projects/adcVis2D/Default.vert";
	fragmentShaderFile = "/home/tristan/Qt Projects/adcVis2D/Default.fsh";

	rgba[0] = 1.0;
	rgba[1] = 1.0;
	rgba[2] = 1.0;
	rgba[3] = 1.0;

	CompileShader();
	UpdateUniforms();
}


void DefaultShader::SetColor(float r, float g, float b, float a)
{
	if (r >= 0.0)
		rgba[0] = r;
	if (g >= 0.0)
		rgba[1] = g;
	if (b >= 0.0)
		rgba[2] = b;
	if (a >= 0.0)
		rgba[3] = a;
}


void DefaultShader::CompileShader()
{
	std::ifstream vertexFile(vertexShaderFile.data());
	std::ifstream fragFile(fragmentShaderFile.data());

	if (vertexFile.is_open() && fragFile.is_open())
	{
		std::string vertex((std::istreambuf_iterator<char>(vertexFile)), (std::istreambuf_iterator<char>()));
		std::string fragment((std::istreambuf_iterator<char>(fragFile)), (std::istreambuf_iterator<char>()));

		vertexFile.close();
		fragFile.close();

		std::string vertSource = "#version ";
		vertSource.append(GLSLversion);
		vertSource.append("\n");
		vertSource.append(vertex);

		std::string fragSource = "#version ";
		fragSource.append(GLSLversion);
		fragSource.append("\n");
		fragSource.append(fragment);

		const char *fullVertSource = vertSource.data();
		const char *fullFragSource = fragSource.data();

		GLuint vertexShaderID = CompileShaderPart(fullVertSource, GL_VERTEX_SHADER);
		GLuint fragmentShaderID = CompileShaderPart(fullFragSource, GL_FRAGMENT_SHADER);

		if (vertexShaderID && fragmentShaderID)
		{
			programID = glCreateProgram();
			glAttachShader(programID, vertexShaderID);
			glAttachShader(programID, fragmentShaderID);
			glLinkProgram(programID);
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);

			loaded = true;
		}
	}
}


void DefaultShader::UpdateUniforms()
{
	if (loaded && camSet)
	{
		glUseProgram(programID);

		GLint MVPUniform = glGetUniformLocation(programID, "MVPMatrix");
		GLint ColorUniform = glGetUniformLocation(programID, "ColorVector");

		glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, camera->MVPMatrix.m);
		glUniform4fv(ColorUniform, 1, rgba);

		allUniformsSet = true;
	} else {
		allUniformsSet = false;
	}
}
