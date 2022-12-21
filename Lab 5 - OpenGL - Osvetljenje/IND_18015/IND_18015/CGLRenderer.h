#pragma once

#include "CGLMaterial.h"

#define	VAZA_BOJA_1 1.00, 0.00, 1.00
#define VAZA_BOJA_1_AMBIENT .25, 0, .25
#define VAZA_BOJA_1_SPECULAR .5, 0, .5

#define VAZA_BOJA_2 1.00, 1.00, 0.00
#define VAZA_BOJA_2_AMBIENT 0.25, 0.25, 0
#define VAZA_BOJA_2_SPECULAR 0.5, 0.5, 0

#define SVETLO_1 1.00, 1.00, 0.00
#define SVETLO_1_SPECULAR 0.4, 0.4, 0

#define SVETLO_2 0.00, 0.00, 1.00
#define SVETLO_2_SPECULAR 0.4, 0, 0.4

#define SVETLO_3 1.00, 0.00, 0.00
#define SVETLO_3_SPECULAR 0.4, 0, 0

constexpr double R_R1 = 5;
constexpr double R_R2 = 3.5;
constexpr double R_R3 = 2;
constexpr double R_H = 1.2;

class CGLRenderer
{

	HGLRC m_hrc;

	CGLMaterial vaseMat1, vaseMat2, light1Mat, light2Mat, light3Mat, tableMat, wallMat;

public:
	bool light3On = true, light2On = true, light1On = true, normalsOn = true;

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

	void DrawLight(double posX, double posY, double posZ, double r, CGLMaterial& mat, bool lightOn = true);
	void ConfigureLight(GLenum light, double posX, double posY, double posZ, double dirX, double dirY, double dirZ, bool lightOn = true);

	void DrawSphere(double r, int nSegAlpha, int nSegBeta, bool onlyUpperHalf = false);
	void DrawCylinder(double h, double r1, double r2, CGLMaterial& mat, int nSeg = 40, bool includeBase = false);
	void DrawBase();
	void DrawVase();
	void DrawWall(double h, int nSeg);
	void DrawSide(double h, double w, int nSegH, int nSegW);
	void DrawCube(double h, int nSeg);
	void DrawCuboid(double h, double w, double d, int nSegH, int nSegW, int nSegD);
	void DrawAxis(double width);

	void RotateView(double dXY, double dXZ);
	void RecalculateCamera();
	void ZoomCamera(bool direction);
};

