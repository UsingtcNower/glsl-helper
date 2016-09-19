#include <opencv2/opencv.hpp>
#include "glslprogram.h"
#include <cstdio>
#include "glfw3.h"

#define FBO

const int screenWidth = 200;	   // width of the screen window in pixels 
const int screenHeight = 200;	   // height of the screen window in pixels
GLuint texId[2];
GLuint fb;
cv::Mat img;
cv::Mat img1;

//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
 void myInit(void)
 {
	img = cv::imread("1.png");
	if(img.empty()) {
		printf("Error imread.\n");
		return ;
	}
	//cv::cvtColor(img, img, CV_BGR2RGB);
	
	glGenTextures(2, texId);
	CheckGlErrors("check glGenTextures");
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	CheckGlErrors("check glBindTextures");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.cols, img.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	CheckGlErrors("check glTexImage2D");

#ifdef FBO
	img1 = cv::Mat(screenHeight, screenWidth, CV_8UC3);
	if(img1.empty()) {
		printf("Error create img1.\n");
		return ;
	}
	glBindTexture(GL_TEXTURE_2D, texId[1]);
	CheckGlErrors("check glBindTextures");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img1.cols, img1.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glGenFramebuffers(1, &fb);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, texId[1], 0);
#endif

	GLint nModelViewMatrixCount = 0;
	GLint nProjectionMatrixCount = 0;

	glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &nModelViewMatrixCount);// 32
	glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &nProjectionMatrixCount); // 4
	printf("before, modelview matrix count %d, projection matrix count %d\n", nModelViewMatrixCount, nProjectionMatrixCount);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, screenWidth, screenHeight);
	//glRotatef(90, 0, 0, -1);
	glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &nModelViewMatrixCount);// 32
	glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &nProjectionMatrixCount); // 4
	printf("after, modelview matrix count %d, projection matrix count %d\n", nModelViewMatrixCount, nProjectionMatrixCount);
	float mv_matrix[16];
	float projection_matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mv_matrix);
	glGetFloatv(GL_PROJECTION_MATRIX, projection_matrix);
	printf("model view matrix\n");
	for(int i=0;i<16;++i) printf("%f ", mv_matrix[i]);
	printf("\n");
	printf("projection matrix\n");
	for(int i=0;i<16;++i) printf("%f ", projection_matrix[i]);
	printf("\n");
	CheckGlErrors("myInit()");

}

//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void renderFBO(void)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	glTexSubImage2D(GL_TEXTURE_2D,0, 0, 0, img.cols, img.rows, GL_RGB, GL_UNSIGNED_BYTE, img.data);
	CheckGlErrors("glTexSubImage2D");
	
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
	checkFramebufferStatus();
	CheckGlErrors("render");
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
	int argc_ = 1;
	char *argv_[1] = {(char *)"something"};
	//glutInit(&argc_, argv_);          // initialize the toolkit
	//GLuint glutwindow = glutCreateWindow("something"); // open the screen window
	if(!glfwInit()) {
		printf("glfwinit fail");
		return ;
	}
	CheckGlErrors("glfwInit");
	GLFWwindow *glfwwindow = glfwCreateWindow(screenWidth, screenHeight, "example", NULL, NULL);
	CheckGlErrors("glfwCreateWindow");
	if(glfwwindow == NULL) {
		printf("glfwCreateWindow fail");
		glfwTerminate();
		return ;
	}
	glfwHideWindow(glfwwindow);
	CheckGlErrors("glfwHideWindow");
	glfwMakeContextCurrent(glfwwindow);
	CheckGlErrors("glfwMakeContextCurrent");
	
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
	for(int i=1;i<3;++i) {
		char filename[30];
		sprintf(filename, "%d.png", i+1);
		img = cv::imread(filename);
		if(img.empty())
			return ;
		//cv::cvtColor(img, img, CV_BGR2RGB);
		renderFBO();
		readBack();
		//cv::cvtColor(img1, img1, CV_BGR2RGB);
		printf("output img, width is %d, height is %d, %d\n", img1.cols, img1.rows, img1.channels());
		
		sprintf(filename, "D:\\%d.bmp", 20+i);
		cv::imwrite(filename, img1);
	}
	//getchar();
#else
	glutMainLoop(); 		     // go into a perpetual loop
#endif
	glDeleteFramebuffers(1, &fb);
	glDeleteTextures(2, texId);
	//glutDestroyWindow(glutwindow);
	glfwTerminate();

	getchar();
}
