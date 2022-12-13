#pragma once

#include "gl\gl.h"

class CGLRenderer
{
	HGLRC m_hrc;

public:
	bool CreateGLContext(CDC* pDC); // kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC); // inicijalizuje scenu
	void Reshape(CDC* pDC, int w, int h); // kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda
	void DrawScene(CDC* pDC); // iscrtava scenu
	void DestroyScene(CDC* pDC); // oslobadja resurse alocirane u drugim funkcijama ove klase
};

