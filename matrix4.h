// TO DO:
//	Fix GetInverse and Invert
//	Fix FrustumZ
//	Fix StereoPerspective
//	Write some test code

#ifndef MATRIX4_H
#define MATRIX4_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _USE_MATH_INCLUDES
// #include <math.h>
#include <cmath>
#include <algorithm>
#include <GL/gl.h>

#include "vec3.h"

#define STACK_SIZE	32

class Matrix4
{
     protected:
	GLfloat m[4][4];
	GLfloat	stack[STACK_SIZE][4][4];
	int	top;			// next matrix to fill with a Push( )
	float	GetSubDeterminant( int, int );

    public:
	Matrix4( );
	Matrix4( const Matrix4& );
	Matrix4( float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float );
	Matrix4( float [4][4] );

	Matrix4&	Frustum( float, float,  float, float,  float, float );
	Matrix4&	FrustumZ( float, float,  float, float,  float, float,  float );
	float		GetDeterminant( );
	float		GetElement( int, int );
	Matrix4		GetInverse( );
	Matrix4		GetInverse3( );
	void		GetMatrix4( float [4][4] );
	void		GetMatrix43( float [3][3] );
	Matrix4		GetTranspose( );
	Matrix4		GetTranspose3( );
	Matrix4&	Invert( );
	Matrix4&	Invert3( );
	Matrix4&	LoadIdentity( );
	Matrix4&	LookAt( float, float, float,   float, float, float,   float, float, float );
	Vec3		MultBy( Vec3 );
	Matrix4&	operator=( const Matrix4& );
	Matrix4		operator*( float );
	Matrix4		operator*( Matrix4& );
	Point3		operator*( Point3& );
	Vec3		operator*( Vec3& );
	Matrix4&	operator*=( Matrix4& );
	Matrix4&	operator*=( float );
	Matrix4		operator+( Matrix4& );
	Matrix4&	operator+=( Matrix4& );
	Matrix4		operator-( Matrix4& );		// binary
	Matrix4&	operator-( );			// unary
	Matrix4&	operator-=( Matrix4& );
	Matrix4&	operator-=( float );
	Matrix4&	Ortho( float, float,  float, float,  float, float );
	Matrix4&	Ortho2D( float, float,  float, float );
	Matrix4&	Perspective( float, float, float, float );
	Matrix4&	PopMatrix4( );
	Matrix4&	Print( char * = "", FILE * = stderr );
	Matrix4&	PushMatrix4( );
	Matrix4&	Rotatef( float,  float, float, float );
	Matrix4&	Scalef( float, float, float );
	Matrix4&	SetElement( int, int, float );
	Matrix4&	SetMatrix4( float [4][4] );
	Matrix4&	SetMatrix43( float [3][3] );
	Matrix4&	StereoPerspective( float, float, float, float, float, float );
	Matrix4&	Translatef( float, float, float );
	Matrix4&	Transpose( );
};

#endif		// MATRIX4_H