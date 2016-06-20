#include <opencv2/opencv.hpp>
#include "glslprogram.h"
#include "GL/glut.h"


const int screenWidth = 640;	   // width of the screen window in pixels 
const int screenHeight = 360;	   // height of the screen window in pixels
GLuint texId[3];
cv::Mat img;

//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
 void myInit(void)
 {
	img = cv::imread("D:\\code\\glman\\shaders\\private\\me.bmp");
	if(img.empty()) {
		printf("Error imread.\n");
		return ;
	}
	cv::cvtColor(img, img, CV_BGR2RGB);
	
	glGenTextures(3, texId);
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	// Create the OpenGL texture map
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.cols, img.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);

	FILE* hFile = fopen("D:\\code\\glman\\shaders\\private\\1.dat", "rb");
	int w = 256, h = 256;
	char* buf  = new char[w*h];
	fread(buf, 1, w*h, hFile);
	fclose(hFile);
	cv::Mat img1(w, h, CV_8UC4);
	int k = 0;
	for(int i=0;i<img1.rows;++i) {
		uchar* p = img1.ptr<uchar>(i);
		for(int j=0;j<img1.cols;++j) {
			p[j+3] = buf[k++];
			p[j] = p[j+1] = p[j+2] = 0;
		}
	}
	delete[] buf;

	glBindTexture(GL_TEXTURE_2D, texId[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, img1.cols, img1.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, img1.data);


	hFile = fopen("D:\\code\\glman\\shaders\\private\\2.dat", "rb");
	w = 2, h = 1024;
	buf  = new char[w*h];
	fread(buf, 1, w*h, hFile);
	fclose(hFile);
	cv::Mat img2(w, h, CV_8UC4);
	k = 0;
	for(int i=0;i<img2.rows;++i) {
		uchar* p = img2.ptr<uchar>(i);
		for(int j=0;j<img2.cols;++j) {
			p[j+3] = buf[k++];
			p[j] = p[j+1] = p[j+2] = 0;
		}
	}
	delete[] buf;

	glBindTexture(GL_TEXTURE_2D, texId[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, img1.cols, img1.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, img1.data);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(180, 0, 0, 1);
}

//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void myDisplay(void)
{
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

	if(!glprog.Create("D:\\code\\glman\\shaders\\private\\mopi_meibai.vert", "D:\\code\\glman\\shaders\\private\\mopi_meibai.frag")) {
		return ;
	}
	glprog.Use();

}

void main(int argc, char ** argv)
{
	//glutInit(&argc, argv);          // initialize the toolkit
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // set the display mode
	glutInitWindowSize(screenWidth, screenHeight); // set the window size
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
