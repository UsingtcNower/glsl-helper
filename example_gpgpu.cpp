#include <opencv2/opencv.hpp>
#include "glslprogram.h"
#include <cstdio>

//#define GPGPU

const int screenWidth = 4000;	   // width of the screen window in pixels 
const int screenHeight = 2000;	   // height of the screen window in pixels
GLuint texId[2];
GLuint fb;
cv::Mat img;
cv::Mat img1;

//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
 void myInit(void)
 {
	img = cv::imread("d:\\1.bmp");
	if(img.empty()) {
		printf("Error imread.\n");
		return ;
	}
	img1 = cv::Mat(1440, 1440, CV_8UC4);

	//glGenFramebuffers(1, &fb);
	//glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glGenTextures(2, texId);

	// tex0
	glActiveTexture(GL_TEXTURE0);
	// can replace by GL_TEXTURE_RECTANGLE_ARB
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	// Create the OpenGL texture map
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.cols, img.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
		img.cols, img.rows, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//// tex1,输出
	//glActiveTexture(GL_TEXTURE1);
	//// can replace by GL_TEXTURE_RECTANGLE_ARB
	//glBindTexture(GL_TEXTURE_2D, texId[1]);
	//// Create the OpenGL texture map
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	////纹理缓存关联到帧缓存对象(FBO)
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
	//	GL_TEXTURE_2D, texId[1], 0);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img1.cols, img1.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
	//	img1.cols, img1.rows, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	////glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//// 将帧缓存对象设置为当前绘制缓存
	//glDrawBuffer(GL_COLOR_ATTACHMENT0);

		
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(0, screenWidth, 0, screenHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glViewport(0, 0, screenWidth, screenHeight);

}

void renderFBO(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT, GL_FILL);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(0, 0);
	glTexCoord2f(1.0, 0);
	glVertex2f(1.0, 0);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(1.0, 1.0);
	glTexCoord2f(0, 1.0);
	glVertex2f(0, 1.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void readBack()
{
	glReadBuffer(fb);
	glReadPixels(0, 0, img1.cols, img1.rows, GL_RGBA, GL_UNSIGNED_BYTE, img1.data);
}

void glslProcess()
{
	GLSLProgram glprog;
	glprog.SetGstap(true);
	glprog.SetVerbose(true);

	if(!glprog.Create("d:\\v0.vert", "d:\\v0.frag")) {
		return ;
	}
	glprog.Use();

}

void main(int argc, char ** argv)
{
	glutInit(&argc, argv);          // initialize the toolkit
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set the display mode
	glutInitWindowSize(1440, 1440); // set the window size
	glutInitWindowPosition(0, 0); // set the window position on screen
	glutCreateWindow("gpgpu example"); // open the screen window
#if  !defined(GPGPU)
	glutDisplayFunc(renderFBO);
#endif
	GLenum err = glewInit();
	if(GLEW_OK != err) {
		printf("glewInit Error: %s\n", glewGetErrorString(err));
		getchar();
		return ;
	}
	myInit();
#ifdef GPGPU
	renderFBO();
#endif
	glslProcess();
#ifdef GPGPU
	readBack();
#else
	glutMainLoop();
#endif
	cv::imwrite("d:\\11.bmp", img1);
}
