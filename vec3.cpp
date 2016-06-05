#include "vec3.h"


Vec3::Vec3( float x, float y, float z )
{
	v0 = x;
	v1 = y;
	v2 = z;
}

Vec3::Vec3( const Vec3& that )
{
	this->v0 = that.v0;
	this->v1 = that.v1;
	this->v2 = that.v2;
}

Vec3&
Vec3::operator=( const Vec3& rhs )
{
	this->v0 = rhs.v0;
	this->v1 = rhs.v1;
	this->v2 = rhs.v2;
	return *this;
}

Vec3&
Vec3::operator*=( float f )
{
	this->v0 *= f;
	this->v1 *= f;
	this->v2 *= f;
	return *this;
}

Vec3
Vec3::operator+( const Vec3& that )
{
	Vec3 result;
	result.v0 = this->v0 + that.v0;
	result.v1 = this->v1 + that.v1;
	result.v2 = this->v2 + that.v2;
	return result;
}

Vec3&
Vec3::operator+=( const Vec3& that )
{
	this->v0 += that.v0;
	this->v1 += that.v1;
	this->v2 += that.v2;
	return *this;
}

Vec3
Vec3::operator-( const Vec3& that )
{
	Vec3 result;
	result.v0 = this->v0 - that.v0;
	result.v1 = this->v1 - that.v1;
	result.v2 = this->v2 - that.v2;
	return result;
}

Vec3
Vec3::operator-( )
{
	Vec3 result;
	result.v0 = -this->v0;
	result.v1 = -this->v1;
	result.v2 = -this->v2;
	return result;
}

Vec3&
Vec3::operator-=( const Vec3& that )
{
	this->v0 -= that.v0;
	this->v1 -= that.v1;
	this->v2 -= that.v2;
	return *this;
}

Vec3
Vec3::Cross( Vec3 that )
{
	Vec3 tmp;

	tmp.v0 = this->v1*that.v2 - that.v1*this->v2;
	tmp.v1 = this->v2*that.v0 - that.v2*this->v0;
	tmp.v2 = this->v0*that.v1 - that.v0*this->v1;
	return tmp;
}

float
Vec3::Dot( Vec3 that )
{
	return this->v0*that.v0 + this->v1*that.v1 + this->v2*that.v2;
}

float
Vec3::Length( )
{
	return sqrt( SQR(this->v0) + SQR(this->v1) + SQR(this->v2) );
}

void
Vec3::Print( char *str, FILE *fp )
{
	fprintf( fp, "%s [ %8.3f %8.3f %8.3f ]\n", str, v0, v1, v2 );
}

Vec3
Vec3::Unit( )
{
	Vec3 tmp;

	float len = this->Length( );
	if( len > 0. )
	{
		tmp.v0 /= len;
		tmp.v1 /= len;
		tmp.v2 /= len;
	}
	return tmp;
}

void
Vec3::Unitize( )
{
	float len = this->Length( );
	if( len > 0. )
	{
		this->v0 /= len;
		this->v1 /= len;
		this->v2 /= len;
	}
}

Point3::Point3( float x, float y, float z ) : Vec3( x, y, z )
{
}

Point3::Point3( const Point3& that ) : Vec3( that.v0, that.v1, that.v2 )
{
}

#ifdef TEST
int
main( int argc, char * argv[ ] )
{
	Vec3 b( 1., 2., 3. );
	Vec3 c( 5., 6., 7. );
	Vec3 d( c );

	Vec3 a = c;
	a.Unitize( );
	a.Print( "a =" );
	b.Print( "b =" );
	c.Print( "c =" );
	d.Print( "d =" );

	a = Vec3( 2., -5., 8. );
	a.Print( "a =" );

	Vec3 ma = -a;
	ma.Print( "-a = " );

	Vec3 e = a + b;
	e.Print( "e =" );

	e *= 3.;
	e.Print( "e =" );

	float f = ( a + b ).Length( );
	fprintf( stderr, "f = %8.3f\n", f );

	float g = a.Dot(  (b+c).Unit( )  );
	fprintf( stderr, "g = %8.3f\n", g );

	Vec3 h = a.Cross(b);
	h.Print( "axb =" );

	float i = c.Dot( a.Cross(b) );
	fprintf( stderr, "c.(axb) = %8.3f\n", i );

	Point3 Q( 1., 2., 0. );
	Point3 R( 5., 3., 0. );
	Point3 S( 3., 6., 0. );
	Vec3 normal = ( R-Q ).Cross( S-Q );
	normal.Print( "normal = " );
	float area = 0.5 * normal.Length( );
	fprintf( stderr, "triangle area = %8.3f\n", area );

	return 0;
}
#endif

#ifdef TEST_RESULTS
a = [    0.477    0.572    0.667 ]
b = [    1.000    2.000    3.000 ]
c = [    5.000    6.000    7.000 ]
d = [    5.000    6.000    7.000 ]
a = [    2.000   -5.000    8.000 ]
-a =  [   -2.000    5.000   -8.000 ]
e = [    3.000   -3.000   11.000 ]
e = [    9.000   -9.000   33.000 ]
f =   11.790
g =    0.000
axb = [  -31.000    2.000    9.000 ]
c.(axb) =  -80.000
normal =  [    0.000    0.000   14.000 ]
triangle area =    7.000
#endif