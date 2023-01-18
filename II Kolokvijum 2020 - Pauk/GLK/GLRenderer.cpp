#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
//#pragma comment(lib, "GL\\glut32.lib")

#include <string>

CGLRenderer::CGLRenderer(void)
{
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
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);

	this->backTex = LoadTexture("res/back.jpg");
	this->frontTex = LoadTexture("res/front.jpg");
	this->topTex = LoadTexture("res/top.jpg");
	this->botTex = LoadTexture("res/bot.jpg");
	this->leftTex = LoadTexture("res/left.jpg");
	this->rightTex = LoadTexture("res/right.jpg");
	this->spiderTex = LoadTexture("res/spider.png");

	float light0[] = { 1,0,0, 1 };
	float lightPos[] = { -50,-50,-50,1 };

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef(0, -5, -25 + this->zoom);

	glRotatef(5 + this->xRot, 1, 0, 0);
	glRotatef(-5 + this->yRot, 0, 1, 0);

	glTranslatef(0, 50, 0);
	DrawEnvCube(100);
	glTranslatef(0, -50, 0);

	//DrawAxes();

	glPushMatrix();
	{
		glRotatef(this->spiderYRot, 0, 1, 0);
		glTranslatef(this->spiderX, 4, 0);

		DrawSpider();
	}
	glPopMatrix();

	//---------------------------------
	glFlush();
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
	gluPerspective(50, (double)w / h, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);

	glDeleteTextures(1, &this->backTex);
	glDeleteTextures(1, &this->frontTex);
	glDeleteTextures(1, &this->topTex);
	glDeleteTextures(1, &this->botTex);
	glDeleteTextures(1, &this->leftTex);
	glDeleteTextures(1, &this->rightTex);
	glDeleteTextures(1, &this->spiderTex);


	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawAxes()
{
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINES);
	{
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(10, 0, 0);

		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 10, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 10);
	}
	glEnd();

	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
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

void CGLRenderer::DrawEnvCube(double a)
{
	//glDisable(GL_DEPTH_TEST);

	// back
	glBindTexture(GL_TEXTURE_2D, this->backTex);
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
	glBindTexture(GL_TEXTURE_2D, this->frontTex);
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
	glBindTexture(GL_TEXTURE_2D, this->topTex);
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
	glBindTexture(GL_TEXTURE_2D, this->botTex);
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
	glBindTexture(GL_TEXTURE_2D, this->leftTex);
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
	glBindTexture(GL_TEXTURE_2D, this->rightTex);
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

	//glEnable(GL_DEPTH_TEST);
}

void CGLRenderer::DrawSphere(double r, int nSeg, double texU, double texV, double texR)
{
	double ang1, ang2;
	double dAng1, dAng2;

	dAng1 = 3.141592 / (double)nSeg;
	dAng2 = 2 * 3.141592 / (double)nSeg;

	ang1 = -3.141592 / 2;

	glBindTexture(GL_TEXTURE_2D, this->spiderTex);

	for (int i = 0; i < nSeg; i++) {

		ang2 = 0;
		glBegin(GL_QUAD_STRIP);

		for (int j = 0; j < nSeg + 1; j++) {
			double x1 = r * cos(ang1) * cos(ang2);
			double y1 = r * sin(ang1);
			double z1 = r * cos(ang1) * sin(ang2);

			double x2 = r * cos(ang1 + dAng1) * cos(ang2);
			double y2 = r * sin(ang1 + dAng1);
			double z2 = r * cos(ang1 + dAng1) * sin(ang2);

			double tx1 = texR * x1 / r + texU;
			double ty1 = texR * z1 / r + texV;
			double tx2 = texR * x2 / r + texU;
			double ty2 = texR * z2 / r + texV;

			glTexCoord2f(tx1, ty1);
			glVertex3d(x1, y1, z1);

			glTexCoord2f(tx2, ty2);
			glVertex3d(x2, y2, z2);

			ang2 += dAng2;
		}
		glEnd();

		ang1 += dAng1;
	}
}

void CGLRenderer::DrawCone(double r, double h, int nSeg, double texU, double texV, double texR)
{
	double ang = 0;
	double dAng = 2 * 3.141592 / nSeg;

	glBindTexture(GL_TEXTURE_2D, this->spiderTex);

	glBegin(GL_TRIANGLE_FAN);

	glTexCoord2f(texU, texV);
	glVertex3f(0, h, 0);

	for (int i = 0; i < nSeg + 1; i++) {
		double x = r * cos(ang);
		double z = r * sin(ang);

		glTexCoord2f(texU + texR * cos(ang), texV + texR * sin(ang));
		glVertex3f(x, 0, z);

		ang += dAng;
	}
	glEnd();
}

void CGLRenderer::DrawLegSegment(double r, double h, int nSeg)
{
	DrawSphere(r, nSeg, 0.75, 0.75, 0.25);
	DrawCone(r, h, nSeg, 0.75, 0.75, 0.25);
}

void CGLRenderer::DrawLeg()
{
	glPushMatrix();
	{
		if (this->qRot == 0)
			glRotatef(50, 1, 0, 0);

		DrawLegSegment(1, 10, 5);
		glTranslatef(0, 10, 0);
		glRotatef(85, 1, 0, 0);
		DrawLegSegment(1, 15, 5);
	}
	glPopMatrix();
}

void CGLRenderer::DrawSpiderBody()
{
	glBindTexture(GL_TEXTURE_2D, this->spiderTex);

	glPushMatrix();
	{
		// head
		glPushMatrix();
		{
			glScalef(1, 0.8, 1);
			DrawSphere(5, 10, 0.25, 0.25, 0.25);
		}
		glPopMatrix();

		glTranslatef(5, 0, 0);


		// body
		glPushMatrix();
		{
			glScalef(1, 0.8, 1);
			DrawSphere(3, 10, 0.25, 0.25, 0.25);
		}
		glPopMatrix();

		glTranslatef(3, 0, 0);


		// tail
		glPushMatrix();
		{
			glScalef(1, 0.5, 1);
			DrawSphere(2, 10, 0.75, 0.25, 0.25);
		}
		glPopMatrix();

	}
	glPopMatrix();
}

void CGLRenderer::DrawSpider()
{
	glTranslatef(0, this->qRot / 5, 0);

	DrawSpiderBody();

	glTranslatef(5, 0, 0);

	glPushMatrix();
	{
		glRotatef(-45, 0, 1, 0);

		if (this->qRot != 0) {
			glRotatef(5 * this->qRot + this->spiderX + this->spiderYRot, 0, 1, 0);
		}


		int legs = this->qRot == 0 ? 8 : 16;

		for (int i = 0; i < legs; i++) {
			glRotatef(30, 0, 1, 0);
			DrawLeg();
		}
	}
	glPopMatrix();
}
