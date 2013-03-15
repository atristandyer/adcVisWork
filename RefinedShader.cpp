#include "RefinedShader.h"

RefinedShader::RefinedShader()
{
	vertexShaderFile = "/home/tristan/Qt Projects/adcVis2D/RefinedShader.vert";
	fragmentShaderFile = "/home/tristan/Qt Projects/adcVis2D/RefinedShader.frag";

	bottomColor[0] = 0.0;
	bottomColor[1] = 0.0;
	bottomColor[2] = 0.0;
	bottomColor[3] = 1.0;
	topColor[0] = 1.0;
	topColor[1] = 1.0;
	topColor[2] = 1.0;
	topColor[3] = 1.0;
	noErrorColor[0] = 0.5;
	noErrorColor[1] = 0.5;
	noErrorColor[2] = 0.5;
	noErrorColor[3] = 0.5;
	noErrorRange[0] = 0.0;
	noErrorRange[1] = 0.0;
	errorRange[0] = -1.0;
	errorRange[1] = 1.0;

	std::cout << "Shader Constructor" << std::endl;
	CompileShader();
	UpdateUniforms();
}


void RefinedShader::SetBottomColor(float r, float g, float b, float a)
{
	if (r >= 0.0)
		bottomColor[0] = r;
	if (g >= 0.0)
		bottomColor[1] = g;
	if (b >= 0.0)
		bottomColor[2] = b;
	if (a >= 0.0)
		bottomColor[3] = a;
}


void RefinedShader::SetTopColor(float r, float g, float b, float a)
{
	if (r >= 0.0)
		topColor[0] = r;
	if (g >= 0.0)
		topColor[1] = g;
	if (b >= 0.0)
		topColor[2] = b;
	if (a >= 0.0)
		topColor[3] = a;
}


void RefinedShader::SetNoErrorColor(float r, float g, float b, float a)
{
	if (r >= 0.0)
		noErrorColor[0] = r;
	if (g >= 0.0)
		noErrorColor[1] = g;
	if (b >= 0.0)
		noErrorColor[2] = b;
	if (a >= 0.0)
		noErrorColor[3] = a;
}


void RefinedShader::SetNoErrorRange(float low, float high)
{
	if (low <= high)
	{
		noErrorRange[0] = low;
		noErrorRange[1] = high;
	}
}


void RefinedShader::SetErrorRange(float low, float high)
{
	if (low < high)
	{
		errorRange[0] = low;
		errorRange[1] = high;
	}
}


void RefinedShader::CompileShader()
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
			std::cout << "Shader Compiled" << std::endl;
		} else {
			std::cout << "Shader did not compile" << std::endl;
		}
	} else {
		std::cout << "Error opening shader file" << std::endl;
	}
}


void RefinedShader::UpdateUniforms()
{
	if (loaded && camSet)
	{
		glUseProgram(programID);

		if (!allUniformsSet)
		{
			uniforms[0] = glGetUniformLocation(programID, "MVPMatrix");
			uniforms[1] = glGetUniformLocation(programID, "BottomColor");
			uniforms[2] = glGetUniformLocation(programID, "TopColor");
			uniforms[3] = glGetUniformLocation(programID, "NoErrorColor");
			uniforms[4] = glGetUniformLocation(programID, "NoErrorRange");
			uniforms[5] = glGetUniformLocation(programID, "ErrorRange");
			allUniformsSet = true;
			std::cout << "Uniforms Set" << std::endl;
		}

		glUniformMatrix4fv(uniforms[0], 1, GL_FALSE, camera->MVPMatrix.m);
		glUniform4fv(uniforms[1], 1, bottomColor);
		glUniform4fv(uniforms[2], 1, topColor);
		glUniform4fv(uniforms[3], 1, noErrorColor);
		glUniform2fv(uniforms[4], 1, noErrorRange);
		glUniform2fv(uniforms[5], 1, errorRange);
	} else {
		allUniformsSet = false;
	}
}
