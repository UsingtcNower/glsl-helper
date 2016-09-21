#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include <ctype.h>
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>

#ifdef WIN32
#include <windows.h>
#endif

#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <map>
#include <stdarg.h>
#include <vector>

#ifndef GL_COMPUTE_SHADER
#define GL_COMPUTE_SHADER	0x91B9
#endif


inline int GetOSU( int flag )
{
	int i;
	glGetIntegerv( flag, &i );
	return i;
}


void	CheckGlErrors( const char* );

bool checkFramebufferStatus();

class GLSLProgram
{
  private:
	std::map<char *, int>	AttributeLocs;
	char *			Cfile;
	unsigned int		Cshader;
	char *			Ffile;
	unsigned int		Fshader;
	char *			Gfile;
	GLuint			Gshader;
	bool			IncludeGstap;
	GLenum			InputTopology;
	GLenum			OutputTopology;
	GLuint			Program;
	char *			TCfile;
	GLuint			TCshader;
	char *			TEfile;
	GLuint			TEshader;
	std::map<char *, int>	UniformLocs;
	bool			Valid;
	char *			Vfile;
	GLuint			Vshader;
	bool			Verbose;
	std::vector<GLuint>	shaders;

	static int		CurrentProgram;

	void	AttachShader( GLuint );
	bool	CanDoBinaryFiles;
	bool	CanDoComputeShaders;
	bool	CanDoFragmentShaders;
	bool	CanDoGeometryShaders;
	bool	CanDoTessControlShaders;
	bool	CanDoTessEvaluationShaders;
	bool	CanDoVertexShaders;
	int	CompileShader( GLuint );
	bool	CreateHelper( char *, ... );
	int	GetAttributeLocation( char * );
	int	GetUniformLocation( char * );


  public:
		GLSLProgram( );
		~GLSLProgram();

	bool	Create( char *, char * = NULL, char * = NULL, char * = NULL, char * = NULL, char * = NULL );
	void	DispatchCompute( GLuint, GLuint = 1, GLuint = 1 );
	bool	IsExtensionSupported( const char * );
	bool	IsNotValid( );
	bool	IsValid( );
	void	LoadBinaryFile( char * );
	void	LoadProgramBinary( const char *, GLenum );
	void	SaveBinaryFile( char * );
	void	SaveProgramBinary( const char *, GLenum * );
	void	SetAttribute( char *, int );
	void	SetAttribute( char *, float );
	void	SetAttribute( char *, float, float, float );
	void	SetAttribute( char *, float[3] );
#ifdef VEC3_H
	void	SetAttribute( char *, Vec3& );
#endif
#ifdef VERTEX_ARRAY_H
	void	SetAttribute( char *, VertexArray&, GLenum );
#endif
#ifdef VERTEX_BUFFER_OBJECT_H
	void	SetAttribute( char *, VertexBufferObject&, GLenum );
#endif
	void	SetGstap( bool );
	void	SetInputTopology( GLenum );
	void	SetOutputTopology( GLenum );
	void	SetUniform( char *, int );
	void	SetUniform( char *, float );
	void	SetUniform( char *, float, float);
	void	SetUniform( char *, float, float, float );
	void	SetUniform( char *, float[3] );
#ifdef VEC3_H
	void	SetUniform( char *, Vec3& );
#endif
#ifdef MATRIX4_H
	void	SetUniform( char *, Matrix4& );
#endif
	void	SetVerbose( bool );
	void	Use( );
	void	Use( GLuint );
	void	UseFixedFunction( );
};

#endif		// #ifndef GLSLPROGRAM_H