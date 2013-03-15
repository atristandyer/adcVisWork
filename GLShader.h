#ifndef GLSHADER_H
#define GLSHADER_H

#include "GLCamera2D.h"
#include <string>

class GLShader
{
	public:

		// Function Definitions
		GLShader();
		~GLShader();
		int	Use();
		void	SetCamera(GLCamera2D *newCam);
		bool	isLoaded();
		bool	cameraSet();
		bool	uniformsSet();

	protected:

		// Variable Definitions
		GLuint		programID;
		std::string	GLSLversion;
		GLCamera2D	*camera;

		// Flags
		bool	loaded;
		bool	allUniformsSet;
		bool	camSet;

		// Protected Functions
		virtual void	CompileShader() = 0;
		virtual void	UpdateUniforms() = 0;
		void		LoadGLSLVersion();
		GLuint		CompileShaderPart(const char *source, GLenum shaderType);

};

#endif // GLSHADER_H
