#include "GLShader.h"
#include <iostream>

GLShader::GLShader()
{
	programID = 0;
	camera = 0;
	loaded = false;
	camSet = false;
	allUniformsSet = false;
	LoadGLSLVersion();
}


GLShader::~GLShader()
{
	if (loaded)
	{
		glUseProgram(0);
		glDeleteProgram(programID);
	}
}


int GLShader::Use()
{
	UpdateUniforms();
	if (allUniformsSet)
	{
		glUseProgram(programID);
		return 0;
	}
	return 1;
}


void GLShader::SetCamera(GLCamera2D *newCam)
{
	if (newCam != 0)
	{
		camera = newCam;
		camSet = true;
		UpdateUniforms();
	}
}


bool GLShader::isLoaded()
{
	return loaded;
}


bool GLShader::cameraSet()
{
	return camSet;
}


bool GLShader::uniformsSet()
{
	return allUniformsSet;
}


GLuint GLShader::CompileShaderPart(const char *source, GLenum shaderType)
{
	GLuint shaderID = glCreateShader(shaderType);

	if (shaderID != 0)
	{
		glShaderSource(shaderID, 1, &source, NULL);
		glCompileShader(shaderID);

		GLint compileResult;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileResult);
		if (compileResult != GL_TRUE)
		{
			std::cout << "Compile Error" << std::endl;
			return 0;
		} else {
			return shaderID;
		}
	} else {
		std::cout << "Error creating shader" << std::endl;
		return 0;
	}
}


void GLShader::LoadGLSLVersion()
{
	std::string version ((char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
	version.erase(version.begin() + 4, version.end());
	version.erase(version.begin() + 1);
	GLSLversion = version;
}
