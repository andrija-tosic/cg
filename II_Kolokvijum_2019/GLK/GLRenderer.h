#pragma once

class CGLRenderer
{
	UINT backTxt, botTxt, excavatorTxt, frontTxt, leftTxt, rightTxt, topTxt;


public:
	float viewZoom = 0;
	float yRot = 0;
	float upViewRot = 0;

	float bodyRot = 0, arm1Rot = 0, arm2Rot = 0, forkRot = 0;

	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawAxis();
	UINT LoadTexture(char* fileName);
	void DrawPolygon(POINTF* points, POINTF* textCoords, int n);
	void DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b);
	void DrawBase();
	void DrawBody();
	void DrawArm(double zh);
	void DrawFork();
	void DrawExcavator();
	void DrawEnvCube(double a);



protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
