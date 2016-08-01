#include <opencv2/opencv.hpp>
#include "glslprogram.h"
#include <cstdio>

#define FBO

const int screenWidth = 1440;	   // width of the screen window in pixels 
const int screenHeight = 1440;	   // height of the screen window in pixels
GLuint texId[3];
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
	cv::cvtColor(img, img, CV_BGR2RGB);
	
	glGenTextures(3, texId);
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.cols, img.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);

#ifdef FBO
	img1 = cv::Mat(screenWidth, screenHeight, CV_8UC3);
	if(img1.empty()) {
		printf("Error create img1.\n");
		return ;
	}
	glBindTexture(GL_TEXTURE_2D, texId[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img1.cols, img1.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, img1.data);
	glGenFramebuffers(1, &fb);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, texId[1], 0);
#endif

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, screenWidth, screenHeight);
	glRotatef(90, 0, 0, -1);

}

//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void renderFBO(void)
{
#ifdef FBO
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
#endif
	glClear(GL_COLOR_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); 
	glVertex2f(-1, -1);
	glTexCoord2f(1, 0); 
	glVertex2f(1, -1);
	glTexCoord2f(1, 1); 
	glVertex2f(1, 1);
	glTexCoord2f(0, 1); 
	glVertex2f(-1, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
#ifndef FBO
	glutSwapBuffers();
#endif
}

void glslProcess()
{
	GLSLProgram glprog;
	glprog.SetGstap(true);
	glprog.SetVerbose(true);

	if(!glprog.Create("D:\\v0.vert", "D:\\v0.frag")) {
		return ;
	}
	glprog.Use();

}


void readBack()
{
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glReadPixels(0, 0, img1.cols, img1.rows, GL_RGB, GL_UNSIGNED_BYTE, img1.data);
}


void main(int argc, char ** argv)
{
	glutInit(&argc, argv);          // initialize the toolkit
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // set the display mode
	glutInitWindowSize(1440, 1440); // set the window size
	glutInitWindowPosition(0, 0); // set the window position on screen
	glutCreateWindow("opengl fbo example"); // open the screen window
#ifndef FBO
	glutDisplayFunc(renderFBO);     // register the redraw function
#endif
	GLenum err = glewInit();
	if(GLEW_OK != err) {
		printf("glewInit Error: %s\n", glewGetErrorString(err));
		getchar();
		return ;
	}
	myInit(); 
	glslProcess();
#ifdef FBO
	renderFBO();
	readBack();
	cv::cvtColor(img1, img1, CV_BGR2RGB);
	printf("output img, width is %d, height is %d\n", img1.cols, img1.rows);
	cv::imwrite("d:\\11.bmp", img1);
	getchar();
#else
	glutMainLoop(); 		     // go into a perpetual loop
#endif
}
