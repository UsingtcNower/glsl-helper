#include <stdio.h>
#include <cmath>

#ifndef VEC3_H
#define VEC3_H

class Vec3
{
    protected:
	float v0, v1, v2;

    public:
	Vec3( float = 0., float = 0., float = 0. );
	Vec3( const Vec3& );
	Vec3&	operator=( const Vec3& );
	Vec3&	operator*=( float );
	Vec3	operator+( const Vec3& );
	Vec3&	operator+=( const Vec3& );
	Vec3	operator-( const Vec3& );	// binary -
	Vec3	operator-( );			// unary -
	Vec3&	operator-=( const Vec3& );
	Vec3	Cross( Vec3 );
	float	Dot( Vec3 );
	float	Length( );
	void	Print( char * = "", FILE * = stderr );
	Vec3	Unit( );
	void	Unitize( );

	friend class Matrix4;
};


class Point3 : public Vec3
{
    public:
	Point3( float = 0., float = 0., float = 0. );
	Point3( const Point3& );
};


inline float
SQR( float f )
{
	return f * f;
}

#endif