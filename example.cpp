#include <opencv2/opencv.hpp>
#include "glslprogram.h"
#include "GL/glut.h"


const int screenWidth = 4000;	   // width of the screen window in pixels 
const int screenHeight = 2000;	   // height of the screen window in pixels
GLuint texId = 0;
cv::Mat img;

//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
 void myInit(void)
 {
	img = cv::imread("d:\\1.bmp");
	if(img.empty()) {
		printf("Error imread.\n");
		return ;
	}
	cv::cvtColor(img, img, CV_BGR2RGB);
	
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	// Create the OpenGL texture map
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.cols, img.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);

}

//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, texId);
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
	glutSwapBuffers();
	
	//glDrawPixels(img.cols, img.rows, GL_RGB, GL_UNSIGNED_BYTE, img.data);
	//glutSwapBuffers();
}

void glslProcess()
{

	//cv::namedWindow("test",CV_WINDOW_AUTOSIZE );
	//cv::imshow("test",img);
	//cv::waitKey(0);
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
	//glutInit(&argc, argv);          // initialize the toolkit
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // set the display mode
	glutInitWindowSize(1440, 1440); // set the window size
	glutInitWindowPosition(0, 0); // set the window position on screen
	glutCreateWindow("opengl example"); // open the screen window
	glutDisplayFunc(myDisplay);     // register the redraw function
	myInit(); 
	GLenum err = glewInit();
	if(GLEW_OK != err) {
		printf("glewInit Error: %s\n", glewGetErrorString(err));
		getchar();
		return ;
	}
	glslProcess();
	glutMainLoop(); 		     // go into a perpetual loop
	//cv::imwrite("d:\\11.bmp", img);
}
