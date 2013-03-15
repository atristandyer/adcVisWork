#include "GLCamera2D.h"
#include <math.h>

GLCamera2D::GLCamera2D()
{
	MVPMatrix = IDENTITY_MATRIX;
	ModelMatrix = IDENTITY_MATRIX;
	ViewMatrix = IDENTITY_MATRIX;
	ProjectionMatrix = IDENTITY_MATRIX;
	panX = 0.0;
	panY = 0.0;
	zoomLevel = 1.0/EARTH_RADIUS;
	zoomScale = 1.1;

	window[0] = -1.0;
	window[1] = 1.0;
	window[2] = -1.0;
	window[3] = 1.0;
	window[4] = -1.0;
	window[5] = 1.0;

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	viewportX = viewport[2];
	viewportY = viewport[3];

	pixelToViewRatio = ((window[1]-window[0])/2.0)/viewportX;
}


void GLCamera2D::Pan(float dx, float dy)
{
	panX += pixelToViewRatio*dx/zoomLevel;
	panY -= pixelToViewRatio*dy/zoomLevel;
	UpdateModel();
}


void GLCamera2D::Zoom(float zoomAmount)
{
	if (zoomAmount > 0)
	{
		zoomLevel *= zoomScale;
	} else {
		zoomLevel /= zoomScale;
	}
	UpdateProjection();
}


void GLCamera2D::SetWindowSize(float l, float r, float b, float t, float n, float f)
{
	window[0] = l;
	window[1] = r;
	window[2] = b;
	window[3] = t;
	window[4] = n;
	window[5] = f;

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	viewportX = viewport[2];
	viewportY = viewport[3];

	pixelToViewRatio = (window[1]-window[0])/viewportX;

	UpdateProjection();
}


void GLCamera2D::ResetView()
{
	panX = 0.0;
	panY = 0.0;
	zoomLevel = 1.0/EARTH_RADIUS;

	UpdateModel();
	UpdateProjection();
}


void GLCamera2D::GetUnprojectedPoint(float x, float y, float *resultX, float *resultY)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLdouble modelview[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

	GLdouble projection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	y = (float)viewport[3] - y;

	GLdouble posX = 0.0;
	GLdouble posY = 0.0;
	GLdouble posZ = 0.0;
	gluUnProject((GLdouble)x, (GLdouble)y, (GLdouble)0.0, modelview, projection, viewport, &posX, &posY, &posZ);

	float aspectRatio = viewportX/viewportY;

	*resultX = aspectRatio*posX/zoomLevel - panX;
	*resultY = posY/zoomLevel - panY;
}


void GLCamera2D::UpdateModel()
{
	ModelMatrix = IDENTITY_MATRIX;

	TranslateMatrix(&ModelMatrix, panX, panY, 0);

	UpdateMVP();
}


void GLCamera2D::UpdateProjection()
{
	ProjectionMatrix = IDENTITY_MATRIX;

	ProjectionMatrix.m[0] = 2.0/(window[1]-window[0]);
	ProjectionMatrix.m[3] = -(window[1]+window[0])/(window[1]-window[0]);
	ProjectionMatrix.m[5] = 2.0/(window[3]-window[2]);
	ProjectionMatrix.m[7] = -(window[3]+window[2])/(window[3]-window[2]);
	ProjectionMatrix.m[10] = -2.0/(window[5]-window[4]);
	ProjectionMatrix.m[11] = -(window[5]+window[4])/(window[5]-window[4]);

	ScaleMatrix(&ProjectionMatrix, zoomLevel, zoomLevel, zoomLevel);

	UpdateMVP();
}


void GLCamera2D::UpdateMVP()
{
	Matrix VP = MultiplyMatrices(&ViewMatrix, &ProjectionMatrix);
	memcpy(&MVPMatrix.m, MultiplyMatrices(&ModelMatrix, &VP).m, sizeof(MVPMatrix.m));

}


Matrix GLCamera2D::MultiplyMatrices(const Matrix *m1, const Matrix *m2)
{
	Matrix out = IDENTITY_MATRIX;
	unsigned int row, column, row_offset;

	for (row=0, row_offset=row*4; row<4; ++row, row_offset=row*4)
	{
		for (column=0; column<4; ++column)
		{
			out.m[row_offset + column] =
					(m1->m[row_offset + 0] * m2->m[column+0]) +
					(m1->m[row_offset + 1] * m2->m[column+4]) +
					(m1->m[row_offset + 2] * m2->m[column+8]) +
					(m1->m[row_offset + 3] * m2->m[column+12]);
		}
	}

	return out;
}


void GLCamera2D::ScaleMatrix(Matrix *m, float x, float y, float z)
{
	Matrix scale = IDENTITY_MATRIX;

	scale.m[0] = x;
	scale.m[5] = y;
	scale.m[10] = z;

	memcpy(m->m, MultiplyMatrices(m, &scale).m, sizeof(m->m));
}


void GLCamera2D::TranslateMatrix(Matrix *m, float x, float y, float z)
{
	Matrix translation = IDENTITY_MATRIX;

	translation.m[12] = x;
	translation.m[13] = y;
	translation.m[14] = z;

	memcpy(m->m, MultiplyMatrices(m, &translation).m, sizeof(m->m));
}
