#include "GradientCompareShader.h"

GradientCompareShader::GradientCompareShader()
{
	vertexShaderFile = "/home/tristan/Qt Projects/adcVis2D/GradientCompareShader.vert";
	fragmentShaderFile = "/home/tristan/Qt Projects/adcVis2D/GradientCompareShader.frag";

	bottomColor[0] = 0.0;
	bottomColor[1] = 0.0;
	bottomColor[2] = 0.0;
	bottomColor[3] = 1.0;
	topColor[0] = 1.0;
	topColor[1] = 1.0;
	topColor[2] = 1.0;
	topColor[3] = 1.0;
	wdColor[0] = 0.0;
	wdColor[1] = 0.0;
	wdColor[2] = 0.0;
	wdColor[3] = 0.0;
	heightRange[0] = 0.0;
	heightRange[1] = 1.0;

	UseCustomHeightRange = false;

	CompileShader();
	UpdateUniforms();
}


void GradientCompareShader::SetBottomColor(float r, float g, float b, float a)
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


void GradientCompareShader::SetTopColor(float r, float g, float b, float a)
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


void GradientCompareShader::SetWDColor(float r, float g, float b, float a)
{
	if (r >= 0.0)
		wdColor[0] = r;
	if (g >= 0.0)
		wdColor[1] = g;
	if (b >= 0.0)
		wdColor[2] = b;
	if (a >= 0.0)
		wdColor[3] = a;
}


void GradientCompareShader::SetHeightRange(float low, float high)
{
	heightRange[0] = low;
	heightRange[1] = high;
}


void GradientCompareShader::CompileShader()
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


void GradientCompareShader::UpdateUniforms()
{
	if (loaded && camSet)
	{
		glUseProgram(programID);

		GLint MVPUniform = glGetUniformLocation(programID, "MVPMatrix");
		GLint LowUniform = glGetUniformLocation(programID, "LowColor");
		GLint HighUniform = glGetUniformLocation(programID, "HighColor");
		GLint WDUniform = glGetUniformLocation(programID, "WDColor");
		GLint HeightUniform = glGetUniformLocation(programID, "HeightRange");

		glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, camera->MVPMatrix.m);
		glUniform4fv(LowUniform, 1, bottomColor);
		glUniform4fv(HighUniform, 1, topColor);
		glUniform4fv(WDUniform, 1, wdColor);
		glUniform2fv(HeightUniform, 1, heightRange);

		allUniformsSet = true;
	} else {
		allUniformsSet = false;
	}
}
