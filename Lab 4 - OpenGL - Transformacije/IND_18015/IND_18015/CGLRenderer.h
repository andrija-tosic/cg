#pragma once

#include "gl\gl.h"

class CGLRenderer
{
	HGLRC m_hrc;

#define	POZADINA 0.47, 0.00, 0.00
#define VAZA 1.00, 1.00, 1.00
#define DEO_KAKTUSA 0.50, 0.75, 1.00
#define SFERA 0.00, 0.8, 0.0
#define GRID 1.00, 0.6, 0.0


public:
	float angle = 0;

	double viewAngleXY;
	double viewAngleXZ;
	double viewR;

	double eyex, eyey, eyez;
	double centerx, centery, centerz;
	double upx, upy, upz;

	CGLRenderer();

	bool CreateGLContext(CDC* pDC); // kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC); // inicijalizuje scenu
	void Reshape(CDC* pDC, int w, int h); // kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda
	void DrawScene(CDC* pDC); // iscrtava scenu
	void DestroyScene(CDC* pDC); // oslobadja resurse alocirane u drugim funkcijama ove klase

	void DrawSphere(double r, int nSegAlpha, int nSegBeta);
	void DrawCylinder(double h, double r1, double r2, int nSeg);
	void DrawCone(double h, double r, int nSeg);
	void DrawAxis(double width);
	void DrawGrid(double width, double height, int nSegW, int nSegH);
	void DrawFigure(double angle);

	void RotateView(double dXY, double dXZ);
	void CalculatePosition();
};

