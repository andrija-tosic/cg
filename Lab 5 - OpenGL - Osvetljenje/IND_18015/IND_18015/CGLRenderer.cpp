#include "pch.h"
#include "CGLRenderer.h"

#include "GL\GL.h"
#include "GL\GLU.h"


#define RAD(a) 3.141592*(a)/180

CGLRenderer::CGLRenderer()
{
	m_hrc = NULL;
	this->viewR = 20;

	this->viewAngleXY = 0;
	this->viewAngleXZ = 0;

	eyex = 0, eyey = 0, eyez = 0;
	centerx = 0, centery = 0, centerz = 0;
	upx = 0, upy = 1, upz = 0;

	this->RecalculateCamera();

	light1Mat.SetAmbient(SVETLO_1, 1);
	light3Mat.SetDiffuse(SVETLO_1, 1);
	light1Mat.SetEmission(SVETLO_1, 1);
	light3Mat.SetShininess(128);

	light2Mat.SetAmbient(SVETLO_2, 1);
	light3Mat.SetDiffuse(SVETLO_2, 1);
	light2Mat.SetEmission(SVETLO_2, 1);
	light3Mat.SetShininess(128);

	light3Mat.SetAmbient(SVETLO_3, 1);
	light3Mat.SetDiffuse(SVETLO_3, 1);
	light3Mat.SetEmission(SVETLO_3, 1);
	light3Mat.SetShininess(128);

	vaseMat1.SetAmbient(VAZA_BOJA_1_AMBIENT, 1.0);
	vaseMat1.SetDiffuse(VAZA_BOJA_1, 1.0);
	vaseMat1.SetSpecular(VAZA_BOJA_1_SPECULAR, 1);

	vaseMat2.SetAmbient(VAZA_BOJA_2_AMBIENT, 1.0);
	vaseMat2.SetDiffuse(VAZA_BOJA_2, 1.0);
	vaseMat2.SetSpecular(VAZA_BOJA_2_SPECULAR, 1);

	wallMat.SetAmbient(.2, .2, .2, 1);
	wallMat.SetDiffuse(.6, .6, .6, 1);
	wallMat.SetEmission(0, 0, 0, 1);
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
	pfd.cDepthBits = 32;
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
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);

		glEnable(GL_DEPTH_TEST);

		GLfloat lm_ambient[] = { 0.8, 0.8, 0.8, 1.0 };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

		GLfloat light_ambient[] = { 0.6, 0.6, 0.6, 1.0 };

		GLfloat light0_diffuse[] = { 0.4, 0.4, 0.4, 0 };
		GLfloat light0_specular[] = { 0.1, 0.1, 0.1, 0 };
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180);

		GLfloat light1_diffuse[] = { SVETLO_1, 1 };
		GLfloat light1_specular[] = { SVETLO_1_SPECULAR, 1 };
		glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
		glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25);

		GLfloat light2_diffuse[] = { SVETLO_2, 1 };
		GLfloat light2_specular[] = { SVETLO_2_SPECULAR, 1 };
		glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
		glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 25);

		GLfloat light3_diffuse[] = { SVETLO_3, 1 };
		GLfloat light3_specular[] = { SVETLO_3_SPECULAR, 1 };
		glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
		glLightfv(GL_LIGHT3, GL_SPECULAR, light3_specular);
		glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 25);

		glEnable(GL_LIGHTING);
	}

	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	{
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(40, (double)w / h, 1, 100);
		glMatrixMode(GL_MODELVIEW);
	}
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		gluLookAt(eyex, eyey, eyez,
			centerx, centery, centerz,
			upx, upy, upz);

		ConfigureLight(GL_LIGHT0, 0, 20, 0, 0, -1, 0, true);
		DrawLight(-20, 0, 0, 1, light1Mat, light3On);
		ConfigureLight(GL_LIGHT1, -20, 0, 0, 1, 0, 0, light3On);
		DrawLight(20, 0, 0, 1, light2Mat, light1On);
		ConfigureLight(GL_LIGHT2, 20, 0, 0, -1, 0, 0, light1On);
		DrawLight(0, 20, 0, 1, light3Mat, light2On);
		ConfigureLight(GL_LIGHT3, 0, 20, 0, 0, -1, 0, light2On);

		glPushMatrix();
		{
			DrawCube(40, 100);
			glTranslated(0, -20, 0);
			DrawBase();
			glTranslated(0, 12, 0);
			DrawVase();
		}
		glPopMatrix();

		glTranslated(0, -20, 0);
		DrawAxis(50);

	}
	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);

}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	{

	}
	wglMakeCurrent(NULL, NULL);

	if (m_hrc) {
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawLight(double posX, double posY, double posZ, double r, CGLMaterial& mat, bool lightOn)
{
	if (!lightOn) {
		return;
	}

	mat.SetFace(GL_FRONT);

	glPushMatrix();
	{
		glTranslated(posX, posY, posZ);
		DrawSphere(r, 10, 10);
	}
	glPopMatrix();
}

void CGLRenderer::ConfigureLight(GLenum light, double posX, double posY, double posZ, double dirX, double dirY, double dirZ, bool lightOn)
{

	if (!lightOn) {
		glDisable(light);
		return;
	}

	GLfloat light_pos[] = { posX, posY, posZ, 1 };
	GLfloat spot_dir[] = { dirX, dirY, dirZ, 1 };

	glLightfv(light, GL_POSITION, light_pos);
	glLightfv(light, GL_SPOT_DIRECTION, spot_dir);

	glEnable(light);
}

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta, bool onlyUpperHalf)
{
	int alphaStep = 180 / nSegAlpha;
	int betaStep = 360 / nSegBeta;

	glBegin(GL_TRIANGLE_STRIP);
	{
		for (int alpha = 0; alpha <= 90; alpha += alphaStep) {
			for (int beta = onlyUpperHalf ? 0 : -90; beta <= 360; beta += betaStep) {
				double x1 = cos(RAD(alpha)) * cos(RAD(beta));
				double y1 = sin(RAD(alpha));
				double z1 = cos(RAD(alpha)) * sin(RAD(beta));

				glNormal3d(x1, y1, z1);
				glVertex3d(r * x1, r * y1, r * z1);

				double x2 = cos(RAD(alpha + alphaStep)) * cos(RAD(beta));
				double y2 = sin(RAD(alpha + alphaStep));
				double z2 = cos(RAD(alpha + alphaStep)) * sin(RAD(beta));

				glNormal3d(x2, y2, z2);
				glVertex3d(r * x2, r * y2, r * z2);
			}
		}
	}
	glEnd();
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, CGLMaterial& mat, int nSeg, bool includeBase)
{
	int alphaStep = 360 / nSeg;

	mat.SetFace(GL_FRONT_AND_BACK);

	if (includeBase) {
		glNormal3d(0, -1, 0);

		glBegin(GL_TRIANGLE_FAN);
		{
			glVertex3d(0, 0, 0);

			for (int alpha = 0; alpha <= 360; alpha += alphaStep) {

				double x = r1 * cos(RAD(alpha));
				double z = r1 * sin(RAD(alpha));

				glVertex3d(x, 0, z);
			}
		}
		glEnd();

		glNormal3d(0, 1, 0);

		glBegin(GL_TRIANGLE_FAN);
		{
			glVertex3d(0, h, 0);

			for (int alpha = 0; alpha <= 360; alpha += alphaStep) {

				double x = r2 * cos(RAD(alpha));
				double z = r2 * sin(RAD(alpha));

				glVertex3d(x, h, z);
			}
		}
		glEnd();
	}

	double x = (r1 - r2) / 2;
	double L = sqrt(pow(x, 2) + pow(h, 2));
	double ny = x / L;
	double nr = h / L;

	// Omotac.
	glBegin(GL_TRIANGLE_STRIP);
	{

		for (int alpha = 0; alpha <= 360; alpha += alphaStep) {
			glNormal3d(nr * cos(RAD(alpha)), ny, nr * sin(RAD(alpha)));
			glVertex3d(r1 * cos(RAD(alpha)), 0, r1 * sin(RAD(alpha)));

			glVertex3d(r2 * cos(RAD(alpha)), h, r2 * sin(RAD(alpha)));
		}
	}
	glEnd();

	if (!normalsOn) {
		return;
	}

	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	{
		glColor3d(0, 1, 0);

		for (int alpha = 0; alpha <= 360; alpha += alphaStep) {
			double x1 = r1 * cos(RAD(alpha));
			double z1 = r1 * sin(RAD(alpha));

			double x2 = r2 * cos(RAD(alpha));
			double z2 = r2 * sin(RAD(alpha));


			glVertex3d(x1, 0, z1);
			glVertex3d(x1 + nr * cos(RAD(alpha)), ny, z1 + nr * sin(RAD(alpha)));

			glVertex3d(x2, h, z2);
			glVertex3d(x2 + nr * cos(RAD(alpha)), h + ny, z2 + nr * sin(RAD(alpha)));
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawBase()
{
	wallMat.SetFace(GL_FRONT);

	glPushMatrix();
	DrawSphere(6, 100, 100, true);
	DrawCylinder(10, 3, 3, wallMat, 8, true);
	glTranslated(0, 10, 0);

	DrawCuboid(2, 15, 15, 2, 100, 100);
	glPopMatrix();
}

void CGLRenderer::DrawVase()
{
	glPushMatrix();
	{
		DrawCylinder(R_H, R_R1, R_R2, vaseMat2);
		glTranslated(0, R_H, 0);
		DrawCylinder(R_H, R_R2, R_R3, vaseMat1);
		glTranslated(0, R_H, 0);

		DrawCylinder(R_H, R_R3, R_R3, vaseMat2);
		glTranslated(0, R_H, 0);
		DrawCylinder(R_H, R_R3, R_R3, vaseMat1);
		glTranslated(0, R_H, 0);

		DrawCylinder(R_H, R_R3, R_R2, vaseMat2);
		glTranslated(0, R_H, 0);
		DrawCylinder(R_H, R_R2, R_R3, vaseMat1);
		glTranslated(0, R_H, 0);

		DrawCylinder(R_H, R_R3, R_R2, vaseMat2);
		glTranslated(0, R_H, 0);
		DrawCylinder(R_H, R_R2, R_R1, vaseMat1);
		glTranslated(0, R_H, 0);

		DrawCylinder(R_H, R_R1, R_R2, vaseMat2);
		glTranslated(0, R_H, 0);
		DrawCylinder(R_H, R_R2, R_R3, vaseMat1);
		glTranslated(0, R_H, 0);

		DrawCylinder(R_H, R_R3, R_R2, vaseMat2);
		glTranslated(0, R_H, 0);
		DrawCylinder(R_H, R_R2, R_R3, vaseMat1);
		glTranslated(0, R_H, 0);

		DrawCylinder(R_H, R_R3, R_R2, vaseMat2);
		glTranslated(0, R_H, 0);
		DrawCylinder(R_H, R_R2, R_R1, vaseMat1);
		glTranslated(0, R_H, 0);

	}
	glPopMatrix();
}

void CGLRenderer::DrawWall(double h, int nSeg)
{
	double segH = h / nSeg;

	wallMat.SetFace(GL_FRONT);

	glNormal3d(0, 0, 1);
	glBegin(GL_QUADS);
	{
		for (double i = 0; i <= h - segH; i += segH) {
			for (double j = 0; j <= h - segH; j += segH) {
				glVertex3d(j, i, 0);
				glVertex3d(j + segH, i, 0);
				glVertex3d(j + segH, i + segH, 0);
				glVertex3d(j, i + segH, 0);
			}
		}
	}
	glEnd();
}

void CGLRenderer::DrawSide(double h, double w, int nSegH, int nSegW)
{
	double segW = w / nSegW;
	double segH = h / nSegH;

	glNormal3d(0, 0, 1);
	glBegin(GL_QUADS);
	{
		for (double i = 0; i < h; i += segH) {
			for (double j = 0; j < w; j += segW) {
				glVertex3d(j, i, 0);
				glVertex3d(j + segW, i, 0);
				glVertex3d(j + segW, i + segH, 0);
				glVertex3d(j, i + segH, 0);
			}
		}
	}
	glEnd();
}

void CGLRenderer::DrawCube(double h, int nSeg)
{
	glEnable(GL_CULL_FACE);

	glPushMatrix();
	{
		glTranslated(-h / 2, -h / 2, -h / 2);

		glPushMatrix();
		{
			DrawWall(h, nSeg);

			glPushMatrix();
			{
				glRotated(90, 0, 1, 0);
				glTranslated(-h, 0, 0);
				DrawWall(h, nSeg);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glRotated(-90, 0, 1, 0);
				glTranslated(0, 0, -h);
				DrawWall(h, nSeg);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glRotated(180, 0, 1, 0);
				glTranslated(-h, 0, -h);
				DrawWall(h, nSeg);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glRotated(90, 1, 0, 0);
				glTranslated(0, 0, -h);
				DrawWall(h, nSeg);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glRotated(-90, 1, 0, 0);
				glTranslated(0, -h, 0);
				DrawWall(h, nSeg);
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();

	glDisable(GL_CULL_FACE);

}

void CGLRenderer::DrawCuboid(double h, double w, double d, int nSegH, int nSegW, int nSegD)
{
	// prednja
	glPushMatrix();
	{
		glTranslated(-w / 2, 0, d / 2);
		DrawSide(h, w, nSegH, nSegW);
	}
	glPopMatrix();

	// zadnja
	glPushMatrix();
	{
		glRotated(180, 0, 1, 0);
		glTranslated(-w / 2, 0, d / 2);
		DrawSide(h, w, nSegH, nSegW);
	}
	glPopMatrix();

	// gornja
	glPushMatrix();
	{
		glRotated(90, 1, 0, 0);
		glTranslated(-w / 2, -d / 2, -h);
		DrawSide(w, d, nSegH, nSegW);
	}
	glPopMatrix();

	// donja
	glPushMatrix();
	{
		glRotated(90, 1, 0, 0);
		glTranslated(-w / 2, -d / 2, 0);
		DrawSide(w, d, nSegH, nSegW);
	}
	glPopMatrix();

	//leva
	glPushMatrix();
	{
		glRotated(90, 0, 1, 0);
		glTranslated(-d / 2, 0, -w / 2);
		DrawSide(h, d, nSegH, nSegW);
	}
	glPopMatrix();
	//desna
	glPushMatrix();
	{
		glRotated(-90, 0, 1, 0);
		glTranslated(-d / 2, 0, -w / 2);
		DrawSide(h, d, nSegH, nSegW);
	}
	glPopMatrix();

}

void CGLRenderer::DrawAxis(double width)
{
	glLineWidth(1);
	glPointSize(10);

	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	{
		glColor3d(1.0, 0.0, 0.0);
		glVertex3d(0, 0, 0);
		glVertex3d(width, 0, 0);

		glColor3d(0.0, 1.0, 0.0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, width, 0);

		glColor3d(0.0, 0.0, 1.0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, width);
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void CGLRenderer::RotateView(double dXY, double dXZ)
{
	this->viewAngleXY += dXY;
	this->viewAngleXZ += dXZ;
	this->RecalculateCamera();
}

void CGLRenderer::RecalculateCamera()
{
	double dWXY = this->viewAngleXY * 3.141592 / 180,
		dWXZ = this->viewAngleXZ * 3.141592 / 180;

	this->eyex = this->viewR * cos(dWXY) * cos(dWXZ);
	this->eyey = 0 + this->viewR * sin(dWXY);
	this->eyez = this->viewR * cos(dWXY) * sin(dWXZ);

	this->upy = signbit(cos(dWXY)) ? -1 : 1;
}

void CGLRenderer::ZoomCamera(bool direction)
{
	viewR += direction ? 1 : -1;

	viewR = max(1, viewR);
	viewR = min(viewR, 80);

	RecalculateCamera();
}
