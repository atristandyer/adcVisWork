#ifndef GLCAMERA2D_H
#define GLCAMERA2D_H

#include "adcData.h"
#include "GLData.h"
#include "string.h"

#include <iostream>

class GLCamera2D
{
	public:

		// Variable Definitions
		Matrix	MVPMatrix;
		Matrix	ModelMatrix;
		Matrix	ViewMatrix;
		Matrix	ProjectionMatrix;

		float	panX;
		float	panY;
		float	zoomLevel;
		float	zoomScale;

		float	window[6];
		float	viewportX;
		float	viewportY;
		float	pixelToViewRatio;

		// Function Definitions
		GLCamera2D();
		void	Pan(float dx, float dy);
		void	Zoom(float zoomAmount);
		void	SetWindowSize(float l, float r, float b, float t, float n, float f);
		void	ResetView();
		void	GetUnprojectedPoint(float x, float y, float *resultX, float *resultY);

	private:

		// Update Matrices Operations
		void	UpdateModel();
		void	UpdateProjection();
		void	UpdateMVP();

		// Matrix Operations
		Matrix	MultiplyMatrices(const Matrix *m1, const Matrix *m2);
		void	ScaleMatrix(Matrix *m, float x, float y, float z);
		void	TranslateMatrix(Matrix *m, float x, float y, float z);
};

#endif // GLCAMERA2D_H
