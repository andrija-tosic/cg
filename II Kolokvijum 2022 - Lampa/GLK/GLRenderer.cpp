#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

#define _USE_MATH_DEFINES
#include "math.h"

#include "DImage.h"

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

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(0, 0, 0, 1);

	topTex = LoadTexture("res/top.jpg");
	botTex = LoadTexture("res/bot.jpg");
	sideTex = LoadTexture("res/side.jpg");
	lampTex = LoadTexture("res/lamp.jpg");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef(0, -15, -25 + this->zoom);

	glRotatef(5 + this->xRot, 1, 0, 0);
	glRotatef(-5 + this->yRot, 0, 1, 0);

	glTranslatef(0, 50, 0);
	DrawEnvCube(100);
	glTranslatef(0, -50, 0);

	DrawAxes();

	DrawLamp();


	//---------------------------------
	glFlush();
	SwapBuffers(pDC->m_hDC);

	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
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

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);

	glDeleteTextures(1, &topTex);
	glDeleteTextures(1, &botTex);
	glDeleteTextures(1, &sideTex);
	glDeleteTextures(1, &lampTex);


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
		glVertex3f(50, 0, 0);

		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 50, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 50);
	}
	glEnd();

	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
}

UINT CGLRenderer::LoadTexture(char* fileName) {
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

void CGLRenderer::DrawEnvCube(double a) {

	// back
	glBindTexture(GL_TEXTURE_2D, this->sideTex);
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
	glBindTexture(GL_TEXTURE_2D, this->sideTex);
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
	glBindTexture(GL_TEXTURE_2D, this->sideTex);
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
	glBindTexture(GL_TEXTURE_2D, this->sideTex);
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

}

void CGLRenderer::DrawCylinder(double r1, double r2, double h, int nSeg, int texMode, bool bIsOpen) {
	double dAng = 2 * M_PI / (double)nSeg;

	glBindTexture(GL_TEXTURE_2D, lampTex);

	double texU, texV;

	if (texMode) { // donja
		texU = 0.5;
		texV = 0.25;
	}
	else { // gornja
		texU = 0.5;
		texV = 0.75;
	}

	glTexCoord2f(texU, texV);
	glVertex3f(0, 0, 0);

	glBegin(GL_TRIANGLE_FAN);
	{
		double ang = 0;

		for (int i = 0; i < nSeg + 1; i++) {
			double x = r1*cos(ang);
			double z = r1*sin(ang);

			glTexCoord2f(0.5 + cos(ang)*0.5, 0.25 + sin(ang)*0.25);
			glVertex3f(x, 0, z);

			ang += dAng;
		}
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	{
		double ang = -M_PI;
		double texAng = -M_PI;

		for (int i = 0; i < nSeg + 1; i++) {
			double x1 = r1*cos(ang);
			double z1 = r1*sin(ang);

			double x2 = r2*cos(ang);
			double z2 = r2*sin(ang);

			double s = (cos(texAng) + 1.0) / 2.0;

			glTexCoord2f(s, texV - 0.25);
			glVertex3f(x1, 0, z1);

			glTexCoord2f(s, texV + 0.25);
			glVertex3f(x2, h, z2);

			ang += dAng;
			texAng += dAng;

			if (texAng > 0) {
				texAng = -M_PI;
			}
		}
	}
	glEnd();

	if (!bIsOpen) {
		glTexCoord2f(texU, texV);
		glVertex3f(0, h, 0);

		glBegin(GL_TRIANGLE_FAN);
		{
			double ang = 0;

			for (int i = 0; i < nSeg + 1; i++) {
				double x = r2*cos(ang);
				double z = r2*sin(ang);

				glTexCoord2f(0.5 + cos(ang)*0.5, 0.25 + sin(ang)*0.25);
				glVertex3f(x, h, z);

				ang += dAng;
			}
		}
		glEnd();
	}
}

void CGLRenderer::DrawLampBase() {
	DrawCylinder(8, 7, 2, 20, 0, false);
}

void CGLRenderer::DrawLampArm() {
	glRotatef(30, 0, 0, 1);
	glPushMatrix();
	{
		glTranslatef(0, 0, -1);

		glRotatef(90, 1, 0, 0);

		DrawCylinder(3, 3, 2, 20, 0, false);
	}
	glPopMatrix();

	DrawCylinder(1, 1, 15, 20, 0, false);
}

void CGLRenderer::DrawLampHead() {
	DrawCylinder(2, 3, 1, 20, 0, false);
	glTranslatef(0, 1, 0);

	DrawCylinder(3, 3, 5, 20, 0, true);
	glTranslatef(0, 5, 0);

	DrawCylinder(3, 6, 5, 20, 1, true);
}

void CGLRenderer::DrawLamp() {
	glPushMatrix();
	{
		DrawLampBase();
		glTranslatef(0, 1, 0);

		glRotatef(arm1Rot, 0, 0, 1);
		DrawLampArm();
		glTranslatef(0, 15, 0);

		glRotatef(-90, 0, 0, 1);

		glRotatef(arm2Rot, 0, 0, 1);
		DrawLampArm();

		glTranslatef(0, 15, 0);

		glRotatef(headRot, 0, 0, 1);
		glPushMatrix();
		{
			glTranslatef(0, 0, -1);

			glRotatef(90, 1, 0, 0);
			DrawCylinder(2, 2, 2, 20, 1, false);
		}
		glPopMatrix();

		glTranslatef(-1, 0, 0);

		glRotatef(-15 - 90, 0, 0, 1);

		DrawLampHead();
	}
	glPopMatrix();
}