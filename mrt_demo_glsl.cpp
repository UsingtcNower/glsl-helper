/*
 * License (based on zlib/libpng):
 *
 * Copyright (c) 2005--2007 Dominik Göddeke, University of Dortmund, Germany.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from 
 * the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose, 
 * including commercial applications, and to alter it and redistribute it 
 * freely.
 *
 */



/*
 * GPGPU Multiple Render Target mini-Tutorial: 
 * Simple demonstration how to output to several textures in one pass.
 * The application basically computes
 * (1) passthrough
 * (2) -passthrough
 * (3) 100*passthrough
 * (4) -100*passthrough
 * in one pass for a given input array.
 *
 * Limitation: Only works on Geforce6 and better. The way it is implemented,
 * it probably won't work on ATI. 
 *
 * Please drop me a note if you encounter any bugs, or if you have suggestions 
 * on how to improve this tutorial: dominik.goeddeke@math.uni-dortmund.de
 *
 */


// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/glut.h>


// problem size
int texSize = 5;


// texture identifiers
GLuint outputTexID[4];
GLuint inputTexID;

// output buffer identifiers
// you have been warned to change these, since a lot is 
// hard-coded. Also, more than four outputs are not supported
// on any hardware at the time of writing.
GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, 
		     GL_COLOR_ATTACHMENT1_EXT, 
		     GL_COLOR_ATTACHMENT2_EXT, 
		     GL_COLOR_ATTACHMENT3_EXT };


// Cg vars
GLuint glslProgram;
GLuint fragmentShader;
GLint inputParam;

// FBO identifier
GLuint fb;

// handle to offscreen "window", only used to properly shut down the app
GLuint glutWindowHandle;

// shortcuts for texture internals
// you have been warned to change these, since a lot is 
// hard-coded.
GLenum texTarget = GL_TEXTURE_RECTANGLE_ARB;
GLenum texInternalFormat = GL_FLOAT_R32_NV;
GLenum texFormat = GL_LUMINANCE;

// shader source. Simply reads in values from input texture and distributes modified values to the outputs
const GLchar* shader = \
"#extension GL_ARB_draw_buffers : enable\n       "\
"#extension GL_ARB_texture_rectangle : enable\n  "\
"uniform sampler2DRect input;                    "\
"void main(void) {                               "\
"  vec2 coords = gl_TexCoord[0].xy;              "\
"  float value = texture2DRect(input, coords).x; "\
"  gl_FragData[0].x = value;                     "\
"  gl_FragData[1].x = -1.0 * value;              "\
"  gl_FragData[2].x = 100.0 * value;             "\
"  gl_FragData[3].x = -100.0 * value; }          ";

// input and result data
float* inputData;
float* outputData0;
float* outputData1;
float* outputData2;
float* outputData3;


/*
 * error checking for GLSL
 */
void printInfoLogs(GLuint obj, GLuint shader) {
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
    if (infologLength > 1) {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf(infoLog);
        printf("\n");
        free(infoLog);
    }
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);
    if (infologLength > 1) {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
        printf(infoLog);
        printf("\n");
        free(infoLog);
    }
}

/**
 * Checks for OpenGL errors.
 * Extremely useful debugging function: When developing, 
 * make sure to call this after almost every GL call.
 */
void checkGLErrors (const char *label) {
    GLenum errCode;
    const GLubyte *errStr;
    if ((errCode = glGetError()) != GL_NO_ERROR) {
		errStr = gluErrorString(errCode);
		printf("OpenGL ERROR: ");
		printf((char*)errStr);
		printf("(Label: ");
		printf(label);
		printf(")\n.");
    }
}

/**
 * Checks framebuffer status.
 * Copied directly out of the spec, modified to deliver a return value.
 */
bool checkFramebufferStatus() {
    GLenum status;
    status = (GLenum) glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch(status) {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
	return true;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
	printf("Framebuffer incomplete, incomplete attachment\n");
	return false;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
	printf("Framebuffer incomplete, missing attachment\n");
	return false;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
	printf("Framebuffer incomplete, attached images must have same dimensions\n");
	return false;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
	printf("Framebuffer incomplete, attached images must have same format\n");
	return false;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
	printf("Framebuffer incomplete, missing draw buffer\n");
	return false;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
	printf("Framebuffer incomplete, missing read buffer\n");
	return false;
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
	printf("Unsupported framebuffer format\n");
	return false;
    }
    return false;
}


/**
 * Sets up a floating point texture with NEAREST filtering.
 * (mipmaps etc. are unsupported for floating point textures on most HW)
 */
void setupTexture (const GLuint texID) {
    glBindTexture(texTarget,texID);
    glTexParameteri(texTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(texTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(texTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(texTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(texTarget,0,texInternalFormat,texSize,texSize,0,texFormat,GL_FLOAT,0);
    checkGLErrors("setupTexture()");
}



/**
 * main, just calls things in the appropriate order
 */
int main(int argc, char **argv) {
    //
    // create data vectors
    //
    inputData = (float*)malloc(texSize*texSize*sizeof(float));
    outputData0 = (float*)malloc(texSize*texSize*sizeof(float));
    outputData1 = (float*)malloc(texSize*texSize*sizeof(float));
    outputData2 = (float*)malloc(texSize*texSize*sizeof(float));
    outputData3 = (float*)malloc(texSize*texSize*sizeof(float));
    for (int i=0; i<texSize*texSize; i++) 
	inputData[i] = i+1.0;
    //
    // init glut and glew
    //
    glutInit ( &argc, argv );
    glutWindowHandle = glutCreateWindow("MAXIMUM DEMO");  
    int err = glewInit();
    if (GLEW_OK != err) {
	printf((char*)glewGetErrorString(err));
	exit(2);
    }  
    //
    // init offscreen framebuffer, bind it and set viewport
    //
    glGenFramebuffersEXT(1, &fb); 
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, texSize, 0.0, texSize);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, texSize, texSize);
    checkGLErrors("initFBO()");	
    //
    // create textures
    //	
    glGenTextures (1, &inputTexID);
    setupTexture (inputTexID);
    glGenTextures (4, outputTexID);
    for (int i=0; i<4; i++)
	setupTexture (outputTexID[i]);
    //
    // init shader
    //
    glslProgram = glCreateProgram();
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shader, NULL);
    glCompileShader(fragmentShader);
    glAttachShader(glslProgram, fragmentShader);
    glLinkProgram(glslProgram);
    printInfoLogs(glslProgram, fragmentShader);
    // 
    // transfer input data to input texture
    //
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(texTarget, inputTexID);
    glTexSubImage2D(texTarget,0,0,0,texSize,texSize,texFormat,GL_FLOAT,inputData);
    checkGLErrors("inputTransfer()");		
    //
    // bind output textures for output
    //
    for (int i=0; i<4; i++) 
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, buffers[i], texTarget, outputTexID[i], 0);
    checkFramebufferStatus();
    checkGLErrors("bind output");	
    //
    // perform computation
    //
    glUseProgram(glslProgram);
    inputParam = glGetUniformLocation(glslProgram, "input");
    glUniform1i(inputParam,0);
    // set render destination
    glDrawBuffers(4,buffers);
    // trigger computation
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex2f(0.0, 0.0);
    glTexCoord2f(texSize, 0.0);
    glVertex2f(texSize, 0.0);
    glTexCoord2f(texSize, texSize);
    glVertex2f(texSize, texSize);
    glTexCoord2f(0.0, texSize);
    glVertex2f(0.0, texSize);
    glEnd();
    checkFramebufferStatus();
    checkGLErrors("render()");
    //
    // read back results and print them out
    //
    glReadBuffer(buffers[0]);
    glReadPixels(0, 0, texSize, texSize,texFormat,GL_FLOAT,outputData0);
    glReadBuffer(buffers[1]);
    glReadPixels(0, 0, texSize, texSize,texFormat,GL_FLOAT,outputData1);
    glReadBuffer(buffers[2]);
    glReadPixels(0, 0, texSize, texSize,texFormat,GL_FLOAT,outputData2);
    glReadBuffer(buffers[3]);
    glReadPixels(0, 0, texSize, texSize,texFormat,GL_FLOAT,outputData3);
    printf("input\toutput0\toutput1\toutput2\toutput3\n");
    for (int i=0; i<texSize*texSize; i++)
	printf("%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", inputData[i], outputData0[i], outputData1[i], outputData2[i], outputData3[i]);
    //
    // clean up
    //
    glDeleteFramebuffersEXT(1,&fb);
    free(inputData);
    free(outputData0);
    free(outputData1);
    free(outputData2);
    free(outputData3);
    glDeleteTextures(1,&inputTexID);
    glDeleteTextures(4,outputTexID);
    glutDestroyWindow (glutWindowHandle);
    return 0;
}

