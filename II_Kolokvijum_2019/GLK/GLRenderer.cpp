#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
#include <string>
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	m_hrc = NULL;
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	m_hrc = wglCreateContext(pDC->m_hDC);

	if (!m_hrc) return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);

	backTxt = LoadTexture("res/back.jpg");
	botTxt = LoadTexture("res/bot.jpg");
	frontTxt = LoadTexture("res/front.jpg");
	leftTxt = LoadTexture("res/left.jpg");
	rightTxt = LoadTexture("res/right.jpg");
	topTxt = LoadTexture("res/top.jpg");
	excavatorTxt = LoadTexture("res/excavator.png");

	glEnable(GL_TEXTURE_2D);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();


	glTranslatef(0, -5, -30 + viewZoom);

	glRotatef(upViewRot, 1, 0, 0);
	glRotatef(yRot, 0, 1, 0);

	glTranslatef(0, 50, 0);
	DrawEnvCube(100);
	glTranslatef(0, -50, 0);

	DrawAxis();
	DrawExcavator();

	glFlush();
	//---------------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(55, (double)w / h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);

	glDeleteTextures(1, &backTxt);
	glDeleteTextures(1, &topTxt);
	glDeleteTextures(1, &leftTxt);
	glDeleteTextures(1, &rightTxt);
	glDeleteTextures(1, &excavatorTxt);
	glDeleteTextures(1, &botTxt);
	glDeleteTextures(1, &frontTxt);


	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawAxis() {
	glLineWidth(1);
	glPointSize(10);
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINES);
	{
		glColor3d(0, 0, 1);
		glVertex3d(0, 0, 0);
		glVertex3d(10, 0, 0);

		glColor3d(1, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 10, 0);

		glColor3d(0, 1, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, 10);
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);

	glColor3f(1, 1, 1);
}

UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT texID;
	DImage img;
	img.Load(CString(fileName));
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}

void CGLRenderer::DrawPolygon(POINTF* points, POINTF* textCoords, int n)
{
	glBegin(GL_TRIANGLE_FAN);
	{
		for (int i = 0; i < n; i++) {
			glTexCoord2f(textCoords[i].x, textCoords[i].y);
			glVertex3f(points[i].x, points[i].y, 0);
		}
	}
	glEnd();
}

void DrawPolygonOtherSide(POINTF* points, POINTF* textCoords, int n, float zh) {
	glBegin(GL_TRIANGLE_FAN);
	{
		for (int i = 0; i < n; i++) {
			glTexCoord2f(textCoords[i].x, textCoords[i].y);
			glVertex3f(points[i].x, points[i].y, zh);
		}
	}
	glEnd();
}

void CGLRenderer::DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b)
{
	DrawPolygon(points, texCoords, n);
	glDisable(GL_TEXTURE_2D);
	glColor3f(r, g, b);

	glBegin(GL_TRIANGLE_STRIP);
	{
		for (int i = 0; i < n; i++) {
			glVertex3f(points[i].x, points[i].y, 0);
			glVertex3f(points[i].x, points[i].y, zh);
		}
	}
	glEnd();

	glEnable(GL_TEXTURE_2D);

	glColor3f(1, 1, 1);
	DrawPolygonOtherSide(points, texCoords, n, zh);
}

void CGLRenderer::DrawBase()
{
	POINTF points[] = {
		{0, 0.5},
		{0.5, 0},

		{15 * 0.5, 0},
		{16 * 0.5, 0.5},

		{16 * 0.5, 3 * 0.5},
		{15 * 0.5, 4 * 0.5},

		{0.5, 4 * 0.5},
		{0, 3 * 0.5}
	};

	POINTF texCoords[] = {
		{0. / 16, 13. / 16},
		{1. / 16, 12. / 16},

		{15. / 16, 12. / 16},
		{16. / 16, 13. / 16},

		{16. / 16, 15. / 16},
		{15. / 16, 16. / 16},

		{1. / 16, 16. / 16},
		{0. / 16, 15. / 16}
	};

	DrawPolygon(points, texCoords, sizeof(points) / sizeof(POINTF));
	DrawExtrudedPolygon(points, texCoords, sizeof(points) / sizeof(POINTF), 5, 0, 0, 0);
}

void CGLRenderer::DrawBody()
{
	POINTF points[] = {
		{0, 0},
		{4 * 0.5, 0},

		{8 * 0.5, 4 * 0.5},
		{8 * 0.5, 8 * 0.5},

		{0, 8 * 0.5}
	};

	POINTF texCoords[] = {
		{8. / 16, 0},
		{12. / 16, 0},

		{16. / 16, 4. / 16},
		{16. / 16, 8. / 16},

		{8. / 16, 8. / 16}
	};


	DrawPolygon(points, texCoords, sizeof(points) / sizeof(POINTF));
	DrawExtrudedPolygon(points, texCoords, sizeof(points) / sizeof(POINTF), 4, 0.96, 0.5, 0.12);
}

void CGLRenderer::DrawArm(double zh)
{
	POINTF points[] = {
	{0, 0.5},
	{0.5, 0},

	{16 * 0.5, 0.5},

	{16 * 0.5, 3 * 0.5},

	{0.5, 4 * 0.5},
	{0, 3 * 0.5}
	};

	POINTF texCoords[] = {
		{0. / 16, 9. / 16},
		{1. / 16, 8. / 16},

		{16. / 16, 9. / 16},

		{16. / 16, 11. / 16},

		{1. / 16, 12. / 16},
		{0. / 16, 11. / 16}
	};

	DrawPolygon(points, texCoords, sizeof(points) / sizeof(POINTF));
	DrawExtrudedPolygon(points, texCoords, sizeof(points) / sizeof(POINTF), zh, 0.96, 0.5, 0.12);
}

void CGLRenderer::DrawFork()
{
	POINTF points[] = {
		{0, 0.5},
		{0.5, 0},

		{7 * 0.5, 0},
		{8 * 0.5, 0.5},

		{8 * 0.5, 6 * 0.5},
		{0, 6 * 0.5}
	};

	POINTF texCoords[] = {
		{0, 1},
		{1, 0},

		{7, 0},
		{8, 1},

		{8, 6},
		{0, 6}
	};

	for (int i = 0; i < sizeof(texCoords) / sizeof(POINTF); i++) {
		texCoords[i].x /= 16.0;
		texCoords[i].y /= 16.0;
	}

	DrawPolygon(points, texCoords, sizeof(points) / sizeof(POINTF));
	DrawExtrudedPolygon(points, texCoords, sizeof(points) / sizeof(POINTF), 1.0, 0.7, 0.7, 0.7);
}

void CGLRenderer::DrawExcavator()
{
	glBindTexture(GL_TEXTURE_2D, this->excavatorTxt);
	glPushMatrix();
	{
		DrawBase();

		glTranslatef(6, 4 * 0.5, 1 * 0.5);
		
		float offset = 2 * 0.5;
		glTranslatef(-2*offset, 0, 2*offset);
		glRotatef(bodyRot, 0, 1, 0);
		glTranslatef(2*offset, 0, -2*offset);
		
		glRotatef(90, 0, 0, 1);
		DrawBody();

		glRotatef(-90, 0, 0, 1);

		glTranslatef(-1, -1, 2);

		// arm1 rot

		// pomeranje u centar rotacije
		glTranslatef(offset, offset, 0);

		glTranslatef(offset, offset, 0);
		glRotatef(90 + arm1Rot, 0, 0, 1);
		glTranslatef(-offset, -offset, 0);

		DrawArm(1.0);

		// pomeranje do druge tacke
		glTranslatef(12 * 0.5, 0, 0);

		// arm2 rot
		// pomeranje u centar rotacije

		glTranslatef(offset, offset, 0);
		glRotatef(-90 + arm2Rot, 0, 0, 1);
		glTranslatef(-offset, -offset, 0);

		DrawArm(1.5);

		// pomeranje do druge tacke
		glTranslatef(13 * 0.5, 6*0.5, 0);

		glScalef(1, -1, 1);

		// rotacija fork
		glTranslatef(2 * 0.5, 4 * 0.5, 0);
		glRotatef(90 + forkRot, 0, 0, 1);
		glTranslatef(-2 * 0.5, -4 * 0.5, 0);

		DrawFork();
	}
	glPopMatrix();

}

void CGLRenderer::DrawEnvCube(double a)
{
	glDisable(GL_DEPTH_TEST);

	// back
	glBindTexture(GL_TEXTURE_2D, this->backTxt);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a / 2, -a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, -a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, -a / 2, -a / 2);
	}
	glEnd();

	// front
	glBindTexture(GL_TEXTURE_2D, this->frontTxt);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a / 2, a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, -a / 2, a / 2);
	}
	glEnd();


	// top
	glBindTexture(GL_TEXTURE_2D, this->topTxt);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a / 2, -a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(a / 2, a / 2, a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, a / 2, a / 2);
	}
	glEnd();

	// bottom
	glBindTexture(GL_TEXTURE_2D, this->botTxt);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, -a / 2, -a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(a / 2, -a / 2, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, -a / 2, a / 2);
	}
	glEnd();

	// left
	glBindTexture(GL_TEXTURE_2D, this->leftTxt);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a / 2, a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(-a / 2, a / 2, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(-a / 2, -a / 2, -a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, -a / 2, a / 2);
	}
	glEnd();

	// right
	glBindTexture(GL_TEXTURE_2D, this->rightTxt);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(a / 2, a / 2, a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, -a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(a / 2, -a / 2, a / 2);
	}
	glEnd();

	glEnable(GL_DEPTH_TEST);
}
