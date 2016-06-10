#include "matrix4.h"

Matrix4::Matrix4( )
{
	LoadIdentity( );
	top = 0;
};

Matrix4::Matrix4( const Matrix4& in )
{
	memcpy( &this->m[0][0], &in.m[0][0], 16*sizeof(float) );
	top = 0;
};



Matrix4::Matrix4( float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p )
{
	this->m[0][0] = a;	this->m[0][1] = b;	this->m[0][2] = c;	this->m[0][3] = d;
	this->m[1][0] = e;	this->m[1][1] = f;	this->m[1][2] = g;	this->m[1][3] = h;
	this->m[2][0] = i;	this->m[2][1] = j;	this->m[2][2] = k;	this->m[2][3] = l;
	this->m[3][0] = m;	this->m[3][1] = n;	this->m[3][2] = o;	this->m[3][3] = p;
	top = 0;
};


Matrix4::Matrix4( float mat[4][4] )
{
	m[0][0] = mat[0][0];    m[0][1] = mat[0][1];    m[0][2] = mat[0][2];    m[0][3] = mat[0][3];   
	m[1][0] = mat[1][0];    m[1][1] = mat[1][1];    m[1][2] = mat[1][2];    m[1][3] = mat[1][3];   
	m[2][0] = mat[2][0];    m[2][1] = mat[2][1];    m[2][2] = mat[2][2];    m[2][3] = mat[2][3];   
	m[3][0] = mat[3][0];    m[3][1] = mat[3][1];    m[3][2] = mat[3][2];    m[3][3] = mat[3][3];   
	top = 0;
}



Matrix4&
Matrix4::Frustum( float l, float r, float b, float t, float n, float f )
{
	Matrix4 tmp(
			2.*n/(r-l), 0.,         (r+l)/(r-l),  0.,
			0.,         2.*n/(t-b), (t+b)/(t-b),  0.,
			0.,         0.,         -(f+n)/(f-n), -2.*f*n/(f-n),
			0.,         0.,         0.,           1.
	);

	this->operator*=( tmp );
	return *this;
}


Matrix4&
Matrix4::FrustumZ( float left, float right, float bottom, float top, float znear, float zfar, float zproj )
{
	if( zproj > 0.0 )
	{
		left   *= ( znear/zproj );
		right  *= ( znear/zproj );
		bottom *= ( znear/zproj );
		top    *= ( znear/zproj );
	}

	this->Frustum( left, right, bottom, top, znear, zfar );
	return *this;
}


float
Matrix4::GetDeterminant( )
{
	float det = 0.;
	det += this->m[0][0] * GetSubDeterminant( 0, 0 );
	det -= this->m[0][1] * GetSubDeterminant( 0, 1 );
	det += this->m[0][2] * GetSubDeterminant( 0, 2 );
	det -= this->m[0][3] * GetSubDeterminant( 0, 3 );
	return det;
}


float
Matrix4::GetSubDeterminant( int r, int c )
{
	float t[9];
	int here;

	here = 0;
	for( int row = 0; row < 4; row++ )
	{
		if( row == r )
			continue;

		for( int col = 0; col < 4; col++ )
		{
			if( col == c )
				continue;

			t[here] = this->m[row][col];
			here++;
		}
	}

	float det = 0.;
	det += ( t[0]*t[4]*t[8] + t[1]*t[5]*t[6] + t[2]*t[3]*t[7] );
	det -= ( t[2]*t[4]*t[6] + t[0]*t[5]*t[7] + t[1]*t[3]*t[8] );
	return det;
}


float
Matrix4::GetElement( int i, int j )
{
	if( i < 0  ||  i > 3 )		return 0.;
	if( j < 0  ||  j > 3 )		return 0.;
	return this->m[i][j];
}


Matrix4
Matrix4::GetInverse( )
{
}


Matrix4
Matrix4::GetInverse3( )
{
}


void
Matrix4::GetMatrix4( float out[4][4] )
{
	out[0][0] = this->m[0][0];    out[0][1] = this->m[0][1];    out[0][2] = this->m[0][2];    out[0][3] = this->m[0][3];   
	out[1][0] = this->m[1][0];    out[1][1] = this->m[1][1];    out[1][2] = this->m[1][2];    out[1][3] = this->m[1][3];   
	out[2][0] = this->m[2][0];    out[2][1] = this->m[2][1];    out[2][2] = this->m[2][2];    out[2][3] = this->m[2][3];   
	out[3][0] = this->m[3][0];    out[3][1] = this->m[3][1];    out[3][2] = this->m[3][2];    out[3][3] = this->m[3][3];   
}


void
Matrix4::GetMatrix43( float out[3][3] )
{
	out[0][0] = this->m[0][0];    out[0][1] = this->m[0][1];    out[0][2] = this->m[0][2];
	out[1][0] = this->m[1][0];    out[1][1] = this->m[1][1];    out[1][2] = this->m[1][2];
	out[2][0] = this->m[2][0];    out[2][1] = this->m[2][1];    out[2][2] = this->m[2][2];
}


Matrix4
Matrix4::GetTranspose( )
{
	Matrix4 result;
	result.m[0][0] = this->m[0][0];    result.m[1][0] = this->m[0][1];    result.m[2][0] = this->m[0][2];    result.m[3][0] = this->m[0][3];   
	result.m[0][1] = this->m[1][0];    result.m[1][1] = this->m[1][1];    result.m[2][1] = this->m[1][2];    result.m[3][1] = this->m[1][3];   
	result.m[0][2] = this->m[2][0];    result.m[1][2] = this->m[2][1];    result.m[2][2] = this->m[2][2];    result.m[3][2] = this->m[2][3];   
	result.m[0][3] = this->m[3][0];    result.m[1][3] = this->m[3][1];    result.m[2][3] = this->m[3][2];    result.m[3][3] = this->m[3][3];   
	return result;
}


Matrix4&
Matrix4::Invert( )
{
	// Uses the Gauss-Jordan elimination method with full pivoting
	// (Numerical Recipes, pp24-29)

	const float EPSILON =		1.0e-6;
	const float SMALL   =		1.0e-3;

	float aa[4][4];			// tmp copy of this->[m]
	bool usedAsPivot[4];		// has this column been used as a pivot yet?
	int rowIndex[4];		// where are the rows really
	int colIndex[4];		// where are the columns really

	// setup:

	for( int j = 0; j < 4; j++ )
		usedAsPivot[j] = false;

	memcpy( aa, this->m, 4*4*sizeof(GLfloat) );	// copy this->m to aa

	// main column reduction loop:

	for( int i = 0; i < 4; i++ )
	{
		// look for biggest number to make the pivot:

		float big = 0.;
		int prow = -1;
		int pcol = -1;
		for( int j = 0; j < 4; j++ )
		{
			if( ! usedAsPivot[j] )
			{
				for( int k = 0; k < 4; k++ )
				{
					if( ! usedAsPivot[k]  &&  fabs( aa[j][k] ) > big )
					{
						big = fabs( aa[j][k] );
						prow = j;
						pcol = k;
					}
				}
			}
		}

		// if didn't find a big number, then matrix is singular:

		if( fabs(big) <= EPSILON )
			return *this;

		// flag this column as already having been used as a pivot:

		usedAsPivot[pcol] = true;

		// interchange rows to put the pivot element on the diagonal:
		// (cols are not interchanged, just indexed)

		if( prow != pcol )
		{
			for( int m = 0; m < 4; m++ )
			{
				std::swap( aa[prow][m], aa[pcol][m] );
			}
		}

		if( fabs( aa[pcol][pcol] ) <= EPSILON )
			return *this;

		rowIndex[i] = prow;
		colIndex[i] = pcol;

		// reduce the pivot row:

		float divide = aa[pcol][pcol];
		aa[pcol][pcol] = 1.;

		for( int m = 0; m < 4; m++ )
		{
			aa[pcol][m] /= divide;
		}

		// reduce the other rows:

		for( int mm = 0; mm < 4; mm++ )
		{
			if( mm != pcol )
			{
				float mul = aa[mm][pcol];
				aa[mm][pcol] = 0.;
				for( int m = 0; m < 4; m++ )
					aa[mm][m] -= mul * aa[pcol][m];
			}
		}
	}

	// unscramble the columns so that the inverse is correct:

	for( int m = 4 - 1; m >= 0; m-- )
	{
		if( rowIndex[m] != colIndex[m] )
		{
			for( int k = 0; k < 4; k++ )
			{
				std::swap(  aa[k][ rowIndex[m] ],  aa[k][ colIndex[m] ]  );
			}
		}
	}

	// successful:

	memcpy( this->m, aa, 4*4*sizeof(GLfloat) );	// copy aa to this->m

	return *this;
}


Matrix4&
Matrix4::Invert3( )
{
	this->m[0][3] = this->m[1][3] = this->m[2][3] = 0.;
	this->m[3][0] = this->m[3][1] = this->m[3][2] = 0.;
	this->m[3][3] = 1.;
	this->Invert( );
	return *this;
}



Matrix4&
Matrix4::LoadIdentity( )
{
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.;
	m[0][1] = m[0][2] = m[0][3] = 0.;
	m[1][0] = m[1][2] = m[1][3] = 0.;
	m[2][0] = m[2][1] = m[2][3] = 0.;
	m[3][0] = m[3][1] = m[3][2] = 0.;
	return *this;
}



Matrix4&
Matrix4::LookAt(	float eyex, float eyey, float eyez,		// eye pt */
		float centerx, float centery, float centerz,	// look-at pt */
		float upx, float upy, float upz )		// up vector */
{
	// establish the w axis:

	Vec3 w( eyex - centerx, eyey - centery, eyez - centerz );
	w.Unitize( );

	// establishthe v axis:

	Vec3 v( upx, upy, upz );
	v.Unitize( );

	// the u axis is perpendicular to both v and w:

	Vec3 u = v.Cross( w );
	u.Unitize( );

	// be sure the up axis is really perpendicular to u and w:

	v = w.Cross( u );

	// produce a matrix such that:
	//
	//	  ux uy uz       px
	//	[ vx vy vz ] * { py }
	//	  wx wy wz       pz
	//
	//

	Matrix4 tmp(	u.v0, u.v1, u.v2, 0.,
			v.v0, v.v1, v.v2, 0.,
			w.v0, w.v1, w.v2, 0.,
			0.,   0.,   0.,   1.
	);

	tmp.Translatef( -eyex, -eyey, -eyez );

	this->operator*=( tmp );
}


Matrix4&
Matrix4::operator=( const Matrix4& that )
{
	this->m[0][0] = that.m[0][0]; this->m[0][1] = that.m[0][1]; this->m[0][2] = that.m[0][2]; this->m[0][3] = that.m[0][3];
	this->m[1][0] = that.m[1][0]; this->m[1][1] = that.m[1][1]; this->m[1][2] = that.m[1][2]; this->m[1][3] = that.m[1][3];
	this->m[2][0] = that.m[2][0]; this->m[2][1] = that.m[2][1]; this->m[2][2] = that.m[2][2]; this->m[2][3] = that.m[2][3];
	this->m[3][0] = that.m[3][0]; this->m[3][1] = that.m[3][1]; this->m[3][2] = that.m[3][2]; this->m[3][3] = that.m[3][3];

	this->top = that.top;

	memcpy( this->stack[0], that.stack[0], STACK_SIZE*4*4*sizeof(float) );

	return *this;
}

Matrix4&
Matrix4::operator*=( Matrix4& that )
{
	// if I had CEAN, I could say things like:
	//	tmp.m[i][j] = this->m[i][0:4] * that.m[0:4][j] ;

	Matrix4 tmp;
	tmp.m[0][0] = this->m[0][0] * that.m[0][0]  +  this->m[0][1] * that.m[1][0]  +  this->m[0][2] * that.m[2][0]  +  this->m[0][3] * that.m[3][0];
	tmp.m[0][1] = this->m[0][0] * that.m[0][1]  +  this->m[0][1] * that.m[1][1]  +  this->m[0][2] * that.m[2][1]  +  this->m[0][3] * that.m[3][1];
	tmp.m[0][2] = this->m[0][0] * that.m[0][2]  +  this->m[0][1] * that.m[1][2]  +  this->m[0][2] * that.m[2][2]  +  this->m[0][3] * that.m[3][2];
	tmp.m[0][3] = this->m[0][0] * that.m[0][3]  +  this->m[0][1] * that.m[1][3]  +  this->m[0][2] * that.m[2][3]  +  this->m[0][3] * that.m[3][3];

	tmp.m[1][0] = this->m[1][0] * that.m[0][0]  +  this->m[1][1] * that.m[1][0]  +  this->m[1][2] * that.m[2][0]  +  this->m[1][3] * that.m[3][0];
	tmp.m[1][1] = this->m[1][0] * that.m[0][1]  +  this->m[1][1] * that.m[1][1]  +  this->m[1][2] * that.m[2][1]  +  this->m[1][3] * that.m[3][1];
	tmp.m[1][2] = this->m[1][0] * that.m[0][2]  +  this->m[1][1] * that.m[1][2]  +  this->m[1][2] * that.m[2][2]  +  this->m[1][3] * that.m[3][2];
	tmp.m[1][3] = this->m[1][0] * that.m[0][3]  +  this->m[1][1] * that.m[1][3]  +  this->m[1][2] * that.m[2][3]  +  this->m[1][3] * that.m[3][3];

	tmp.m[2][0] = this->m[2][0] * that.m[0][0]  +  this->m[2][1] * that.m[1][0]  +  this->m[2][2] * that.m[2][0]  +  this->m[2][3] * that.m[3][0];
	tmp.m[2][1] = this->m[2][0] * that.m[0][1]  +  this->m[2][1] * that.m[1][1]  +  this->m[2][2] * that.m[2][1]  +  this->m[2][3] * that.m[3][1];
	tmp.m[2][2] = this->m[2][0] * that.m[0][2]  +  this->m[2][1] * that.m[1][2]  +  this->m[2][2] * that.m[2][2]  +  this->m[2][3] * that.m[3][2];
	tmp.m[2][3] = this->m[2][0] * that.m[0][3]  +  this->m[2][1] * that.m[1][3]  +  this->m[2][2] * that.m[2][3]  +  this->m[2][3] * that.m[3][3];

	tmp.m[3][0] = this->m[3][0] * that.m[0][0]  +  this->m[3][1] * that.m[1][0]  +  this->m[3][2] * that.m[2][0]  +  this->m[3][3] * that.m[3][0];
	tmp.m[3][1] = this->m[3][0] * that.m[0][1]  +  this->m[3][1] * that.m[1][1]  +  this->m[3][2] * that.m[2][1]  +  this->m[3][3] * that.m[3][1];
	tmp.m[3][2] = this->m[3][0] * that.m[0][2]  +  this->m[3][1] * that.m[1][2]  +  this->m[3][2] * that.m[2][2]  +  this->m[3][3] * that.m[3][2];
	tmp.m[3][3] = this->m[3][0] * that.m[0][3]  +  this->m[3][1] * that.m[1][3]  +  this->m[3][2] * that.m[2][3]  +  this->m[3][3] * that.m[3][3];

	this->m[0][0] = tmp.m[0][0];	this->m[0][1] = tmp.m[0][1];	this->m[0][2] = tmp.m[0][2];	this->m[0][3] = tmp.m[0][3];
	this->m[1][0] = tmp.m[1][0];	this->m[1][1] = tmp.m[1][1];	this->m[1][2] = tmp.m[1][2];	this->m[1][3] = tmp.m[1][3];
	this->m[2][0] = tmp.m[2][0];	this->m[2][1] = tmp.m[2][1];	this->m[2][2] = tmp.m[2][2];	this->m[2][3] = tmp.m[2][3];
	this->m[3][0] = tmp.m[3][0];	this->m[3][1] = tmp.m[3][1];	this->m[3][2] = tmp.m[3][2];	this->m[3][3] = tmp.m[3][3];
	return *this;
}

Matrix4&
Matrix4::operator*=( float f )
{
	this->m[0][0] *= f;		this->m[0][1] *= f;		this->m[0][2] *= f;		this->m[0][3] *= f;
	this->m[1][0] *= f;		this->m[1][1] *= f;		this->m[1][2] *= f;		this->m[1][3] *= f;
	this->m[2][0] *= f;		this->m[2][1] *= f;		this->m[2][2] *= f;		this->m[2][3] *= f;
	this->m[3][0] *= f;		this->m[3][1] *= f;		this->m[3][2] *= f;		this->m[3][3] *= f;
	return *this;
}


Matrix4
Matrix4::operator*( float f )
{
	Matrix4 result = *this;
	result *= f;
	return result;
}


Matrix4
Matrix4::operator*( Matrix4& that )
{
	Matrix4 result = *this;
	result *= that;
	return result;
}


Point3
Matrix4::operator*( Point3& that )
{
	Point3 result;

	result.v0 = this->m[0][0]*that.v0 + this->m[0][1]*that.v1 + this->m[0][2]*that.v2 + this->m[0][3];
	result.v1 = this->m[1][0]*that.v0 + this->m[1][1]*that.v1 + this->m[1][2]*that.v2 + this->m[1][3];
	result.v2 = this->m[2][0]*that.v0 + this->m[2][1]*that.v1 + this->m[2][2]*that.v2 + this->m[2][3];
	return result;
}

Vec3
Matrix4::operator*( Vec3& that )
{
	Vec3 result;

	result.v0 = this->m[0][0]*that.v0 + this->m[0][1]*that.v1 + this->m[0][2]*that.v2;
	result.v1 = this->m[1][0]*that.v0 + this->m[1][1]*that.v1 + this->m[1][2]*that.v2;
	result.v2 = this->m[2][0]*that.v0 + this->m[2][1]*that.v1 + this->m[2][2]*that.v2;
	return result;
}

Matrix4&
Matrix4::operator+=( Matrix4& that )
{
	this->m[0][0] += that.m[0][0];   this->m[0][1] += that.m[0][1];   this->m[0][2] += that.m[0][2];   this->m[0][3] += that.m[0][3];
	this->m[1][0] += that.m[1][0];   this->m[1][1] += that.m[1][1];   this->m[1][2] += that.m[1][2];   this->m[1][3] += that.m[1][3];
	this->m[2][0] += that.m[2][0];   this->m[2][1] += that.m[2][1];   this->m[2][2] += that.m[2][2];   this->m[2][3] += that.m[2][3];
	this->m[3][0] += that.m[3][0];   this->m[3][1] += that.m[3][1];   this->m[3][2] += that.m[3][2];   this->m[3][3] += that.m[3][3];
	return *this;
}

Matrix4
Matrix4::operator+( Matrix4& that )
{
	Matrix4 result = *this;
	result += that;
	return result;
}

Matrix4&
Matrix4::operator-=( Matrix4& that )
{
	this->m[0][0] -= that.m[0][0];   this->m[0][1] -= that.m[0][1];   this->m[0][2] -= that.m[0][2];   this->m[0][3] -= that.m[0][3];
	this->m[1][0] -= that.m[1][0];   this->m[1][1] -= that.m[1][1];   this->m[1][2] -= that.m[1][2];   this->m[1][3] -= that.m[1][3];
	this->m[2][0] -= that.m[2][0];   this->m[2][1] -= that.m[2][1];   this->m[2][2] -= that.m[2][2];   this->m[2][3] -= that.m[2][3];
	this->m[3][0] -= that.m[3][0];   this->m[3][1] -= that.m[3][1];   this->m[3][2] -= that.m[3][2];   this->m[3][3] -= that.m[3][3];
	return *this;
}

Matrix4
Matrix4::operator-( Matrix4& that )
{
	Matrix4 result = *this;
	result -= that;
	return result;
}

Matrix4&
Matrix4::operator-( )
{
	this->m[0][0] = -this->m[0][0];   this->m[0][1] = -this->m[0][1];   this->m[0][2] = -this->m[0][2];   this->m[0][3] = -this->m[0][3];
	this->m[1][0] = -this->m[1][0];   this->m[1][1] = -this->m[1][1];   this->m[1][2] = -this->m[1][2];   this->m[1][3] = -this->m[1][3];
	this->m[2][0] = -this->m[2][0];   this->m[2][1] = -this->m[2][1];   this->m[2][2] = -this->m[2][2];   this->m[2][3] = -this->m[2][3];
	this->m[3][0] = -this->m[3][0];   this->m[3][1] = -this->m[3][1];   this->m[3][2] = -this->m[3][2];   this->m[3][3] = -this->m[3][3];
	return *this;
}


Matrix4&
Matrix4::Ortho( float l, float r, float b, float t, float n, float f )
{
	Matrix4 tmp(
			2./(r-l), 0.,       0.,        (r+l)/(r-l),
			0.,       2./(t-b), 0.,        (t+b)/(t-b),
			0.,       0.,       -2./(f-n), (f+n)/(f-n),
			0.,       0.,       0.,        1.
	);

	this->operator*=( tmp );
	return *this;
}



Matrix4&
Matrix4::Ortho2D( float left, float right, float bottom, float top )
{
	this->Ortho( left, right,   bottom, top,   -1.0, 1.0 );
	return *this;
}



Matrix4&
Matrix4::Perspective( float fovy, float aspect, float near, float far )
{
	float rad = (M_PI/180.) * (fovy/2.);
	float t = sin( rad ) / cos( rad );

	float top = near * t;
	float bottom = -top;

	float left = aspect * bottom;
	float right = aspect * top;

	this->Frustum( left, right, bottom, top, near, far );
	return *this;
}



Matrix4&
Matrix4::PopMatrix4( )
{
	if( top > 0 )
	{
		top--;
		memcpy( &this->m[0][0], &stack[top][0][0], 16*sizeof(float) );
	}
	return *this;
}



Matrix4&
Matrix4::Print( char *str, FILE *fp )
{
	fprintf( fp, "%s \n", str );
	fprintf( fp, "\t%8.2f  %8.2f  %8.2f  %8.2f\n", this->m[0][0], this->m[0][1], this->m[0][2], this->m[0][3] );
	fprintf( fp, "\t%8.2f  %8.2f  %8.2f  %8.2f\n", this->m[1][0], this->m[1][1], this->m[1][2], this->m[1][3] );
	fprintf( fp, "\t%8.2f  %8.2f  %8.2f  %8.2f\n", this->m[2][0], this->m[2][1], this->m[2][2], this->m[2][3] );
	fprintf( fp, "\t%8.2f  %8.2f  %8.2f  %8.2f\n", this->m[3][0], this->m[3][1], this->m[3][2], this->m[3][3] );
	return *this;
}



Matrix4&
Matrix4::PushMatrix4( )
{
	if( top < STACK_SIZE-1 )
	{
		memcpy( &stack[top][0][0], &this->m[0][0], 16*sizeof(float) );
		top++;
	}
	return *this;
}


Matrix4&
Matrix4::Rotatef( float deg, float vx, float vy, float vz )
{
	// make the axis a unit vector:

	float d = sqrt( vx*vx + vy*vy + vz*vz );
	vx /= d;
	vy /= d;
	vz /= d;

	// create the perpendicular matrix:
	// [perp] = [I] - [parr]
	// where
	//	ppar  =  v ( v . p )

	Matrix4 parr(	vx*vx, vx*vy, vx*vz, 0.f,
			vx*vy, vy*vy, vy*vz, 0.f,
			vx*vz, vy*vz, vz*vz, 0.f,
			0.,    0.,    0.,    1.f
	);

	Matrix4 perp(	1. - vx*vx, -vx*vy,     -vx*vz,     0.f,
			-vx*vy,     1. - vy*vy, -vy*vz,     0.f,
			-vx*vz,     -vy*vz,     1. - vz*vz, 0.f,
			0.f,        0.f,        0.f,        1.f
	);

	// create the up matrix:
	// up =   p   X v

	Matrix4 up(	0., -vz,  vy, 0.f,
			 vz,  0., -vx, 0.f,
			-vy,  vx,  0., 0.f,
			 0.,  0.,  0., 1.f
	);


	// [M] = [parr] + c*[perp] + s*[up]

	float rad = ( M_PI/180. ) * deg;
	float c = cos( rad );
	float s = sin( rad );

	perp *= c;
	up   *= s;
	Matrix4 tmp = parr + perp + up;
	this->operator*=( tmp );
	this->m[3][3] = 1.;
	return *this;
}



Matrix4&
Matrix4::Scalef( float sx, float sy, float sz )
{
	Matrix4 tmp(
			sx, 0., 0., 0.,
			0., sy, 0., 0.,
			0., 0., sz, 0.,
			0., 0., 0., 1.
	);

	this->operator*=( tmp );
	return *this;
}


Matrix4&
Matrix4::SetElement( int i, int j, float value )
{
	if( i < 0  ||  i > 3 )		return *this;
	if( j < 0  ||  j > 3 )		return *this;
	this->m[i][j] = value;
	return *this;
}


Matrix4&
Matrix4::SetMatrix4( float in[4][4] )
{
	this->m[0][0] = in[0][0];    this->m[0][1] = in[0][1];    this->m[0][2] = in[0][2];    this->m[0][3] = in[0][3];   
	this->m[1][0] = in[1][0];    this->m[1][1] = in[1][1];    this->m[1][2] = in[1][2];    this->m[1][3] = in[1][3];   
	this->m[2][0] = in[2][0];    this->m[2][1] = in[2][1];    this->m[2][2] = in[2][2];    this->m[2][3] = in[2][3];   
	this->m[3][0] = in[3][0];    this->m[3][1] = in[3][1];    this->m[3][2] = in[3][2];    this->m[3][3] = in[3][3];   
	return *this;
}


Matrix4&
Matrix4::SetMatrix43( float in[3][3] )
{
	this->m[0][0] = in[0][0];    this->m[0][1] = in[0][1];    this->m[0][2] = in[0][2];
	this->m[1][0] = in[1][0];    this->m[1][1] = in[1][1];    this->m[1][2] = in[1][2];
	this->m[2][0] = in[2][0];    this->m[2][1] = in[2][1];    this->m[2][2] = in[2][2];
	this->m[0][3] = this->m[1][3] = this->m[2][3] = 0.;
	this->m[3][0] = this->m[3][1] = this->m[3][2] = 0.;
	this->m[3][3] = 1.;
	return *this;
}

Matrix4&
Matrix4::StereoPerspective( float fovy, float aspect, float znear, float zfar, float z0p, float eyesep )
{
	float left, right;	// x boundaries on z0p	
	float bottom, top;	// y boundaries on z0p	
	float tanfovy;		// tangent of y fov angle

	// tangent of the y field-of-view angle:	

	tanfovy = tan( fovy * (M_PI / 180.) / 2. );

	// top and bottom boundaries:	

	top = z0p * tanfovy;
	bottom = -top;

	// left and right boundaries come from the aspect ratio:

	right = aspect * top;
	left =  aspect * bottom;

	// take eye translation into account:

	left  -= eyesep;
	right -= eyesep;

	// ask for a window in terms of the z0p plane:	

	this->FrustumZ( left, right, bottom, top, znear, zfar, z0p );
	this->Translatef( -eyesep, 0.0, 0.0 );
	return *this;
}


Matrix4&
Matrix4::Translatef( float tx, float ty, float tz )
{
	Matrix4 tmp(
			1., 0., 0., tx,
			0., 1., 0., ty,
			0., 0., 1., tz,
			0., 0., 0., 1.
	);

	this->operator*=( tmp );
	return *this;
}


Matrix4&
Matrix4::Transpose( )
{
	Matrix4 tmp = *this;
	this->m[0][0] = tmp.m[0][0];    this->m[1][0] = tmp.m[0][1];    this->m[2][0] = tmp.m[0][2];    this->m[3][0] = tmp.m[0][3];   
	this->m[0][1] = tmp.m[1][0];    this->m[1][1] = tmp.m[1][1];    this->m[2][1] = tmp.m[1][2];    this->m[3][1] = tmp.m[1][3];   
	this->m[0][2] = tmp.m[2][0];    this->m[1][2] = tmp.m[2][1];    this->m[2][2] = tmp.m[2][2];    this->m[3][2] = tmp.m[2][3];   
	this->m[0][3] = tmp.m[3][0];    this->m[1][3] = tmp.m[3][1];    this->m[2][3] = tmp.m[3][2];    this->m[3][3] = tmp.m[3][3];   
	return *this;
}



// #define TEST

#ifdef TEST

int
main( int argc, char *argv[ ] )
{
	Matrix4 I;
	I.Print( "I = " );

	Matrix4 R;
	R.Rotatef( 30.,  1., 0., 0. );
	R.Print( "R = " );

	Matrix4 T;
	T.Translatef( 2., 3., 4. );
	T.Print ( "T = " );

	Matrix4 P = R * T;
	P.Print( "P1 = " );

	P = T * R;
	P.Print( "P2 = " );

	P = T*R + R*T;
	P.Print( "P3 = " );

	Matrix4 RI = R;
	RI.Invert( );
	RI.Print( "Rinverse = " );
	RI *= R;
	RI.Print( "Rinverse * R = " );

#define A	1.
#define B	2.
#define C	3.
	Matrix4 Comp;
	Comp.LoadIdentity( ).Translatef( A, B, C ).Scalef( 3., 4., 1. ).Translatef( -A, -B, -C ).Print( "Composite = " );
	Comp.Invert( ).Print( "Composite Inverse = " );

	fprintf( stderr, "Determinant of Composite Inverse = %8.3f\n", Comp.GetDeterminant( )  );

	return 0;
}

#endif		// TEST